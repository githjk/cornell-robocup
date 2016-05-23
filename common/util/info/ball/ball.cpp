/*
 *	ball.cpp
 */

#include "ball.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "static_vars.h"
#include "common/util/math/angle.h"

//returns a boolean, true if point is off the field, false otherwise
bool offField(const SystemParameters& params, const Pair &Point, const float &wallPushout)
{
	return ((Point.getX() < params.field.OUR_GOAL_LINE + wallPushout)
			||  (Point.getX() > params.field.THEIR_GOAL_LINE - wallPushout)
			||  (Point.getY() > params.field.LEFT_SIDE_LINE - wallPushout)
			||  (Point.getY() < params.field.RIGHT_SIDE_LINE + wallPushout));
}

// The following method assumes that the point is on field (x-coord of the
// point is within our goal-line and their goal-line)
bool insideTheirGoalieBox (const SystemParameters & params, const Pair &point)
{
  return ((point.getX() >= params.field.THEIR_GOALIE_BOX) 
      &&  (point.getY() <= params.field.LEFT_GOALIE_BOX) 
      &&  (point.getY() >= params.field.RIGHT_GOALIE_BOX));
}

// The following method assumes that the point is on field (x-coord of the
// point is within our goal-line and their goal-line)
bool insideOurGoalieBox (const SystemParameters & params, const Pair &point)
{
  return ((point.getX() <= params.field.OUR_GOALIE_BOX) 
      &&  (point.getY() <= params.field.LEFT_GOALIE_BOX) 
      &&  (point.getY() >= params.field.RIGHT_GOALIE_BOX));
}

//=========================================================================
//Ball Stuck - returns non zero value if the ball is smack against the wall
//
//BallStuckStatus:
//  NOT_STUCK = 0,
//  STUCK_ON_LEFT = 1,
//  STUCK_ON_RIGHT = 2,
//  STUCK_ON_THEIR_WALL = 3,
//  STUCK_ON_OUR_WALL = 4
BallStuckStatus ballStuck(const VisionData& field,
    		                  const SystemParameters& params)
{
	//
	//       Enemy Goal
	//         _____
	//    ____|     |____
	//   |       3       |
	//   |               |
	//   |               |
	//   |               |
	//   |               |
	//   |1             2|
	//   |               |
	//   |               |
	//   |               |
	//   |               |
	//   |               |
	//   |____   4   ____|
	//        |_____|
	//   
	//        Our Goal
	//
	//If ball is within this distance from any side, it is
	//considered stuck to that side
	const float SIDE_TOLERANCE = 0.7f*params.general.PLAYER_RADIUS;

	//----Not used----
	//If ball is within this distance from any TWO walls,
	//it is considered to be stuck in a corner
	//#define Corner_Tolerance (1.9f*sd.p.PLAYER_RADIUS)
	//#define Corner_Tolerance (0.7f*sd.p.PLAYER_RADIUS)
	//----------------

	float ballX = ballXLocation(field);
	float ballY = ballYLocation(field);

	//-----------------------
	//Check corners
	//-----------------------
	//Their Wall/Corner
  if(
     (ABS(params.field.THEIR_GOAL_LINE - ballX) < SIDE_TOLERANCE) &&
	   (
	    (ballY > params.field.LEFT_GOALIE_BOX) ||
	    (ballY < params.field.RIGHT_GOALIE_BOX)
	   )
    )
  {
		return STUCK_ON_THEIR_WALL; // == 3
  }
  if(
     (ABS(params.field.OUR_GOAL_LINE - ballX) < SIDE_TOLERANCE) &&
	   (
	    (ballY > params.field.LEFT_GOALIE_BOX) ||
	    (ballY < params.field.RIGHT_GOALIE_BOX)
	   )
    )
  {
		return STUCK_ON_OUR_WALL; // == 4
  }

	//-----------------------
	//Now check walls
	//-----------------------
	//Left Wall
	if((float)(fabs(params.field.LEFT_SIDE_LINE - ballY))
							< SIDE_TOLERANCE)
  {
		return STUCK_ON_LEFT; // == 1
  }
	//Right Wall
	if((float)(fabs(params.field.RIGHT_SIDE_LINE - ballY))
						< SIDE_TOLERANCE)
  {
		return STUCK_ON_RIGHT; // == 2
  }

	//-----------------------
	//Else, not stuck
	//-----------------------
	return NOT_STUCK;
}

//=====================================================================
//Find the shortest enemy->ball distance
float findShortestRobotDistance(const VisionData& field,
                                const int team,
                                RobotIndex* robotID)
{
	//Give a larger than will ever exist value
	float shortestRobotDistance = 9999999.9f;
	float newDistance;
  if(robotID != NULL)
    (*robotID) = NO_ROBOT;

	//Check all distances, at least will be smaller =-)
	for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
	{
		//Check to make sure robot is valid
    if(field.isRobotFound(team, i))
    {
      //find how far this robot is to the ball
			newDistance = getRobocupBall(field).getPos().distanceTo(getXLocation(team, i, field),
                                                              getYLocation(team, i, field));
			if(newDistance < shortestRobotDistance)
      {
        shortestRobotDistance = newDistance;
        //if user wants the id of the closest robot, set it. 
        if(robotID != NULL)
          (*robotID) = i;
      }
		}
	}	
	return shortestRobotDistance;
}



//Find the robot->ball distance
float distanceToBall(const VisionData& field,
                     const int team,
                     const RobotIndex robotID)
{
  return getRobocupBall(field).getPos().distanceTo(getXLocation(team, robotID, field),
                                                   getYLocation(team, robotID, field));
}



//=====================================================================
//Returns true if an enemy bot is closer than the
//Longest distance from the ball
bool enemyHasBall(const VisionData& field, 
                  const SystemParameters& params)
{
  //Threshhold - below this distance we consider enemy to have the ball
  const float THRESHOLD = 2*params.general.OPPONENT_RADIUS;

  return (findShortestRobotDistance(field, params.general.OTHER_TEAM) < THRESHOLD);
}

//Determines ball is between a point and a target in 
// a path of width buf.  
//If it isn't, return false, else return true
bool isBallBetween(const Pair &robotLoc, // location of the robot
		   const Pair &point, // locatin of the point
		   float buf,        // the width of the lane
		   const VisionData &field // where everyone else is now
)
{
  xBot = robotLoc.getX();
  yBot = robotLoc.getY();
  xHold = point.getX();
  yHold = point.getY();
  
  float buffer = buf/2;
  interceptBuffer = buffer;
  
  if(xHold != xBot){
    m = (yHold - yBot)/(xHold - xBot);
    b = yBot - xBot*m;
    if(m != 0)
      interceptBuffer = (float(fabs(buffer / sin(atan(fabs(1/m))))));
  }
  //Case that robots have the same x value.  We set the variables so that we
  //check a lane from passer to receiver of width 2*buffer
  else{
    m=0;
    b= float( 0.5f*(yBot + yHold));
    interceptBuffer = float( .5* fabs(yBot - yHold));
    xBot = xBot - buffer;
    xHold = xHold + buffer;
  }
  
  xOther = getRobocupBall(field).getPos().getX();
  yOther = getRobocupBall(field).getPos().getY();

  yValue = m*xOther + b;

  //Check to see if any of our friends are in the way
  //Hold the closest obstacle in (xHold, yHold)
  if ((yOther < yValue + interceptBuffer) &&
      (yOther > yValue - interceptBuffer) &&
      ((fabs(m) <= 1 &&
	((xOther <= xBot && xOther >= xHold) ||
	 (xOther >= xBot && xOther <= xHold)))
       ||
       (fabs(m) >= 1 &&
	((yOther <= yBot && yOther >= yHold) ||
	 (yOther >= yBot && yOther <= yHold)))
       )
      )
  {
    return true;
  }
  return false;
}


//===================================================================================


//Returns the number of (enemy) obstacles within a path from the robot of position to ball
int numObstaclesToBall (RobotIndex ID,
						const VisionData &field,
						const SystemParameters &p)
{
  int obstacles = 0;

  float xBot = getXLocation(ID, field, p);
  float yBot = getYLocation(ID, field, p);

  float xBall = getRobocupBall(field).getXPos();
  float yBall = getRobocupBall(field).getYPos();

  float buffer = p.general.PLAYER_RADIUS + p.general.OPPONENT_RADIUS;

  float m, b;
  if(xBall != xBot) {
    m = (yBall - yBot)/(xBall - xBot);
    b = yBot - xBot*m;
  }
  //Case that robots have the same x value.  We set the variables so that we
  //check a lane from passer to receiver of width 2*buffer
  else {
    m = 999999.0f;
    b = float( .5*(yBot + yBall));
  }
  float xLowerBound = MIN(xBot, xBall);
  float xUpperBound = MAX(xBot, xBall);
  Pair other;

  //Check to make sure no robots are in the way (excluding passer and receiver)
  for(RobotIndex i=ROBOT0; i < NUM_ROBOTS; i++)
  {
    //If enemy robot is found, check to see if it is in the way
    if(theirRobotFound(i, field, p))
    {
      other = field.getRobot(p.general.OTHER_TEAM, i).getPos();

      //Check to see if any of our enemies are in the way
      if ((xLowerBound < other.getX())  //Use strict inequality (rather than <=)
        &&(xUpperBound > other.getX())  // to ensure that robotLoc itself isn't considered an obstacle
        &&(distFromPointToLine(m, b, other) < buffer))
      {
	      obstacles++;   
      }
    }
  }

  return obstacles;
}
//------------------------------------------------------------------
//check to make sure an opponent isn't in your face
bool kickSafe(int robotID, 
      			  VisionData &field, 
			        SystemParameters &sp,
              RobocupStrategyData &sd,
       			  float safeDistance,
      			  float safeAngle)
{
  //default value 
  if(safeDistance < 0.0f)
  {
    safeDistance = sp.general.PLAYER_RADIUS + 
                   sp.general.OPPONENT_RADIUS +
                   0.5f;

  }

  //default value 
  if(safeAngle < 0.0f)
  {
    safeAngle = 0.7f;
  }

  Pair robotLoc = getLocation(sp.general.TEAM, (RobotIndex)robotID, field);
  //use defensive ball, in case it's lost and on opponent
  Pair ballLoc = sd.getDefensiveBallLocation();

  Pair opLoc;
  //check over every opponent
  for(int i=0; i<NUM_ROBOTS; i++)
  {
    if(robotFound(sp.general.OTHER_TEAM, (RobotIndex)i, field, sp))
    {
	  	opLoc = getLocation(sp.general.OTHER_TEAM, (RobotIndex)i, field);
		  if(
		      (
            robotLoc.distanceTo(opLoc) < safeDistance &&
		        ABS(
                 angleDifference(
                                  angleBetween(robotLoc, ballLoc),
                                  angleBetween(robotLoc, opLoc)
                                )
               ) < safeAngle 
          ) ||
          (
            robotLoc.distanceTo(opLoc) < 2.0f*sp.general.PLAYER_RADIUS + 
                                         sp.general.OPPONENT_RADIUS
                                         
          )
		    )
      {
        return false;
      }                       
    }
  }

  return true;
}
//------------------------------------------------------------------
