#include "LooseDefenseStorm.h"
#include "skillz/TandemDefenderSkill.h"
#include "common/util/info/ball/ball.h"
#include "common/util/math/perpendicular.h"


//========================================
LooseDefenseStorm::LooseDefenseStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList0[] = {BLOCKER};
  clouds[0] = new Cloud(pList0, 1);

  Position pList1[] = {AGGRESSOR, 
                       DEFENDER,
                       SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR};  
  clouds[1] = new Cloud(pList1, 5);

  Position pList2[] = {CREATOR};
  clouds[2] = new Cloud(pList2, 1);
}
//========================================
void LooseDefenseStorm::preformSwitching(VisionModule* vm,
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
    //Get Defender's Location
    RobotIndex DefenderID = sd->getRobotByPosition(DEFENDER);
    Pair defenderLocation = getLocation(DefenderID,
                                        *vision,
                                        *params);                                                                                  
    //-----
    //Get Special Op Defender's Location
    RobotIndex SpecialOpDefenderID = sd->getRobotByPosition(SPECIAL_OP_DEFENDER);
    Pair specialOpDefenderLocation = getLocation(SpecialOpDefenderID,
                                                 *vision,
                                                 *params);                                                                                  
    //-----
    //Get Ball's Location
    Pair ball = getBallLocation(*vision);
    //-----
    float historisisFactor = params->general.PLAYER_RADIUS;

    //====================================
    //======= Conditional Switches =======
    //===================================

    //-----------------------------------------------------------    
    //ensure the special op is a SPECIAL_OP_DEFENDER
    if(sd->getRobotByPosition(SPECIAL_OP_CREATOR) != NO_ROBOT)
      switchPositions(SPECIAL_OP_CREATOR, SPECIAL_OP_DEFENDER);
    if(sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR) != NO_ROBOT)
      switchPositions(SPECIAL_OP_AGGRESSOR, SPECIAL_OP_DEFENDER);
    //-----------------------------------------------------------
    RobotIndex theirID;
    //if they have possession
    if(theyHavePossession(*vm->getCurrentFrame(),
                          *params,
                          &theirID))
    {
      if(
          SpecialOpDefenderID != NO_ROBOT &&
          AggressorID != NO_ROBOT
        )
      {
        Pair opLoc = getLocation(params->general.OTHER_TEAM,
                                 theirID,
                                 *vm->getCurrentFrame());
        Pair ballLoc = getBallLocation(*vm->getCurrentFrame());
        //if aggressor behind opponent and special op not, then swap
        float aggressorOffset = pOffSet(opLoc,
                                        ballLoc,
                                        aggressorLocation);
        float specialOpOffset = pOffSet(opLoc,
                                        ballLoc,
                                        specialOpDefenderLocation);
        if(aggressorOffset < 0.0f &&
           specialOpOffset > 0.0f)
        {
          switchPositions(AGGRESSOR, SPECIAL_OP_DEFENDER);
        }
      }
    }
    //else, if special op closer to ball than aggressor, swap
    else if(
             SpecialOpDefenderID != NO_ROBOT && 
             AggressorID != NO_ROBOT &&
             specialOpDefenderLocation.distanceTo(ball) < 
             aggressorLocation.distanceTo(ball) - historisisFactor
           )
    {
      switchPositions(AGGRESSOR, SPECIAL_OP_DEFENDER);
    }
    //----------------------------------------------------------------
    //If special op defender is closer to the defender's 
    //destination than the defender is switch
    //or If the special op is outside of the defender's dest, but really close to him, swap them out.
   if(DefenderID != NO_ROBOT &&
       SpecialOpDefenderID != NO_ROBOT && 
       (
         (
           sd->getDestination(DefenderID)->getPos().distanceTo(specialOpDefenderLocation) <
           sd->getDestination(DefenderID)->getPos().distanceTo(defenderLocation) -4.0f*historisisFactor  
         ) ||
         (
          (
           (specialOpDefenderLocation.getY() > defenderLocation.getY() && ball.getY() > params->field.SPLIT_LINE) ||
           (specialOpDefenderLocation.getY() < defenderLocation.getY() && ball.getY() < params->field.SPLIT_LINE)
          ) &&
          specialOpDefenderLocation.distanceTo(sd->getDestination(DefenderID)->getPos()) < 3.0f*params->general.PLAYER_RADIUS
         )
       )
      )
       switchPositions(DEFENDER, SPECIAL_OP_DEFENDER);
    //----------------------------------------------------------------

    //----------------------------------------------------------------

}
//========================================
