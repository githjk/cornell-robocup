#ifndef BACK_PASS_H
#define BACK_PASS_H

#include "VisionModule.h"
#include "robocup_strategy_data.h"

bool trapBackPass(const VisionData& field,
                  const RobocupStrategyData& sd,
                  //robots Position
                  RobotIndex robotID,
                  Destination* sendTo);

void evaluateBackPass(const VisionData& field, 
                      const RobocupStrategyData& GS,
                      Pass* passData);

#endif BACK_PASS_H