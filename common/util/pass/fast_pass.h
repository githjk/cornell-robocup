#ifndef FAST_PASS_H
#define FAST_PASS_H

#include "robocup_strategy_data.h"
#include "obstacle.h"


//formerly Receive_Fast_Pass
void setupFastPass(const VisionData& field,
                   RobocupStrategyData* sd,
                   RobotIndex robotID,
                   Destination* command);


Pass findFastPassLane(const VisionData& field, 
					  const SystemParameters& params,
					  Pair goaliePosition,
					  Pair ballPosition,
					  Obstacles* incomingObstacles, 
				      float optimalPassDistance,
					  Pair assigningRobot
                     );

bool trapFastPass(const VisionData& field,
                  const RobocupStrategyData& sd,
                  RobotIndex robotID,
                  Destination* sendTo);


#endif