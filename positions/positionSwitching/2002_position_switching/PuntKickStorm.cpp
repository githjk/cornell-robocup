#include "PuntKickStorm.h"
#include "common/util/info/ball/ball.h"

//========================================
PuntKickStorm::PuntKickStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList0[] = {BLOCKER};
  clouds[0] = new Cloud(pList0, 1);

  Position pList1[] = {DEFENDER};
  clouds[1] = new Cloud(pList1, 1);

  Position pList2[] = {AGGRESSOR, 
                       CREATOR,
                       SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR};  
  clouds[2] = new Cloud(pList2, 5);

}
//========================================
void PuntKickStorm::preformSwitching(VisionModule* vm,
                                        SystemParameters* params)
{
}
//========================================
