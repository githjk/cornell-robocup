#include "penaltyShotUsPivot2002Transition.h"
#include "Skillz/PenaltyShotUsAimAndShootSkill.h"
#include "plays/2002plays/upfieldDefense.h"
#include "plays/2002plays/jamAndShoot.h"

//*********************************************************
void PenaltyShotUsPivot2002Transition::executePlayTransition(VisionData& field, 
                                                     RobocupStrategyData* rsd,
                                                     BasePlay* currentPlay)
{
  if(currentPlay->readTimer() >= TIMEOUT)
  {
  		setPlay(JamAndShoot::playNum, rsd);
  }

  //when aggressor is finished, go to upfield defense.
  RobotIndex AggressorID = rsd->getRobotByPosition(AGGRESSOR);
  if(AggressorID != NO_ROBOT)
  {
      SkillSet* skills = rsd->getStrategyModule().getSkillSet(AggressorID);
      Skill* skill = skills->getSkill(PenaltyShotUsAimAndShootSkill::skillNum);
      if(skill->isFinished())
      {
		    setPlay(UpfieldDefense::playNum, rsd);   
      }
  }
  else
  {
    setPlay(UpfieldDefense::playNum, rsd);
  }
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void PenaltyShotUsPivot2002Transition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/PenaltyShotUsPivot2002TransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(TIMEOUT);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

