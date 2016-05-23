#include "TouchShootStorm.h"
#include "common/util/info/ball/ball.h"

//========================================
TouchShootStorm::TouchShootStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList0[] = {BLOCKER};
  clouds[0] = new Cloud(pList0, 1);

  Position pList1[] = {AGGRESSOR};
  clouds[1] = new Cloud(pList1, 1);
  

  Position pList2[] = {SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR};  
  clouds[2] = new Cloud(pList2, 3);

  Position pList3[] = {CREATOR};
  clouds[3] = new Cloud(pList3, 1);

  Position pList4[] = {DEFENDER};
  clouds[4] = new Cloud(pList4, 1);
}
//========================================
void TouchShootStorm::preformSwitching(VisionModule* vm,
                                        SystemParameters* params)
{
    //-----
    //Get latest strategy data and vision data required for tests
    RobocupStrategyData* sd = strategy->getCurrentRoboCupFrame();
    VisionData* vision = vm->getCurrentFrame();
    //-----
    //Get Aggressor's Location
    RobotIndex AggressorID = sd->getRobotByPosition(AGGRESSOR);
    Pair aggressorLocation = getLocation(AggressorID,
                                         *vision,
                                         *params);                                                                                  
    //-----
    //Get Creator's Location
    RobotIndex CreatorID = sd->getRobotByPosition(CREATOR);
    Pair creatorLocation = getLocation(CreatorID,
                                         *vision,
                                         *params);                                                                                  
    //-----
    //Get Special Op Creator's Location
    RobotIndex SpecialOpCreatorID = sd->getRobotByPosition(SPECIAL_OP_CREATOR);
    Pair specialOpCreatorLocation = getLocation(SpecialOpCreatorID,
                                                *vision,
                                                *params);                                                                                  
    //-----
    //Get Ball's Location
    Pair ball = getBallLocation(*vision);
    //-----
    float historisisFactor = params->general.PLAYER_RADIUS;


    //====================================
    //======= Conditional Switches =======
    //====================================

    //----------------------------------------------------------------
    //ensure the special op is a SPECIAL_OP_AGGRESSOR
    if(sd->getRobotByPosition(SPECIAL_OP_CREATOR) != NO_ROBOT)
      switchPositions(SPECIAL_OP_CREATOR, SPECIAL_OP_AGGRESSOR);
    if(sd->getRobotByPosition(SPECIAL_OP_DEFENDER) != NO_ROBOT)
      switchPositions(SPECIAL_OP_DEFENDER, SPECIAL_OP_AGGRESSOR);
    //----------------------------------------------------------------

}
//========================================
