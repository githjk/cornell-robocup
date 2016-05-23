#include "fast_pass.h"
#include "info/robot/robot.h"
#include "info/ball/ball.h"
#include "math/tangent.h"
#include "intersect/intersect.h"
#include "shoot/shoot.h"
#include "math/angle.h"

//---------------------------------------------------------
//Use this algorithm to send robots into fast-pass lanes
//---------------------------------------------------------
void setupFastPass(const VisionData& field,
                   RobocupStrategyData* sd,
                   RobotIndex robotID,
                   Destination* command)
{
    //Use this to test the fast pass lane routine

	//Enemy_Goal_Left
	Pair enemyGoalLeft( sd->getSystemParams().field.THEIR_GOAL_LINE, sd->getSystemParams().field.LEFT_GOAL_POST);
	//Enemy_Goal_Right
	Pair enemyGoalRight(sd->getSystemParams().field.THEIR_GOAL_LINE, sd->getSystemParams().field.RIGHT_GOAL_POST);
	//Ball Position
	Pair ballPosition = getRobocupBall(field).getPos();
	

	/* UNCOMMENT THIS TO ENABLE THIS CHECK IN THE BEGINNING OF FUNCTION, AND NOT THE END.
	// YOU ALSO NEED TO COMMENT OUT SOME LINES AT END OF FUNCTION TO DISABLE THE CHECK THERE.
	//Distance from robot to ball
	float distanceToBall = thisRobotLoc.distanceTo(ballPosition);

	if(distanceToBall < 10*sd->getSystemParams().general.PLAYER_RADIUS)
	{
		sd->setPassValue(robotID, 0);
		return;
	}
	*/


	//Goalie_Position
	Pair goaliePosition;
	RobotIndex theirGoalieID = NO_ROBOT;
	if((getTheirGoalie(field, sd->getSystemParams(), &theirGoalieID)) &&
	   (getTheirYLocation(theirGoalieID, field, sd->getSystemParams()) < enemyGoalLeft.getY()) &&
	   (getTheirYLocation(theirGoalieID, field, sd->getSystemParams()) > enemyGoalRight.getY()))

	{	//If found, set position of goalie
		goaliePosition = getTheirRobotLocation(theirGoalieID, field, sd->getSystemParams());
	}
	else
	{	//Else, say goalie is off to side of goal, so we don't avoid him at all =-)
		goaliePosition.setX(sd->getSystemParams().field.THEIR_GOAL_LINE);
		//if ball is on the left side, throw the goalie on the right, else on the left
		if(ballPosition.getY() > 0.0f)
			goaliePosition.setY(sd->getSystemParams().field.LEFT_GOAL_POST + sd->getSystemParams().general.OPPONENT_RADIUS);
		else
			goaliePosition.setY(sd->getSystemParams().field.RIGHT_GOAL_POST - sd->getSystemParams().general.OPPONENT_RADIUS);
	}

	//This robot's position
	Pair thisRobotLoc = getRobot(robotID, field, sd->getSystemParams()).getPos();
	//Make array of obstacles
	Obstacles enemyObstacles;
	findObstacles(field, sd->getSystemParams(), robotID, enemyObstacles.getObstacleArray());
	//We will use only the first 5 elements(the enemy robots)

	//Optimal pass distance
	const float optimalDistance = 1.20f;//m

    Pass bestPass = findFastPassLane(field,
                                     sd->getSystemParams(),
                                     goaliePosition,
                                     ballPosition,
                                     &enemyObstacles,
                                     optimalDistance,
                                     thisRobotLoc);

	float openGoalWidthLeft = ABS(enemyGoalLeft.getY() - goaliePosition.getY());
	float openGoalWidthRight = ABS(goaliePosition.getY() - enemyGoalRight.getY());
	
	Pair shootToPoint;	
	
	// Shoot at the largest hole in the goal.  The ball is aimed at the point in the  	
	// middle of the largest hole.
	if(openGoalWidthLeft > openGoalWidthRight)
	{
		shootToPoint.set(enemyGoalRight.getX(), 
				(enemyGoalLeft.getY() - (0.5f * (openGoalWidthLeft - sd->getSystemParams().general.OPPONENT_RADIUS))));
		//shootToPoint.set(enemyGoalRight.getX(), (enemyGoalLeft.getY() + goaliePosition.getY())/2);
	}
	else
	{
		shootToPoint.set(enemyGoalRight.getX(), 
				(enemyGoalRight.getY() + (0.5f * (openGoalWidthRight - sd->getSystemParams().general.OPPONENT_RADIUS))));
		//shootToPoint.set(enemyGoalRight.getX(), (enemyGoalLeft.getY() + goaliePosition.getY())/2);		
	}
	
    command->setRotation( (angleBetween(thisRobotLoc,shootToPoint) + angleToBall(thisRobotLoc, field))
                          /2);
	
    command->setPos(bestPass.getPassPos());
    command->setKick(KICK_SHOT);

	//-------------------------------------------------
	//Set the pass_position and value so that the passer
	//knows where we are going and if he can pass to us
	sd->setPassDest(robotID, frontOfRobot(robotID, field, sd->getSystemParams()));

	float distanceFromDestination = thisRobotLoc.distanceTo(bestPass.getPassPos());

	float distanceToBall = thisRobotLoc.distanceTo(ballPosition);
	
	if(
	   //If I can see the ball...
	   isLane(thisRobotLoc,
			  ballPosition,
              2*sd->getSystemParams().general.BALL_RADIUS,
	   		  field,
			  sd->getSystemParams(),
              true) &&
	   //And there is a clear shot on the net
	   clearShot(robotID, 
	             field,
				 sd->getSystemParams()) &&
	   
		//..and I'm near my destination
	   (distanceFromDestination <= (2*sd->getSystemParams().general.PLAYER_RADIUS)) &&
	   
	   //..and I'm near the ball (so we don't pass all the way down the field
	   (distanceToBall < 10*sd->getSystemParams().general.PLAYER_RADIUS)
	   // SHOULDN'T WE CHECK AT LEAST ONE OF THE ABOVE PARAMETERS *BEFORE* WE DO CALCULATIONS??
	  )
    {
	  sd->setPassValue(robotID, 1);
    }
	else
    {
	  sd->setPassValue(robotID, 0);
    }
	//-------------------------------------------------	

}

//*********************************************************************************

Pass findFastPassLane(const VisionData& field, 
					  const SystemParameters& params,
					  Pair goaliePosition,
					  Pair ballPosition,
					  Obstacles* incomingObstacles, 
				      float optimalPassDistance,
					  Pair assigningRobot
                     )
{

    //-----------------------------------------------------------------
	//Step 1.) Prepare list of obstacles
	incomingObstacles->ignoreUs();
	incomingObstacles->ignoreLost();
	incomingObstacles->ignoreIfBehindX(params.field.THEIR_GOAL_LINE/2);

    Pair obstacles[MAX_OBSTACLES];
    int numObstacles = incomingObstacles->copyObstacles(obstacles);
    ASSERT(numObstacles <= 5, "something funky happened.");

	//-----------------------------------------------------------------
	//Step 2.) Find Shot Target
	Pair shotTarget; //find midpoint between goalie and close post
					
	//if no goalie (no obstacles), use center of goal
	shotTarget.setX(params.field.THEIR_GOAL_LINE);

	if(ballYLocation(field) < 0.0f) //ball is on the left
		shotTarget.setY((goaliePosition.getY() + params.general.OPPONENT_RADIUS + params.field.LEFT_GOAL_POST)/2);
	else
		shotTarget.setY((goaliePosition.getY() - params.general.OPPONENT_RADIUS + params.field.RIGHT_GOAL_POST)/2);
	//-----------------------------------------------------------------
	//Step 3) Find Sweet Spots
	Pass passLanes[21]; //5_enemy_bots*4 + 1_default_position = 21
	int numPassLanes = 0; //by default. as we add them, we will increment this counter

	Pair ballObstacleCase1Tangent,
			 ballObstacleCase2Tangent,
			 shootTargetObstacleCase1Tangent,
			 shootTargetObstacleCase2Tangent;

	Line ballToTangent, goalToTangent;	

	float tangentRadius = 2.2f*params.general.OPPONENT_RADIUS;

	//for every obstacle, find the 4 possible positions
    int i;
	for(i=0; i<numObstacles; i++)
	{
		//If we can find all the tangents, can by all means, 
		//find the four intersection points
		if(
		   (findTangents( obstacles[i], //the obstacles
			              ballPosition, //the ball
						  tangentRadius, //radius off obstacle
						  &ballObstacleCase1Tangent,
						  &ballObstacleCase2Tangent)) &&
		   (findTangents( obstacles[i], //the obstacles
			              shotTarget,   //the ball
						  tangentRadius, //radius off obstacle
						  &shootTargetObstacleCase1Tangent,
						  &shootTargetObstacleCase2Tangent))
		  )
		{
			//Same for all 4 intersections
			ballToTangent.setA(ballPosition);
			goalToTangent.setA(shotTarget);

			//Intersection #1
			ballToTangent.setB(ballObstacleCase1Tangent);
			goalToTangent.setB(shootTargetObstacleCase1Tangent);
			passLanes[numPassLanes].setPassPos(findLineIntersection(ballToTangent, goalToTangent));
			passLanes[numPassLanes].setPassValue(1);

			//Intersection #2
			ballToTangent.setB(ballObstacleCase2Tangent);
			goalToTangent.setB(shootTargetObstacleCase1Tangent);
			passLanes[numPassLanes+1].setPassPos(findLineIntersection(ballToTangent, goalToTangent));
			passLanes[numPassLanes+1].setPassValue(1);

			//Intersection #3
			ballToTangent.setB(ballObstacleCase1Tangent);
			goalToTangent.setB(shootTargetObstacleCase2Tangent);
			passLanes[numPassLanes+2].setPassPos(findLineIntersection(ballToTangent, goalToTangent));
			passLanes[numPassLanes+2].setPassValue(1);

			//Intersection #4
			ballToTangent.setB(ballObstacleCase2Tangent);
			goalToTangent.setB(shootTargetObstacleCase2Tangent);
			passLanes[numPassLanes+3].setPassPos(findLineIntersection(ballToTangent, goalToTangent));
			passLanes[numPassLanes+3].setPassValue(1);
			
			//We just added 4 sweet spots =-)
			numPassLanes+=4;
		}
	}
	//-----------------------------------------------------------------
	//Step 4.) Increase the scores of sweet spots if they 
	//         cannot see the ball or the shoot to point
	for(i=0; i <numPassLanes; i++)
	{
	  if(
          (!isPassLane(passLanes[i].getPassPos(),
					   ballPosition,
					   assigningRobot,
					   field,
					   params)) ||
		  (!isShot(passLanes[i].getPassPos(),
				   shotTarget,
				   assigningRobot,
				   field,
				   params))
		  )
        {
		  passLanes[i].setPassValue(0); 
        }
	}
	//-----------------------------------------------------------------
	//Step 5.) Remove points that are off the field, 
	//         or in the goalie box,
	//         or just have a x position past their goalie box
	//         or on the same side of the field as the ball,
	//         or on or side of the field (x < 0)
	for(i=0; i<numPassLanes; i++)
	{
		if(
           //off the field
		   (!isLocationInField(passLanes[i].getPassPos(), params)) ||

		   //in their goalie box
		   (inTheirGoalieBox(passLanes[i].getPassPos(), params)) ||

		   //past goal box line
		   (passLanes[i].getPassPos().getX() > (params.field.THEIR_GOALIE_BOX - params.general.PLAYER_RADIUS)) ||

		   //is too close to the ball
		   // Shouldn't we use sqrt(a^2 + b^2) instead of just the difference in y?
		   (ABS(passLanes[i].getPassPos().getY() - ballPosition.getY()) < 0.30f) ||

           //on our side of the field
		   (passLanes[i].getPassPos().getX() <= 0.0f)
		  )
        {
		  passLanes[i].setPassValue(0); 
        }
    }
    //-----------------------------------------------------------------
    //Step 6.) Find the best pass, determined by the difference between
    // the location's distance to the ball and teh optimal pass distance.
    //(Excluding those pass locations already marked with value = 0;
    float bestScore = 99999.0f;
    int bestPass = -1; // (corresponds to bestScore)

    float tempScore;
    for(i=0; i<numPassLanes; i++)
    {
      if(passLanes[i].getPassValue())
      {
        tempScore = ABS(passLanes[i].getPassPos().distanceTo(ballPosition)
                        - optimalPassDistance);
        if (tempScore < bestScore)
        {
          bestPass = i;
          bestScore = tempScore;
        }
      }
    }

    //-------------------------------------------------------------------
    //Step 7.) Return the best pass lane 

    if(bestPass != -1)
    {
        return passLanes[bestPass];
    }
    else
    {
      //
      //Oi Vey!  Not a single pass is legal.
      //
      //let's make something up:
      // use the point halfway between middle of field and enemy goal, and
      // on the same side of the field as the ball.
    
      Pass newPass;
      if(ballYLocation(field) > 0.0f)
          newPass.setPassPosY(params.field.RIGHT_GOALIE_BOX + (2.0f*params.general.PLAYER_RADIUS));
      else
          newPass.setPassPosY(params.field.LEFT_GOALIE_BOX - (2.0f*params.general.PLAYER_RADIUS));
        
      newPass.setPassPosX(params.field.THEIR_GOALIE_BOX - (2.0f*params.general.PLAYER_RADIUS));                
      newPass.setPassValue(1);

      return newPass;
    }

}

//*********************************************************************************

bool trapFastPass(const VisionData& field,
                  RobocupStrategyData& sd,
                  RobotIndex robotID,
                  Destination* command)
{

	//========Find the point on the goal that we want to shoot into====================
	
  Pair ballPosition = getRobocupBall(field).getPos();
  Pair robotPosition = getLocation(robotID, field, sd.getSystemParams());

  Pair target;
  // HOW CAN YOU SEE IF THERE IS A CLEAR SHOT WHEN TARGET ISN'T INITIALIZED?
  if (! clearShot(robotID, field, sd.getSystemParams(), &target) )
  {
    command->setPos(ballPosition);
    command->setRotation(angleToBall(robotPosition, field));
    return false;
  }
  
  Line ballMovement = getRobocupBall(field).getMovementVector();
  Line strikingLine(sd.getPassDest(robotID), target);
  
  //-------------------------------------------------------------	
  //Find the intersection...
  Pair destination = findLineIntersection(ballMovement, strikingLine);
  
  //if no intersection, which is really bad, then return false
  if(destination.getX() == NEVER_INTERSECT) 
  {
    command->setPos(ballPosition);
    command->setRotation(angleToBall(robotPosition, field));
    return false;
  }
  
  
  command->setRotation( (angleBetween(robotPosition,target) + angleBetween(robotPosition, ballPosition))
                        /2);

  float robotRotation = getRotation(robotID, field, sd.getSystemParams());
  command->setXPos(destination.getX() - sd.getSystemParams().general.PLAYER_RADIUS * COS(robotRotation));
  command->setYPos(destination.getY() - sd.getSystemParams().general.PLAYER_RADIUS * SIN(robotRotation));
  
  //when receiving a fast pass, always arm the kicker 
  command->setKick(KICK_SHOT);
  
  float diffAngle = normalizeAngle(
                       (angleBetween(robotPosition, ballPosition) + PI )
                       - ATAN2( ballYVel(field), ballXVel(field) ));
  
  
  if ( ABS(diffAngle) > (PI / 2.5) )
    return false;
  
  return true;
}