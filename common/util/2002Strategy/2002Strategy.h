
#ifndef STRATEGY_2002_H
#define STRATEGY_2002_H

#include "robocup_strategy_data.h"

bool isThreeManDefenseFormed(const VisionData &field, 
							               RobocupStrategyData &sd,
							               const SystemParameters &rp,
                             float outOfPositionDist=.2f);


#endif //STRATEGY_2002_H