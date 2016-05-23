#ifndef DEFENDING_H
#define DEFENDING_H

#include "ai.h"
#include "robocup_strategy_data.h"
#include "common/datatypes/robocup_strategy_data.h"
#include "common/datatypes/SystemParameters.h"

//================================================================================
// Finds the "shadow" cast by one of our robots on our goal line.  It returns the
// left and right points of that shadow, such that y1<y2
void findShadow(Pair ball, Pair robot, const SystemParameters &params, float &y1, float &y2);

//================================================================================
// Determines if a robot is in the "Consideration Zone" for Tight Defense
bool isInConsiderationZone(Pair robot,RobocupStrategyData *rsd, const VisionData &vision, const SystemParameters &params);
bool isInConsiderationZone(const int team, RobotIndex robotID,RobocupStrategyData *rsd, const VisionData &vision, const SystemParameters &params);

//===============================================================================
//Finds the left and right endpoints of the shot on goal that op2 has.
//pl is the left endpoint, pr is the right endpoint
//   Originally written for LooseDSpecialOp.  Stolen so ShotBlock can us it, too.
bool findOpShot(RobotIndex op2Index, RobotIndex ignoreID,
                                       float &openShotLeft, float &openShotRight,
                                       const VisionData &field,
                                       const SystemParameters &rp) ;


//================================================================================
//Calculates whether the ball is heading towards our goal at the specified speed
bool towardGoal(MovingObject ball, float goal_line, float leftbound, float rightbound,float ballspeed);

//================================================================================
//Calculates whether something is heading towards our goal
bool towardGoal(Pair thing, float angle, float goal_line, float leftbound, float rightbound);

//================================================================================
//finds out if the defender is gone or out of position
bool badRobot(Position position,float position_dist, 
                 const VisionData &field, 
                 RobocupStrategyData &sd,
                 const SystemParameters &rp);
//================================================================================

#endif DEFENDING_H