#include "action/action.h"
#include "info/robot/robot.h"
#include "info/ball/ball.h"
#include "info/ball/possession.h"
#include "math/angle.h"
#include "math/dist.h"
#include "intersect/intersect.h"
#include "robocup_strategy_data.h"
#include "info/ball/ball.h"
//------------------------------------------
//Static variables used in Strip_Ball

#define DIST_TO_DEST_THRESH .05f
#define ANGLE_TO_DEST_THRESH .1f
#define STOP_AVOIDING_OBSTACLES_DIST (params.general.PLAYER_RADIUS + params.general.OPPONENT_RADIUS + 2*params.general.BALL_RADIUS + .05f)
#define STRIP_ANGLE_ROTATION 3*PI/5

bool stripBall(const VisionData& field,
               const SystemParameters& params,
               RobotIndex thisRobot, // our robot's id
               RobotIndex enemyRobot, // enemy robot's id
               Destination* command) 
{

//  printf("Robot %d is stripping from opponent %d\n", thisRobot, enemyRobot);
//  fflush(stdout);

  command->setDribble(FAST_DRIBBLE);
  command->setVerticalDribble(FAST_V_DRIBBLE);

  Pair ballPos( getBallLocation(field) );
  float ballToOpponent,robotToOpponent;
  float desiredRotation;

  ballToOpponent = angleBetween( ballPos,getTheirRobotLocation(enemyRobot,field,params) );
  robotToOpponent = angleBetween( thisRobot,getTheirRobotLocation(enemyRobot,field,params), field, params );

  // if the ball is found, go get it facing the opponent
  if ( isRobocupBallFound(field) )
  {
    command->setPos( robotPositionGivenFront(ballPos,ballToOpponent,params) );
    command->setRotation(ballToOpponent);
  }
  // if the ball is lost, assume it is in a good (safe) position
  else
  {
    // if in defensive half, assume the ball is between the opponent and the our goal
    if ( ballPos.getX() < 0.0f )
    {
      ballToOpponent = angleBetween( Pair(params.field.OUR_GOAL_LINE,0.0f),getTheirRobotLocation(enemyRobot,field,params) );
      ballPos.setX( getTheirXLocation(enemyRobot,field,params) - params.general.OPPONENT_RADIUS*COS(ballToOpponent) );
      ballPos.setY( getTheirYLocation(enemyRobot,field,params) - params.general.OPPONENT_RADIUS*SIN(ballToOpponent) );
      command->setPos( robotPositionGivenFront(ballPos,ballToOpponent,params) );
      command->setRotation(ballToOpponent);
    }
    // if in offensive half, assume the ball is between our robot and the opponent robot
    else
    {
      ballToOpponent = robotToOpponent;
      ballPos.setX( getXLocation(thisRobot,field,params) + params.general.PLAYER_RADIUS*COS(ballToOpponent) );
      ballPos.setY( getYLocation(thisRobot,field,params) + params.general.PLAYER_RADIUS*SIN(ballToOpponent) );
      command->setPos( robotPositionGivenFront(ballPos,ballToOpponent,params) );
      command->setRotation(ballToOpponent);
    }
  }

  // if close to destination, then start turning to strip the ball
 //if ( command->getPos().distanceTo(getLocation(thisRobot,field,params)) < DIST_TO_DEST_THRESH )// &&
  if (friendlyHasPossession(thisRobot,params))
  {

    desiredRotation = robotToOpponent;

    // if on the left sideline, we want to turn towards the center of the field
    if (params.field.LEFT_SIDE_LINE - getYLocation(thisRobot,field,params) < 2*params.general.PLAYER_RADIUS)
    {
      if ( ABS(desiredRotation) < PI/2 ) {
        command->setRotation(desiredRotation - STRIP_ANGLE_ROTATION);
      }
      else
      {
        command->setRotation(desiredRotation + STRIP_ANGLE_ROTATION);
      }
    }
    else if ( getYLocation(thisRobot,field,params) - params.field.RIGHT_SIDE_LINE < 2*params.general.PLAYER_RADIUS)
    {
      if ( ABS(desiredRotation) < PI/2 ) {
        command->setRotation(desiredRotation + STRIP_ANGLE_ROTATION);
      }
      else
      {
        command->setRotation(desiredRotation - STRIP_ANGLE_ROTATION);
      }
    }
    // if we're in the middle of the field, just turn towards their goal
    else if ( desiredRotation > 0 )
    {
      command->setRotation(desiredRotation - STRIP_ANGLE_ROTATION);
    }
    else
    {
      command->setRotation(desiredRotation + STRIP_ANGLE_ROTATION);
    }
    // stay stationary until we're finished spinning
    command->setPos(getLocation(thisRobot,field,params));

    // after we've spun, move forward with the ball if we have it, or get out of the way if we don't
    //if ( ABS(normalizeAngle(getRotation(thisRobot,field,params) - rot2)) < ANGLE_TO_DEST_THRESH )
    if ( ABS(angleDifference(getRotation(thisRobot,field,params),command->getRotation())) < ANGLE_TO_DEST_THRESH )
    {
      command->setPos(frontOfRobot(thisRobot,field,params));
    }
  }

	//If we get too close, obstacle avoidance will move us away from the
	//enemy robot. Thus if the FRONT of our robot is under a threshhold distance from the enemy bot,
	//stop using obstical avoidence...


  if( (frontOfRobot(thisRobot,field,params).distanceTo( getTheirRobotLocation(enemyRobot,field,params) ) <= STOP_AVOIDING_OBSTACLES_DIST ) && 
      (ABS(angleDifference(ballToOpponent, robotToOpponent)) < PI/6) )
  {
		//do not avoid opponent
    command->setControl(OMNI_NO_AVOID);
		return false;
  }
  else {
		//we are not too close, so use obstacle avoidence
    command->setControl(OMNI_NORMAL);
		return true;
  }
}




//=========================================================================================================




