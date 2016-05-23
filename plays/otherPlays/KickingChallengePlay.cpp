#include "KickingChallengePlay.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "Skillz/TestSkill.h"
#include <iostream.h>
#include "Skillz/TestSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/shoot/shoot.h"
#include "common/util/info/robot/robot.h"
#include "common/util/pair/pair.h"
#include "common/util/math/angle.h"
#include "common/util/defense/defending.h"
#include "info/ball/ball.h"
#include "GUI/GUI.h"

Play KickingChallengePlay::playNum = -1;

void KickingChallengePlay::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
   state = SINGLE_ASSIST;
   currentID = rsd->getRobotByPosition(AGGRESSOR);
   if(currentID == NO_ROBOT)
   {
     currentID = ROBOT0;
   }
   skills = rsd->getStrategyModule().getSkillSet(currentID);
   acquireSkill = (AcquirePossessionSkill*)skills->getSkill(AcquirePossessionSkill::skillNum);
   advanceSkill = (AdvanceBallUpfieldSkill*)skills->getSkill(AdvanceBallUpfieldSkill::skillNum);
   jamSkill = (JamAndShootSkill*)skills->getSkill(JamAndShootSkill::skillNum);
    
   


}

void KickingChallengePlay::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  
  RobotIndex opponentRobotID,robotID; 
  Pair goalPos(rsd->getSystemParams().field.THEIR_GOAL_LINE, rsd->getSystemParams().field.SPLIT_LINE);
  Pair aggressorPos(getLocation(currentID,*vision,rsd->getSystemParams()));


  /// Transisitions from SingleAssist
   if(state == SINGLE_ASSIST &&
      (
        !weHavePossession(*vision,rsd->getMutableSystemParams(),*rsd, &robotID) ||
        currentID == rsd->getRobotByPosition(BLOCKER) ||
        currentID == rsd->getRobotByPosition(DEFENDER)
      ) &&
      (
        rsd->getRobotByPosition(AGGRESSOR) == NO_ROBOT ||
        (
         !ballIsBetween(getLocation(rsd->getRobotByPosition(AGGRESSOR), *vision, rsd->getSystemParams()),
                        rsd->getDestination(rsd->getRobotByPosition(AGGRESSOR))->getPos(),
                        *vision,
                        rsd->getSystemParams(),
                        2*rsd->getSystemParams().general.PLAYER_RADIUS,
                        1.5f*rsd->getSystemParams().general.PLAYER_RADIUS) &&
         isRobocupBallFound(*vision)
        )
      )
    )
   {

     rsd->setMessage(currentID,"Doing Upfield");
     GUI_Record.debuggingInfo.addDebugMessage("IN UPFIELD");
     state = UPFIELD_DEFENSE;
     uninitializeSkills();
     
   }


   if (state == SINGLE_ASSIST && currentID != NO_ROBOT &&
       jamSkill->isValid() &&
       dist(goalPos,aggressorPos) < DISTANCE_GOAL)
   {
     rsd->setMessage(currentID,"Doing Jam and Shoot");
     GUI_Record.debuggingInfo.addDebugMessage("IN JAM SHOOT");
     state = JAM_AND_SHOOT;
     uninitializeSkills();
     
   }


  /// Transistions from Jam and Shoot
   if(state == JAM_AND_SHOOT && rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT &&
      jamSkill->isFinished(0.0f) &&
      !friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR),rsd->getSystemParams(),*vision, *rsd,true))
   {
     rsd->setMessage(currentID,"Doing Upfield");
     GUI_Record.debuggingInfo.addDebugMessage("IN UPFIELD");
     state = UPFIELD_DEFENSE;
     uninitializeSkills();
     
   }

  /// Transistions from UpfieldDefense
  if(state == UPFIELD_DEFENSE && !theyHavePossession(*vision, rsd->getMutableSystemParams(), &opponentRobotID) &&
	    friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR), rsd->getSystemParams()) &&
      rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT )
  {
    rsd->setMessage(currentID,"SingleAssist");
    GUI_Record.debuggingInfo.addDebugMessage("IN SINGLEASSIST");
    state = SINGLE_ASSIST;
    uninitializeSkills();
    
  }


  if(state == JAM_AND_SHOOT)
  { 
    rsd->setMessage(currentID,"Jam and Shoot");
    doJamAndShoot(rsd);
  }
  else if(state == UPFIELD_DEFENSE)
  { 
    rsd->setMessage(currentID,"Upfield Defense");
    doUpfieldDefense(rsd);
  }
  else if(state == SINGLE_ASSIST)
  { 
    rsd->setMessage(currentID,"SingleAssist");
    doSingleAssist(rsd);
  }

  
}
    
    

void KickingChallengePlay::doSingleAssist(RobocupStrategyData* rsd)
{

  //-------------------
  //initialize skill if unitialized
  //-------------------
  if(!advanceSkill->isInitialized())
	  advanceSkill->initialize();

  //-------------------
  //run skill
  //-------------------
  advanceSkill->run();
  rsd->getDestination(currentID)->setSpeed(FORMATION_SPEED);
}


void KickingChallengePlay::doUpfieldDefense(RobocupStrategyData* rsd)
{

  //-------------------
  //initialize skill if it is not initialized
  //-------------------
  if(!acquireSkill->isInitialized())
	  acquireSkill->initialize();

  //-------------------
  //run skill
  //-------------------
  acquireSkill->run();
}

void KickingChallengePlay::doJamAndShoot(RobocupStrategyData* rsd)
{
  
   
  //-------------------
  //initialize skill if unitialized
  //-------------------
  if(!jamSkill->isInitialized())
	  jamSkill->initialize();

  //-------------------
  //run skill
  //-------------------
  jamSkill->run();
  //rsd->getDestination(currentID)->setSpeed(FORMATION_SPEED);
}

void KickingChallengePlay::uninitializeSkills()
{
  acquireSkill->unInitialize();
  advanceSkill->unInitialize();
  jamSkill->unInitialize();
}


//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void KickingChallengePlay::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/other/KickingChallengePlayParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  ///Read the parameters
  READ_FLOAT(DISTANCE_GOAL);
}

