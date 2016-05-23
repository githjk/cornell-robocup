#include "DoublePickOffenseStorm.h"

//========================================
DoublePickOffenseStorm::DoublePickOffenseStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList0[] = {BLOCKER};
  clouds[0] = new Cloud(pList0, 1);

  Position pList1[] = {DEFENDER};
  clouds[1] = new Cloud(pList1, 1);

  Position pList2[] = {AGGRESSOR};
  clouds[2] = new Cloud(pList2, 1);

  Position pList3[] = {CREATOR,
                       SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR};  
  clouds[3] = new Cloud(pList3, 5);

}
//========================================
void DoublePickOffenseStorm::preformSwitching(VisionModule* vm,
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
    Pair ball = sd->getOffensiveBallLocation();
    //-----
    float historisisFactor = params->general.PLAYER_RADIUS;

    //====================================
    //======= Conditional Switches =======
    //====================================

    //----------------------------------------------------------------
    //ensure the special op is a SPECIAL_OP_CREATOR
    if(sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR) != NO_ROBOT)
      switchPositions(SPECIAL_OP_AGGRESSOR, SPECIAL_OP_CREATOR);
    if(sd->getRobotByPosition(SPECIAL_OP_DEFENDER) != NO_ROBOT)
      switchPositions(SPECIAL_OP_DEFENDER, SPECIAL_OP_CREATOR);
    //----------------------------------------------------------------
    //make sure creator is always in front of the special op
    if(CreatorID != NO_ROBOT &&
       SpecialOpCreatorID != NO_ROBOT &&
       creatorLocation.getX() < specialOpCreatorLocation.getX() - historisisFactor)
       switchPositions(SPECIAL_OP_CREATOR, CREATOR);
    //----------------------------------------------------------------
}
//========================================
