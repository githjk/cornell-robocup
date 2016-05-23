/*
 *	POSSESSION.H
 *
 */

#ifndef UTIL_INFO_BALL_POSSESSION_H
#define UTIL_INFO_BALL_POSSESSION_H

#include "ai.h"
#include "../datatypes/robocup_strategy_data.h"
#include "math/cmath.h"
#include "info/robot/robot.h"
#include "common/datatypes/SystemParameters.h"

//*******POSSESSION FUNCTIONS*********
// Returns true if the specified robot has possession of the ball (by checking the counter...)
bool friendlyHasPossession(RobotIndex index, 
                           const SystemParameters& p); // ID of the travelling robot

//smarter algorithm, if the ball is lost uses offensive and defensive balls
bool friendlyHasPossession(RobotIndex index, 
                           const SystemParameters& p,
                           const VisionData& field,
                           RobocupStrategyData& rsd,
                           bool useOffensiveBallWhenLost);

//determines if any of our robots have the ball. uses offensive ball if ball is lost
bool weHavePossession(VisionData& field,  // This is where everyone else is now
                      SystemParameters& p,	  // This contains TEAM, FRAME_RATE, wall locations, etc
                      RobocupStrategyData& rsd, //to get access to offensive ball data
                      RobotIndex * bot);      

//Returns true if an enemy robot has possession of the ball
bool opponentHasPossession(RobotIndex index, 
                           const SystemParameters& p);


//Returns true if any enemy robot has possession of the ball
bool theyHavePossession(const VisionData& field, 
                        const SystemParameters& params,
                        RobotIndex *bot);

//Returns true if the ball is within the radius of our robot at the specified position
bool ballWithinRadius(RobotIndex index, const VisionData& field, const SystemParameters& params);
//===================================================================================
//The following functions update if robots have possession
//checks if all of our robots have possession of the ball
void updateRobotsPossession(const RawVisionData& field, 
                            const SystemParameters& p,
                            StrategyData* sd);

//checks specific robot for possession, and updates or resets the possession counter...
void updateFriendlyHasPossession(
    RobotIndex index,         // ID of the travelling robot
    const RawVisionData& field,  // This is where everyone else is now
    const SystemParameters& p);      // This contains TEAM, FRAME_RATE, wall locations, etc

//checks specific robot for possession, and updates or resets the possession counter...
void updateOpponentPossession(
    RobotIndex index,         // ID of the travelling robot
    const RawVisionData& field,  // This is where everyone else is now
    const SystemParameters& p      // This contains TEAM, FRAME_RATE, wall locations, etc
);

//Resets possession counters...
void resetPossessionCounters();
//-------------------------------------------------
//Returns the coordinates of the front of the robot (at a specified point and rot)
Pair frontOfRobot(Pair robotLoc,
                  float robotRot,
                  const SystemParameters& params);

//Places the coordinates of the front of the robot with position in (x, y)
Pair frontOfRobot(RobotIndex index, 
                  const VisionData& field, 
                  const SystemParameters& params);

//Returns the coordinates of a point a certain distance in the front of the robot
Pair frontOfRobot(RobotIndex index, 
                  const VisionData& field, 
                  const SystemParameters& params,
                  float tolerance);             // extra distance in front of the robot

//Given the front of the robot and the angle at which the robot should be facing
// returns the position the robot should be in. Used for receiving passes.
Pair robotPositionGivenFront(const Pair& frontOfBot,      //position of the front of the robot
                             float thetaBot,              //orientation
                             const SystemParameters& params);

//Given a point a distance 'tolerance' in front of the robot and the angle at which the
// robot should be facing returns the position the robot should be in. Used for receiving passes.
Pair robotPositionGivenFront(const Pair& frontOfBot,float thetaBot,
                             const SystemParameters& params,float tolerance);

#endif //UTIL_INFO_BALL_POSSESSION_H
