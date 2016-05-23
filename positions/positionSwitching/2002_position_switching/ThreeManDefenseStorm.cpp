#include "ThreeManDefenseStorm.h"
#include "common/util/info/robot/robot.h"
#include "common/util/info/ball/ball.h"
#include "common/util/math/perpendicular.h"
#include "common/util/2002Strategy/2002Strategy.h"
#include "GUI/GUI.h"
//========================================
ThreeManDefenseStorm::ThreeManDefenseStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList0[] = {BLOCKER};
  clouds[0] = new Cloud(pList0, 1);

  Position pList2[] = {AGGRESSOR,
                       DEFENDER,
                       SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR};  
  clouds[1] = new Cloud(pList2, 5);

  Position pList3[] = {CREATOR};
  clouds[2] = new Cloud(pList3, 1);
}
//========================================
void ThreeManDefenseStorm::preformSwitching(VisionModule* vm,
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
    Pair ball = sd->getDefensiveBallLocation();
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
    //----------------------------------------------------------------
    //do NOT switch aggressor if 3man hasn't formed yet.
    if(isThreeManDefenseFormed(*vision,*sd,*params))
    {
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
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    //----------------------------------------------------------------
     /*
    bool considerSwitchingDtoA = true;
//==========================================
    //check if the aggressor should take over as the special op or the defender
    if(AggressorID != NO_ROBOT)
    {
      bool considerDefender = false;
      bool considerSpecialOp = false;
      float dDistance = -1.0f;
      float sDistance = -1.0f;
      float aDistance = -1.0f;
      Pair ballLoc = getBallLocation(*vm->getCurrentFrame());

      float minOpponentDistance = -1.0f;
      for(int i=0; i<NUM_ROBOTS; i++)
      {      
        if(theirRobotFound((RobotIndex)i,
                           *vm->getCurrentFrame(),
                           strategy->getCurrentRoboCupFrame()->getSystemParams()))
        {
          float tempDist = ballLoc.distanceTo(getLocation(strategy->getCurrentRoboCupFrame()->
                                          getSystemParams().general.OTHER_TEAM,
                                          (RobotIndex)i,
                                          *vm->getCurrentFrame()));
          if(minOpponentDistance < 0.0f ||
             tempDist < minOpponentDistance)
          {
            minOpponentDistance = tempDist;
          }
        }
      }

      if(DefenderID != NO_ROBOT)
      {
        dDistance = defenderLocation.distanceTo(sd->getDestination(DefenderID)->getPos());
        aDistance = aggressorLocation.distanceTo(sd->getDestination(DefenderID)->getPos());
        considerDefender = true;
      }

      if(SpecialOpDefenderID != NO_ROBOT)
      {
        sDistance = specialOpDefenderLocation.distanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
        float tempDist = aggressorLocation.distanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
        if(
            !considerDefender ||
            (tempDist < aDistance)
          )
        {
          aDistance = tempDist;
          considerDefender = false;
          considerSpecialOp = true;
        }
      }

      if(considerDefender)
      {
        if(aDistance < dDistance &&
           minOpponentDistance < 0.4 &&
           minOpponentDistance >= 0.0)
        {           
          switchPositions(AGGRESSOR, DEFENDER);
          considerSwitchingDtoA = false;
        }
      }
      else
      {
        if(considerSpecialOp)
        {
        if(aDistance < sDistance &&
           minOpponentDistance < 0.4 &&
           minOpponentDistance >= 0.0)
        {           
          switchPositions(AGGRESSOR, SPECIAL_OP_DEFENDER);
          considerSwitchingDtoA = false;
        }

        }
      }
    }
    //==========================================  
    //Carefully decide if either the special op or the defender should become the aggressor
    if(SpecialOpDefenderID != NO_ROBOT && 
            DefenderID != NO_ROBOT && 
            AggressorID != NO_ROBOT  &&
            ball.getX() < params->field.DEFENSE_ZONE_LINE &&
            considerSwitchingDtoA)
    {

      //find the distance of the closest opponent to the ball
      float minOpponentDist=32000.0f;
      float currentDist;
      for(RobotIndex i=ROBOT0; i<NUM_ROBOTS ; i++){
        currentDist=ball.distanceTo(getLocation(params->general.OTHER_TEAM,i,*vision));
        if(currentDist < minOpponentDist){
          minOpponentDist=currentDist;
        }
      }

      //find the closer of defender and specop
      float minFriendDist=ball.distanceTo(specialOpDefenderLocation);
      RobotIndex closeID=SpecialOpDefenderID;
      Pair closePos=specialOpDefenderLocation;
      Position closeGuy=SPECIAL_OP_DEFENDER;
      if(ball.distanceTo(defenderLocation) < minFriendDist){
        minFriendDist=ball.distanceTo(defenderLocation);
        closeID=DefenderID;
        closePos=defenderLocation;
        closeGuy=DEFENDER;
      }

      //See if we're closer than aggressor
      if(ball.distanceTo(aggressorLocation) - historisisFactor > minFriendDist)
      {

        //calculate and compare the times
        float opponentTime=minOpponentDist/params->general.OPPONENT_SPEED.MAX_VELOCITY;
        float friendlyTime; // simple:   =minFriendDist/strategy->getRobotTrajParams(closeID)->DEFAULT.MAX_VELOCITY;

        ObjectPosVel start, final;
        start.xPos = closePos.getX();
        start.yPos = closePos.getY();
 
        final.xPos = ball.getX();
        final.yPos = ball.getY();

        expectedPathTime(start,final,
                         params->general.FRAME_RATE,
                         strategy->getRobotTrajParams(closeID)->DEFAULT.MAX_ACCELERATION,
                         strategy->getRobotTrajParams(closeID)->DEFAULT.MAX_VELOCITY, friendlyTime);

        //if we can beat them, switch
        if(friendlyTime < opponentTime + BUFFER_TIME)
        {
          if(closeGuy == DEFENDER)
          {
            switchPositions(DEFENDER, SPECIAL_OP_DEFENDER);
          }
          switchPositions(AGGRESSOR,SPECIAL_OP_DEFENDER);
          GUI_Record.debuggingInfo.addDebugMessage("Took one of the guys from 3man to be aggressor.");
        }
      }


    }
    */
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    //minimize distances special op defender and defender have to travel
    //However, do no switch if the specop is not close to the defender's location.
    if(SpecialOpDefenderID != NO_ROBOT &&
       DefenderID != NO_ROBOT)
    {
       float dist1 = defenderLocation.squareDistanceTo(sd->getDestination(DefenderID)->getPos()) +
                     specialOpDefenderLocation.squareDistanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
       float dist2 = specialOpDefenderLocation.squareDistanceTo(sd->getDestination(DefenderID)->getPos())+
                     defenderLocation.squareDistanceTo(sd->getDestination(SpecialOpDefenderID)->getPos());
       if(
           dist2 < dist1 - historisisFactor*historisisFactor && 
           specialOpDefenderLocation.getX()<params->field.DEFENSE_ZONE_LINE &&
           specialOpDefenderLocation.distanceTo(sd->getDestination(DefenderID)->getPos()) <= 4*params->general.PLAYER_RADIUS
         )
       {
         switchPositions(DEFENDER, SPECIAL_OP_DEFENDER);
       }
    }
    //----------------------------------------------------------------
}
//========================================
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void ThreeManDefenseStorm::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Position_Params/PositionSwitching/2002_position_switching/ThreeManDefenseStormParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(BUFFER_TIME);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************
