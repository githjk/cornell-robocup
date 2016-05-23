/*
*   basic_actions.h
*/

#ifndef BASIC_ACTION_H
#define BASIC_ACTION_H

#include "robocup_strategy_data.h"
#include "VisionModule.h"

void stopRobot(RobotIndex ID, BasePlay* play, 
	              const VisionData& field, RobocupStrategyData* sd);
/*
void findDefensivePosition(RobotIndex ID, BasePlay* play, 
	              const VisionData& field, RobocupStrategyData* sd);

void findOffensivePosition(RobotIndex ID, BasePlay* play, 
	              const VisionData& field, RobocupStrategyData* sd);

void offensiveAssignment(const VisionData& field, RobocupStrategyData* sd);
void midfieldAssignment(const VisionData& field, RobocupStrategyData* sd);
void defensiveAssignment(const VisionData& field, RobocupStrategyData* sd);

void assignOnXFirstGetBall(const VisionData& field, RobocupStrategyData* sd, Position roles[4]);

void checkOffensivePrerequisites(BasePlay* play, RobotIndex activeID,
                  VisionData& field, RobocupStrategyData* sd);

Play getOffensivePlay(const VisionData& field, RobocupStrategyData* sd);
*/

#endif //BASIC_ACTION_H