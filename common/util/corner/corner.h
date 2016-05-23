/*
 *	corner.h
 *
 */

#ifndef CORNER_H
#define CORNER_H

#include "ai.h"
#include "VisionModule.h"
#include "../datatypes/robocup_strategy_data.h"
#include "../../positions/ScratchVars.h"
#include "../info/ball/ball.h"
//------------------------------------------
//This algorithm determines if the ball is in one of the corners
//of the field on the offensive side.
bool ballInCorner(const VisionData& field,
               RobocupStrategyData* sd);

bool ballStillInCorner(const VisionData& field,
                  const SystemParameters& params);

void getBallOutOfCorner(RobotIndex ID,const VisionData& field,
               Destination * command, RobocupStrategyData* sd,BasePlay* play);

bool inCornerManuever(RobocupStrategyData* sd);

void stripBallFromWall(BallStuckStatus stuckFlag, RobotIndex ID,const VisionData& field,
               Destination * command, RobocupStrategyData* sd);
//------------------------------------------

#endif CORNER_H