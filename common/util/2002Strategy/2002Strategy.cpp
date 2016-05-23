
#include "common/util/2002Strategy/2002Strategy.h"
#include "common/util/defense/defending.h"

bool isThreeManDefenseFormed(const VisionData &field, 
							               RobocupStrategyData &sd,
							               const SystemParameters &rp,
                             float outOfPositionDist)
{
  return !badRobot(DEFENDER,outOfPositionDist,field,sd,rp) &&
         !badRobot(SPECIAL_OP_DEFENDER,outOfPositionDist,field,sd,rp);
}
