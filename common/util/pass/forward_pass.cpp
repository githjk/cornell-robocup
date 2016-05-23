#include "forward_pass.h"

//******************************************
//******************************************
//*************Forward Pass*****************
//******************************************
//******************************************



//==================================================================
//Static variables for trapForwardPass

//Make two lines...
static Line ballMovement;
static Line horizontalMovement; //based on our X reception we set when waiting to receive the ball
//Find the intersection...
static Pair *destination;

//---------------------------------------------------------
//When you are into the forward passing mode, that is, the
//passer has kicked the ball, have the receiving robot call
//this procedure to carefully receive the pass
//---------------------------------------------------------
bool trapForwardPass(const VisionData& field,
                     RobocupStrategyData* sd,
                     //robots Position
                     RobotIndex robotID,
                     //------------------------------
                     // Destination to send robot to
                     //------------------------------
                     Destination* sendTo)
{
  //------------------------------------
  //First, initialize the destination...
  //------------------------------------
  sendTo->initialize();

  //------------------------------------
  //Make two lines...
  //------------------------------------
  //Line one is the ball's velocity vector
  //------------------------------------
  //starting with it's current position
  ballMovement.setAposX( ballXLocation(field) );
  ballMovement.setAposY( ballYLocation(field) );
  ballMovement.setBposX( ballMovement.getAposX() +
                         10*ballXVel(field) );
  ballMovement.setBposY( ballMovement.getAposY() +
                         10*ballYVel(field) );

  //------------------------------------
  //The second line assumes we have our pass-x position
  //correct, and that all movment will be along the
  //width of the field to receive the pass
  //------------------------------------
  horizontalMovement.setAposX( sd->getPassDest(robotID).getX() );
  horizontalMovement.setBposX( sd->getPassDest(robotID).getX() );
  horizontalMovement.setAposY( sd->getSystemParams().field.LEFT_SIDE_LINE );
  horizontalMovement.setBposY( sd->getSystemParams().field.RIGHT_SIDE_LINE );

  //------------------------------------
  //Find the intersection... (where we receive pass)
  //------------------------------------
  destination = new Pair();
  findLineIntersection(ballMovement, horizontalMovement, destination);

  //------------------------------------
  //Under certain cases, we need to redecide
  //where to get the ball (as in it won't
  //be a simple intersection problem)
  //------------------------------------
  //If no intersection was found....
  if(
     (destination->getX() == -32000) ||
     (ballXVel(field)== 0.0f)
    )
  {
    destination->setX( ballXLocation(field) );
    destination->setY( ballYLocation(field) );
  }

  //------------------------------------
  //Set the destination...
  //------------------------------------
  sendTo->setPos((*destination));

  //------------------------------------
  //when trapping a forward pass, arm dribbler so we can catch it easily
  //------------------------------------
  sendTo->setDribble(DRIBBLE_DEFAULT);

  //--------------------------------------------------------------------
  //when trapping a forward pass, look straight at the ball at all times
  //--------------------------------------------------------------------
  sendTo->setRotation( angleToBall(robotID, field, sd->getSystemParams()) );

  //------------------------------------
  //If the intersection is off the field,
  //or in their goalie box,
  //or the ball has already passed us,
  //then return false
  //------------------------------------
  if(
     (offField(sd->getSystemParams(), (*destination))) ||
     (nearTheirGoalieBox(*destination, sd->getSystemParams(), (float)(1.5*sd->getSystemParams().general.PLAYER_RADIUS))) ||
     (ballXLocation(field) > (getXLocation(robotID, field, sd->getSystemParams())))
    )
    return false;
  else
    return true;
}











//==================================================================
//Static variables for Receive_Forward_Pass
//Ball Location
static Pair ballLocation;
//Make array of obstacles
static Pair theObstacles[2*NUM_PLAYERS_ON_TEAM - 1];
//Optimal pass distance
static float optimalDistance;
//This robot's position
static Pair thisRobotLocation;
//Other robots also trying to get forward passes
static Pair otherRobot[NUM_PLAYERS_ON_TEAM - 1];
static int numberOfAssigningRobots;
//Ok, ready to find position!!! =-)
static Pair* endpoint;
//used for deciding on final angle...
static float angleToTheBall, angleToTheGoal;
//used to hold the goals position, used in determining final angle for robot
static Pair goalLocation;
static float distanceFromDestination;
static float angleToTarget;
//used to iterate
static int i;

//Static variables used in Forward_Pass_Lane

//holds how many obstales we have to consider
static int numberOfObstacles;
//holds how many sweet spots we are considering
static int numberOfPassLanes;
//used for index'ing arrays, uncomment if needed
//static int i;
//used to compact teh fragmented obstacle array
static int freePointer;
static int currPointer;
//used to swap Pairs in an array
static Pair tempPair;
//actual pass lanes (sweet spots)
static SweetSpot passLane[2*NUM_PLAYERS_ON_TEAM + 5];
static int currentPassLane;
static float subtractionDistance;
static float ballObstacleSlope;
static float dx, dy;
static int j;
static float ballPassDistance;
static float difference;
static SweetSpot sweetTemp;

//---------------------------------------------------------
//Finally, receiveForwardPass calls forwardPassLane to do
//all the work. It gets back the results and sets the robot
//to that position
//---------------------------------------------------------

void findForwardPassLane(const VisionData& field,
                         RobocupStrategyData* sd,
                         const Pair& ballLocation,
                         Pair primitiveObstacle[2*NUM_PLAYERS_ON_TEAM - 1],
                         const float& optimalDistance,
                         //So we don't see outself as an obstacle
                         const Pair& assigningRobot,
                         //------------------------------------------
                         // Return pair here
                         //------------------------------------------
                         Pair* passlane)
{
  //------------------------------------------------------------------------
  // Assume there are NUM_PLAYERS_ON_TEAM obstacles to consider (enemy bots)
  //------------------------------------------------------------------------
  numberOfObstacles = NUM_PLAYERS_ON_TEAM;

  //-----------------------------------------------------------------
  // Step 1.)
  // Go through obstacles and "remove" (set as invalid) those
  // that will not apply (either not found or behind the ball, 
  // or robots on our team)
  //-----------------------------------------------------------------
  for(i = 0; i < (2*NUM_PLAYERS_ON_TEAM - 1); i++)
  {
    //If either obstacle coordinate invalid, set both to be invalid
    if(
       (primitiveObstacle[i].getX() == -32000.0f) ||
       (primitiveObstacle[i].getY() == -32000.0f)
      )
    {
      primitiveObstacle[i].setX(-32000.0f);
      primitiveObstacle[i].setY(-32000.0f);
      numberOfObstacles--;
    }

    //If found, check to see if obstacle is not between the enemy goal and the ball.
    //(Since we are passing forward, obstacles behind us are irrelovent)
    else if(
            (
             (primitiveObstacle[i].getX() > sd->getSystemParams().field.THEIR_GOAL_LINE) && 
             (primitiveObstacle[i].getX() > ballLocation.getX() )
            ) ||
            (
             (primitiveObstacle[i].getX() < sd->getSystemParams().field.THEIR_GOAL_LINE) && 
             (primitiveObstacle[i].getX() < ballLocation.getX() )
            )
           )
    {
      //Since not in the path of play, set as invalid.
      primitiveObstacle[i].setX(-32000.0f);
      primitiveObstacle[i].setY(-32000.0f);
      numberOfObstacles--;
    }

    //If you are receiving a forward pass (presumable from the goalie)
    //then always want to be on our side of the field, so disregard
    //obstacles on the other side of the field
    else if(primitiveObstacle[i].getX() > 0.0f)
    {
      //Since not in the path of play, set as invalid.
      primitiveObstacle[i].setX(-32000.0f);
      primitiveObstacle[i].setY(-32000.0f);
      numberOfObstacles--;
    }   
   
    //Finally, remove all of our own friendly people. We don't consider
    //our friends to be obstacles...
    else if(i >= NUM_PLAYERS_ON_TEAM)
    {
      //Since not in the path of play, set as invalid.
      primitiveObstacle[i].setX(-32000.0f);
      primitiveObstacle[i].setY(-32000.0f);
    }   
  }

  //---------------------------------------------------------------------
  //Step 2.)
  //Compact obstacle array such all valid obstacles are placed at the front
  //Delete old entries for safety reasons.
  //---------------------------------------------------------------------
  freePointer = 0;
  currPointer = 0;
  for(currPointer = 0; currPointer < (2*NUM_PLAYERS_ON_TEAM - 1); currPointer++)
  {
    //If we every find a valid obstacle, move it towards 
    //the front at the free space
    if(
       (primitiveObstacle[currPointer].getX() != -32000.0f) &&
       (freePointer != currPointer)
      )    
    {
      primitiveObstacle[freePointer] = primitiveObstacle[currPointer];
      primitiveObstacle[currPointer].setX(-32000.0f);
      primitiveObstacle[currPointer].setY(-32000.0f);
      freePointer++;
    }
  }

  //-----------------------------------------------------------------
  // Step 3.) 
  // Now that we have an array of obstacles,
  // we know which ones are valid, and we know
  // how many obstacles are valid, make a list of
  // "sweet spots." A sweet spot is a good place to be
  // to receive a pass. For example, between and yet behind
  // two enemy robots, to split them and get the ball upfield
  // from them, or in front of an opponent, such that a pass would
  // be clean and easy, and a simple spin around the opponent would
  // suddenly have you moving upfield and past the enemy robots.
  //-----------------------------------------------------------------

  //First plan your sweet spots. For now, I've come up with three kinds of
  //sweet spots.

  //Type 1.)
  //it is always nice to split defenders. If I have N valid obstacles, then I can
  //find N-1 points that split each pair of consecutive obstacles. For example,
  //if I have robots 1 2 3 lined up just like that, I can find a sweet spot between
  //and slightly behind robots 1 and 2, and another sweet spot behind robots 2 and 3.
  //### Thus I will have (Number_of_Obstacles - 1) sweet spots for type 1.

  //Type 2.) 
  //It is often useful to get the ball just in front of an enemy robot,
  //and then spit around him to take the ball upfield. In this case,
  //### there will be (Number_of_Obstacles) sweet spots for type 2.

  //Type 3.)	
  //At any time a large "hole" may develop on the field. being in
  //some place in this hole might be nice, as you could easily move back and forth to
  //get a pass, adjusting as the ball might slightly move off course. It would also
  //provide you ample space to move the ball upfield, as you would be in a hole and thus
  //few people would be around. 
  //### This is really a special case. Use the find_open_space algorithm to find an open space
  //in whatever quadrant you wish
	
  //From above, you can see I only need to allocate space for two types of sweet spots, so
  //I'll need an array of ((Number_of_Obstacles * 2) - 1) sweet spots.
  numberOfPassLanes = ((numberOfObstacles * 2) - 1);

  //this flips out if there are no obstacles, in which case, set
  //# pass lanes to 0 now (rather than later and have it flip out
  if(numberOfPassLanes == -1)
    numberOfPassLanes++;

  //Make array of possible pass lanes

  //Go through each pass lane and initialize it's position and
  //score. Scores should be > 0 and smaller scores are "better." 
  //This allows us to give priority to certain types of passes over
  //others.


  //-----------------------------------------------------------------
  //      Order obstacles by Y value
  //-----------------------------------------------------------------
  //First, order the obstacles so they go from left to right (by y position)
	                                                 
  if(numberOfObstacles > 1) //else no need
  {
    for(int n=0; n<(numberOfObstacles - 1); n++)
    {
      for(int m=0; m<(numberOfObstacles - n - 1); m++)
      {
        //if left is bigger than right, then swap the two obstacles.
        if( primitiveObstacle[m].getY() > primitiveObstacle[m+1].getY() )
        {
          tempPair = primitiveObstacle[m];
          primitiveObstacle[m] = primitiveObstacle[m+1];
          primitiveObstacle[m+1] = tempPair;
        }
      }
    }	
  }
  //-----------------------------------------------------------------
  //      End ordering obstacles by Y value
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  //      Create Pass Lanes
  //-----------------------------------------------------------------
  //We'll say the first (Number_of_Obstacles - 1) are 
  //type 1 (split the defenders)
  //-------------------------------------------------
  for(i=0; i<(numberOfObstacles-1); i++)
  {
    //arbitrary distance to move back :)
    extendVectorBallMidpoint(ballLocation,
                             primitiveObstacle[i],
                             primitiveObstacle[i+1],
                             0.45f,                  //45cm
                             &passLane[i].point); 

    //default score before checking occurs
    passLane[i].score = 1.0f;
  }

  //-------------------------------------------------
  //Next we'll do type 2 (pivot of defender) 
  //which has (Number_of_Obstacles) cases
  //-------------------------------------------------
  for(i=0; i<numberOfObstacles; i++)
  {
    //find point below defender, and set pass-lane
    //---
    //draw vector from ball to obstacle
    //find slope
    //move toward ball from defender along that vector by set distance

    //avoid recalculate several times
    currentPassLane = (numberOfObstacles - 1 + i); 
   
    //default pivoting distance
    subtractionDistance = 0.25f;       //25cm

    if( (primitiveObstacle[i].getX() - ballLocation.getX()) == 0.0f )
    {
      passLane[currentPassLane].point.setX( primitiveObstacle[i].getX() );
      passLane[currentPassLane].point.setY( primitiveObstacle[i].getY() - subtractionDistance );
    }
    else
    {
      ballObstacleSlope = (
                           ( primitiveObstacle[i].getY() - ballLocation.getY() ) /
                           ( primitiveObstacle[i].getX() - ballLocation.getX() )
                          );

      dx = SQRT(
                (subtractionDistance * subtractionDistance) /
                (1 + (ballObstacleSlope * ballObstacleSlope))
               );
      dy = SQRT(
                (subtractionDistance * subtractionDistance) -
                (dx * dx)
               );    


      passLane[i].point.setX( primitiveObstacle[i].getX() - dx );

      //If slope is < 0 then add multitude of slope.
      if(ballObstacleSlope < 0)
        passLane[i].point.setY( primitiveObstacle[i].getY() + dy );

      //Else subtract multitude of slope.
      else
        passLane[i].point.setY( primitiveObstacle[i].getY() - dy );
    }

    //set default score before checking
    passLane[i].score = 1.0f;

  }
  //-----------------------------------------------------------------
  //      End Creating Pass Lanes
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  //      Mark Pass Lanes that are off field
  //-----------------------------------------------------------------
  for(i=0; i<numberOfPassLanes; i++)
  {
    //Off board? VERY bad!
    if( offField(sd->getSystemParams(), passLane[i].point) )
      passLane[i].score += 500.0f;
  }
  //-----------------------------------------------------------------
  //      End Mark Pass Lanes that are off field
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  //		Add special case sweet spots
  //-----------------------------------------------------------------
  //point point just pushed out away from our goal box, read to receive ball

  passLane[numberOfPassLanes].point.setX(
                                          sd->getSystemParams().field.OUR_GOALIE_BOX + 
                                          1.75f*sd->getSystemParams().general.PLAYER_RADIUS 
                                         );
  passLane[numberOfPassLanes].score = 1.0f;

  if(ballLocation.getY() > 0.0f)
    passLane[numberOfPassLanes].point.setY( 0.75f * sd->getSystemParams().field.LEFT_SIDE_LINE );
  else
    passLane[numberOfPassLanes].point.setY( 0.75f * sd->getSystemParams().field.RIGHT_SIDE_LINE );

  //we are adding one =-)
  numberOfPassLanes+=1;
  //-----------------------------------------------------------------
  //		End Add special case sweet spots
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  //      Mark Pass Lanes that are in shadows
  //-----------------------------------------------------------------
  for(i=0; i<numberOfPassLanes; i++)
  {
    //Check for obstacles using Nicole's snazzy algorithm =-)
    if(!isShot(passLane[i].point,
               ballLocation,
               assigningRobot,
               field,
               sd->getSystemParams()
              ))
    {
      //increase score to decrease chance of being chosen
      if(passLane[i].score > 0.0f)
        passLane[i].score += 200.0f;
    }
  }
  //-----------------------------------------------------------------
  //      End Mark Pass Lanes that are in shadows
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  //      Remove passes that are too short
  //-----------------------------------------------------------------
  for(i=0; i<numberOfPassLanes; i++)
  {
    //If pass is going ot be less than 10.5cm, then not really worth it...
    if( dist(passLane[i].point, ballLocation) < 0.105f ) 
      passLane[i].score +=500.0f;
  }
  //-----------------------------------------------------------------
  //      End Remove passes that are too short
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  // Remove passes on opposite side of field from goal (along the width)
  // b/c passing across the goal is very risky!
  //
  // Also remove all passes that woudl be across half field
  //-----------------------------------------------------------------
  for(i=0; i<numberOfPassLanes; i++)
  {
    //Across half field 
    //(I want to call this icing, but it's not, just plain risky though)
    if(passLane[i].point.getX() > 0.0f)
      passLane[i].score = -100.0f;

    //Pass across the crease
    if( (passLane[i].point.getY() * ballLocation.getY()) < 0.0f )
      passLane[i].score = -100.0f;
  }
  //-----------------------------------------------------------------
  //      End Remove passes on our side of the field if not screening
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  //      Now alter scores based on difference in distance from 
  //      ball->pass lane and the "optimal" distance that is preset
  //-----------------------------------------------------------------
  for(i=0; i<numberOfPassLanes; i++)
  {
    //If pass is still valid...
    if(passLane[i].score >= 0.0f)
    {    
      ballPassDistance = dist(passLane[i].point, ballLocation);
      difference = (float)(fabs(ballPassDistance - optimalDistance));
      passLane[i].score = passLane[i].score * difference;
    }
  }
  //-----------------------------------------------------------------
  //      End altering scores based on different in distance from 
  //      ball->pass lane and the "optimal" distance that is preset
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  //      Order passlanes by increasing score
  //-----------------------------------------------------------------
  for(i=0; i<numberOfPassLanes; i++)
  {
    for(j=0; j<(numberOfPassLanes-i-1); j++)
    {
      //if left is bigger and right is positive (negative would mean 
      //it is invalid so never swap past it)
      //or if the left is invalid (< 0.0f)
      if(
         (
          (passLane[j].score > passLane[j+1].score) &&
          (passLane[j+1].score > 0.0f)
         ) ||
         (passLane[j].score < 0.0f)
        )
      {
        sweetTemp = passLane[j];
        passLane[j] = passLane[j+1];
        passLane[j+1] = sweetTemp;
      }
    }
  }
  //-----------------------------------------------------------------
  //      End Order passlanes by increasing score
  //-----------------------------------------------------------------

  //-----------------------------------------------------------------
  //      Determine passlane
  //-----------------------------------------------------------------
  //READ: Orginally forward pass was going to be used on a greater scale.
  //However, I decided to simplify the algorithm to only be used for when
  //the goalie whiches to commit to a forward pass to a single player during
  //the play goalie ball. Since there will only be one player trying to get the ball
  //we simply pick the first sweet spot and use that.
  //
  //Originally this function also was used for screening. I decided it was best
  //to separate these two functions so that the code was cleaner and modifications
  //could occur on each without affecting the other.
  //
  //The overall structure, as of 12/7/2000 of forwardPass is VERY similar to that
  //from before. So it could be copied and slickly modified (sweet spots, determining at end)
  //to work for screening if desired.

  //First lane has smaller score and thus is best...
  passlane = &(passLane[0].point);

  //-----------------------------------------------------------------
  //      End Determine passlane
  //-----------------------------------------------------------------
}

