#include "KickUpfieldStorm.h"
#include "common/util/info/ball/ball.h"

//========================================
KickUpfieldStorm::KickUpfieldStorm(RoboCupStrategyModule* strategy) : 
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
void KickUpfieldStorm::preformSwitching(VisionModule* vm,
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
    //ensure the closest robot to the ball is the aggressor
    Position closestRobot = NO_POSITION;
    float closestDistance = -1.0f;
    
    if(AggressorID != NO_ROBOT)
    {
      closestDistance = aggressorLocation.distanceTo(ball);
      closestRobot = AGGRESSOR;
    }
    if(CreatorID != NO_ROBOT &&
       (creatorLocation.distanceTo(ball) < closestDistance - historisisFactor ||
        closestRobot == NO_ROBOT))
    {
      closestDistance = creatorLocation.distanceTo(ball);
      closestRobot = CREATOR;
    }
    if(SpecialOpDefenderID != NO_ROBOT &&
       (specialOpDefenderLocation.distanceTo(ball) < closestDistance - historisisFactor ||
        closestRobot == NO_ROBOT))
    {
      closestDistance = specialOpDefenderLocation.distanceTo(ball);
      closestRobot = SPECIAL_OP_DEFENDER;
    }

    if(closestRobot != NO_POSITION && AggressorID != NO_ROBOT)
      switchPositions(AGGRESSOR, closestRobot);
    //----------------------------------------------------------------
    //minimize distances special op aggressor and creator have to travel
    if(SpecialOpDefenderID != NO_ROBOT &&
       CreatorID != NO_ROBOT)
    {
       float dist1 = creatorLocation.squareDistanceTo(sd->getDestination(CreatorID)->getPos()) +
                     specialOpDefenderLocation.squareDistanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
       float dist2 = specialOpDefenderLocation.squareDistanceTo(sd->getDestination(CreatorID)->getPos()) +
                     creatorLocation.squareDistanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
       if(dist2 < dist1 - historisisFactor*historisisFactor)
       {
         switchPositions(CREATOR, SPECIAL_OP_DEFENDER);
       }
    }
    //----------------------------------------------------------------
}
//========================================
