#include "crossCreasePassTransition.h"
#include "common/util/math/dist.h"
#include "common/util/shoot/shoot.h"
#include "info/ball/ball.h"

#include "plays/2002plays/upfieldDefense.h"
#include "plays/2002plays/touchShoot.h"
#include "plays/2002plays/jamAndShoot.h"

#include "Skillz/CrossCreasePassSkill.h"


void CrossCreasePassTransition::executePlayTransition(VisionData& field, RobocupStrategyData* rsd, 
			  									        BasePlay* currentPlay)
{
  if(rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT)
  {
      SkillSet* skills = rsd->getStrategyModule().getSkillSet
        (rsd->getRobotByPosition(AGGRESSOR));
      CrossCreasePassSkill* skillHandle = 
      (CrossCreasePassSkill*)skills->getSkill(CrossCreasePassSkill::skillNum);

      RobotIndex activeRobotID;
     //If no robot (But blocker) has possesion of ball
     //then go to upfield defense
      if(!weHavePossession(field,rsd->getMutableSystemParams(),*rsd, &activeRobotID) ||
        activeRobotID == rsd->getRobotByPosition(BLOCKER) ||
        activeRobotID == rsd->getRobotByPosition(DEFENDER))
      {
		    setPlay(UpfieldDefense::playNum, rsd);
	    }
      //if aggressor finished, go to touch shoot
      else if(skillHandle->isFinished(0.0f))
      {
		    setPlay(TouchShoot::playNum, rsd);
      }
      //else if max time elapsed, go to jam and shoot
      else if(currentPlay->readTimer() >= MAX_ELAPSED_TIME)
	    {
		    setPlay(JamAndShoot::playNum, rsd);
	    }
  }
  else
  {
     setPlay(UpfieldDefense::playNum, rsd);
  }

}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void CrossCreasePassTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/CrossCreasePassTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(MAX_ELAPSED_TIME);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

