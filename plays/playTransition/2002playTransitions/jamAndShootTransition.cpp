#include "JamAndShootTransition.h"
#include "common/util/math/dist.h"
#include "info/ball/ball.h"

#include "plays/2002plays/upfieldDefense.h"

#include "Skillz/JamAndShootSkill.h"

#include "GUI/GUI.h"

void JamAndShootTransition::executePlayTransition(VisionData& field, RobocupStrategyData* rsd, 
			  									        BasePlay* currentPlay)
{
  
  
  if(rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT)
  {
      //If aggressor has kicked then transion to upfield defense
      if(rsd->getStrategyModule().getSkillSet(rsd->getRobotByPosition(AGGRESSOR))->getSkill(JamAndShootSkill::skillNum)->isFinished())
	    {
		    setPlay(UpfieldDefense::playNum, rsd);
	    }
      //if the aggressor does not have the ball, and he has not kicked, go to upfield defense
      else if(!friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR), 
	                                 rsd->getSystemParams(), 
									 field, 
									 *rsd, 
									 true))
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
void JamAndShootTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/JamAndShootTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

