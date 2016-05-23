/*
 *	action.h
 *
 */

#ifndef ACTION_ACTION_H
#define ACTION_ACTION_H

#include "ai.h"
#include "VisionModule.h"
#include "../static_vars.h"
#include "robocup_strategy_data.h"
//-----------------------------------------------------------------------------------------------------
//This algorithm sends a robot to go strip the ball =-)
//Prototyped and tested on Mark's TI-86 lol!
bool stripBall(const VisionData& field,
               const SystemParameters& params,
               RobotIndex thisRobot, // our robot's id
               RobotIndex enemyRobot, // enemy robot's id
               Destination* command); //so we can turn the dribbler on.

//-----------------------------------------------------------------------------------------------------


#endif ACTION_ACTION_H

