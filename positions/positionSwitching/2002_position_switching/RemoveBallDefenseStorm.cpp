#include "RemoveBallDefenseStorm.h"
#include "common/util/info/ball/ball.h"

//========================================
RemoveBallDefenseStorm::RemoveBallDefenseStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList1[] = {BLOCKER};
  clouds[0] = new Cloud(pList1, 1);

  Position pList2[] = {DEFENDER, 
                       SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR,
                       AGGRESSOR, 
                       CREATOR};
  clouds[1] = new Cloud(pList2, 6);
}
//========================================
void RemoveBallDefenseStorm::preformSwitching(VisionModule* vm,
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
    //Get Special Op Defender's Location
    RobotIndex SpecialOpDefenderID = sd->getRobotByPosition(SPECIAL_OP_DEFENDER);
    Pair specialOpDefenderLocation = getLocation(SpecialOpDefenderID,
                                                 *vision,
                                                 *params);                                                                                  
    //-----
    //Get Defender's Location
    RobotIndex DefenderID = sd->getRobotByPosition(DEFENDER);
    Pair defenderLocation = getLocation(DefenderID,
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
    //ensure the special op is a SPECIAL_OP_DEFENDER
    if(sd->getRobotByPosition(SPECIAL_OP_CREATOR) != NO_ROBOT)
      switchPositions(SPECIAL_OP_CREATOR, SPECIAL_OP_DEFENDER);
    if(sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR) != NO_ROBOT)
      switchPositions(SPECIAL_OP_AGGRESSOR, SPECIAL_OP_DEFENDER);
    //----------------------------------------------------------------
   /* //keep defender on opposite side of the field as the ball
    if(
        DefenderID != NO_ROBOT && 
        SpecialOpDefenderID != NO_ROBOT &&
        (ball.getY() > (params->field.SPLIT_LINE + historisisFactor) &&
          defenderLocation.getY() > (specialOpDefenderLocation.getY() + historisisFactor)) ||
        (ball.getY() < (params->field.SPLIT_LINE - historisisFactor) &&
          defenderLocation.getY() < (specialOpDefenderLocation.getY() - historisisFactor))
      )
      switchPositions(SPECIAL_OP_DEFENDER, DEFENDER);
    //----------------------------------------------------------------
    //keep keep aggressor on same side of the field as the ball
    if(
        AggressorID != NO_ROBOT && 
        CreatorID != NO_ROBOT &&
        (ball.getY() > (params->field.SPLIT_LINE + historisisFactor) &&
          aggressorLocation.getY() < (creatorLocation.getY() - historisisFactor)) ||
        (ball.getY() < (params->field.SPLIT_LINE - historisisFactor) &&
          aggressorLocation.getY() > (creatorLocation.getY() + historisisFactor))
      )
      switchPositions(AGGRESSOR, CREATOR);
    //----------------------------------------------------------------
    //if the special op is farther upfield than aggressor, then switch
    if(
        AggressorID != NO_ROBOT && 
        SpecialOpDefenderID != NO_ROBOT &&
        specialOpDefenderLocation.getX() > aggressorLocation.getX() + historisisFactor
      )
      switchPositions(AGGRESSOR, SPECIAL_OP_DEFENDER);
    //----------------------------------------------------------------
    
    //if the defender is farther upfield than creator then switch
    if(
        DefenderID != NO_ROBOT && 
        CreatorID != NO_ROBOT &&
        defenderLocation.getX() > creatorLocation.getX() + historisisFactor
      )
      switchPositions(DEFENDER, CREATOR);
    //----------------------------------------------------------------
    */
    //----------------------------------------------------------------
    //minimize distances special op aggressor and creator have to travel
    if(SpecialOpDefenderID != NO_ROBOT &&
       AggressorID != NO_ROBOT)
    {
       float dist1 = aggressorLocation.squareDistanceTo(sd->getDestination(AggressorID)->getPos()) +
                     specialOpDefenderLocation.squareDistanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
       float dist2 = specialOpDefenderLocation.squareDistanceTo(sd->getDestination(AggressorID)->getPos()) +
                     aggressorLocation.squareDistanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
       if(dist2 < dist1 - historisisFactor*historisisFactor)
       {
         switchPositions(AGGRESSOR, SPECIAL_OP_DEFENDER);
       }
    }
    //----------------------------------------------------------------

}
//========================================
