#include "cageDefenseTransition.h"
#include "info/ball/ball.h"

#include "plays/2002plays/cageDefense.h"
#include "plays/2002plays/tightDefense.h"
#include "plays/2002plays/looseDefense.h"
#include "plays/2002plays/ThreeManDefense.h"
#include "plays/2002plays/RemoveBallDefense.h"
#include "skillz/BlockerGainPossessionSkill.h"

void CageDefenseTransition::executePlayTransition(VisionData& field, RobocupStrategyData* rsd, 
												  BasePlay* currentPlay)
{

  Pair ballPos = rsd->getDefensiveBallLocation();
  RobotIndex BlockerID=rsd->getRobotByPosition(BLOCKER);

	if(BlockerID != NO_ROBOT && !friendlyHasPossession(BlockerID,rsd->getSystemParams()) &&
    ((ballPos.getY() > (rsd->getSystemParams().field.LEFT_MIDDLE_SECTOR + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)) ||
	          (ballPos.getY() < (rsd->getSystemParams().field.RIGHT_MIDDLE_SECTOR - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS))))
	{
      setPlay(TightDefense::playNum, rsd);
	}

	//Otherwise if ball is out of death zone
	//transition to three man defense
	else if(ballPos.getX() > (rsd->getSystemParams().field.DEATH_ZONE_LINE + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS))
	{
		setPlay(ThreeManDefense::playNum, rsd);
	}
  //if ball inside goal box, transition to remove ball
  else if(BlockerID != NO_ROBOT &&
          rsd->getStrategyModule().getSkillSet(BlockerID)->getSkill(
          BlockerGainPossessionSkill::skillNum)->isFinished()){
    setPlay(RemoveBallDefense::playNum, rsd);
  }

}


//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void CageDefenseTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/CageDefenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

