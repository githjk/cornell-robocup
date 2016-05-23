#include "CageDefenseStorm.h"
#include "common/util/info/ball/ball.h"

//========================================
CageDefenseStorm::CageDefenseStorm(RoboCupStrategyModule* strategy) : 
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
void CageDefenseStorm::preformSwitching(VisionModule* vm,
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
        //====================================

        //----------------------------------------------------------------
        //ensure the special op is a SPECIAL_OP_DEFENDER
        if(sd->getRobotByPosition(SPECIAL_OP_CREATOR) != NO_ROBOT)
          switchPositions(SPECIAL_OP_CREATOR, SPECIAL_OP_DEFENDER);
        if(sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR) != NO_ROBOT)
          switchPositions(SPECIAL_OP_AGGRESSOR, SPECIAL_OP_DEFENDER);
        //----------------------------------------------------------------
        /*//if special op defender exists and is  aggressor, switch
        if(//both positions exist
           SpecialOpDefenderID != NO_ROBOT &&
           AggressorID != NO_ROBOT &&
           //special op behind aggressor
           specialOpDefenderLocation.getX() < 
           (aggressorLocation.getX() - historisisFactor) &&
           //both positions on same side (width wise) of the field as the ball
           //(so they won't run through it and knock it when switching)
           (
             //both on the left
             (aggressorLocation.getY() > ball.getY() &&
              specialOpDefenderLocation.getY() > ball.getY()) ||
             //both on the right
             (aggressorLocation.getY() < ball.getY() &&
              specialOpDefenderLocation.getY() < ball.getY())
           ))
           switchPositions(AGGRESSOR, SPECIAL_OP_DEFENDER);
           */
        //----------------------------------------------------------------
        //If defender is on outside edge swap with aggressor if they
        //they are on opposite sides of the ball (so they don't both run through the ball)
        //we don't always swap because if one guy is close to being in position he protects the ball,
        //and likely will hit the ball if he did get moved over to the other side
        if(SpecialOpDefenderID != NO_ROBOT &&
           DefenderID != NO_ROBOT)
        {
           float dist1 = defenderLocation.squareDistanceTo(sd->getDestination(DefenderID)->getPos()) +
                         specialOpDefenderLocation.squareDistanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
           float dist2 = specialOpDefenderLocation.squareDistanceTo(sd->getDestination(DefenderID)->getPos()) +
                         defenderLocation.squareDistanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
           if(dist2 < dist1 - historisisFactor*historisisFactor)
           {
             switchPositions(DEFENDER, SPECIAL_OP_DEFENDER);
           }
        }
        //----------------------------------------------------------------
  

}
//========================================
