#include "penaltyShotUsOpenLoop2002Transition.h"
#include "plays/2002plays/upfieldDefense.h"
#include "plays/2002plays/jamAndShoot.h"
#include "Skillz/PenaltySpinSkill.h"
//*********************************************************
void PenaltyShotUsOpenLoop2002Transition::executePlayTransition(VisionData& field, 
                                                     RobocupStrategyData* rsd,
                                                     BasePlay* currentPlay)
{
  bool done = false;
  if(rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT)
  {
    //-------------------
    //get a handle on skillset for this robot
    //-------------------
    SkillSet* skills = rsd->getStrategyModule().getSkillSet(rsd->getRobotByPosition(AGGRESSOR));

    //-------------------
    //get a handle on upfield defense aggressor skill
    //-------------------
    PenaltySpinSkill* skillHandle = 
    (PenaltySpinSkill*)skills->getSkill(PenaltySpinSkill::skillNum);

    done = skillHandle->isFinished(0.0f);
  }

  if(
      currentPlay->readTimer() >= TIMEOUT ||
      done
    )
  {
  		setPlay(JamAndShoot::playNum, rsd);
  }
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void PenaltyShotUsOpenLoop2002Transition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/PenaltyShotUsOpenLoop2002TransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(TIMEOUT);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

