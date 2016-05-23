#include "doublePickOffenseTransition.h"
#include "common/util/math/dist.h"
#include "common/util/shoot/shoot.h"
#include "info/ball/ball.h"

#include "plays/2002plays/upfieldDefense.h"
#include "skillz/DribblePickScoreSkill.h"
#include "plays/2002plays/singleAssistOffense.h"
#include "plays/2002plays/jamAndShoot.h"

#include "Skillz/AdvanceBallUpfieldSkill.h"
//*********************************************************
void DoublePickOffenseTransition::executePlayTransition(VisionData& field, 
                                                        RobocupStrategyData* rsd, 
			  									                              BasePlay* currentPlay)
{
  if(rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT)
  {
  
      Pair ball=rsd->getOffensiveBallLocation();
      SkillSet* skills = rsd->getStrategyModule().getSkillSet
        (rsd->getRobotByPosition(AGGRESSOR));
      DribblePickScoreSkill* skillHandle = 
      (DribblePickScoreSkill*)skills->getSkill(DribblePickScoreSkill::skillNum);

     //If no robot (But blocker) has possesion of ball
     //then go to upfield defense
      RobotIndex activeRobotID;
      if(!weHavePossession(field,rsd->getMutableSystemParams(),*rsd, &activeRobotID) ||
        activeRobotID == rsd->getRobotByPosition(BLOCKER) ||
        activeRobotID == rsd->getRobotByPosition(DEFENDER))
      {
		    setPlay(UpfieldDefense::playNum, rsd);
	    }
      //if aggressor finished, go to single assist
      else if(skillHandle->isFinished(FINISH_DIST)  || ball.getX() <= rsd->getSystemParams().field.HALF_LINE)
      {
		    setPlay(SingleAssistOffense::playNum, rsd);
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
void DoublePickOffenseTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/DoublePickOffenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(MAX_ELAPSED_TIME);
  READ_FLOAT(FINISH_DIST);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

