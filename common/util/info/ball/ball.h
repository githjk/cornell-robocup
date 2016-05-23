/*
 *	ball.h
 *
 */

#ifndef UTIL_BALL_H
#define UTIL_BALL_H

//#include <stdio.h>
#include <math.h>

#include "ai.h"
#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "common/datatypes/SystemParameters.h"

/*****************************Ball Data Access functions********************************/

// Returns Ball location
__inline Pair getBallLocation(const VisionData& field){
	return field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos();
}

// Returns Ball
__inline MovingObject getRobocupBall(const VisionData& field){
	return field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX);
}

__inline bool isRobocupBallFound(const VisionData& field){
	return field.isBallFound(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX);
}

// Returns Ball X Location
__inline float ballXLocation(const VisionData& field){
	return float(field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos().getX());
}


//Returns Ball Y Location
__inline float ballYLocation(const VisionData& field){
	return float(field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos().getY());
}

// Returns Ball velocity
__inline Pair ballVel(const VisionData& field){
	return field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getVel();
}

// Returns Ball X velocity
__inline float ballXVel(const VisionData& field){
	return float(field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getVel().getX());}

//Returns Ball Y velocity
__inline float ballYVel(const VisionData& field){
	return float(field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getVel().getY());
}

//Returns the ball's speed (a scalar)
__inline float getBallSpeed(const VisionData& field) {
  return SQRT( ( ballXVel(field) * ballXVel(field) ) + ( ballYVel(field) * ballYVel(field) ) );
}

bool offField(const SystemParameters& params, const Pair &Point, const float & wallPushout = 0.0f);
bool insideTheirGoalieBox (const SystemParameters & params, const Pair &point);
bool insideOurGoalieBox (const SystemParameters & params, const Pair &point);
//Ball Stuck - returns non zero value if the ball is smack against the wall
enum BallStuckStatus {
  NOT_STUCK = 0,
  STUCK_ON_LEFT = 1,
  STUCK_ON_RIGHT = 2,
  STUCK_ON_THEIR_WALL = 3,
  STUCK_ON_OUR_WALL = 4
};
BallStuckStatus ballStuck(const VisionData& field, 
	                        const SystemParameters& params);


//Find the shortest robot->ball distance
float findShortestRobotDistance(const VisionData& field,
                                const int team,
                                RobotIndex* robotID=NULL);

//Find the robot->ball distance
float distanceToBall(const VisionData& field,
                     const int team,
                     const RobotIndex robotID);

//Returns true if an enemy bot is closer than the
//longest distance from the ball
bool enemyHasBall(const VisionData& field, 
                  const SystemParameters& params);


// Determines ball is between a point and a target in
// a path of width buf.
// If it isn't, return false, else return true
bool isBallBetween(
                   const Pair &robotLoc, // location of the robot
                   const Pair &point, // locatin of the point
                   float buf,        // the width of the lane
                   const VisionData &field // where everyone else is now
);

//Returns the number of (enemy) obstacles within a path from the robot of position to ball
int numObstaclesToBall (RobotIndex ID,
						const VisionData &field,
						const SystemParameters &p);

//check to make sure an opponent isn't in your face
bool kickSafe(int robotID, 
      			  VisionData &field, 
			        SystemParameters &sp,
              RobocupStrategyData &sd,
       			  float safeDistance = -1.0f,
      			  float safeAngle = -1.0f);


#endif // UTIL_BALL_H

