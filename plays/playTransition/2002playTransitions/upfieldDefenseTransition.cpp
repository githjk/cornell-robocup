#include "upfieldDefenseTransition.h"
#include "info/ball/ball.h"

#include "plays/2002plays/looseDefense.h"
#include "plays/2002plays/ThreeManDefense.h"
#include "plays/2002plays/singleAssistOffense.h"

#include "Skillz/StripBallSkill.h"

#include "GUI/GUI.h"

void UpfieldDefenseTransition::executePlayTransition(VisionData& field, 
                                                     RobocupStrategyData* rsd,
                                                     BasePlay* currentPlay)
{

	Pair ballPos = getBallLocation(field);
  RobotIndex opponentRobotID;
	RobotIndex specopID=rsd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  if(specopID == NO_ROBOT) specopID=rsd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  if(specopID == NO_ROBOT) specopID=rsd->getRobotByPosition(SPECIAL_OP_CREATOR);

	//If a non defendering robot has possesion of ball and there is an aggressor
	//then play single assist offense
  if(
      !theyHavePossession(field, rsd->getMutableSystemParams(), &opponentRobotID) &&
	    (
        friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR), rsd->getSystemParams()) ||
        friendlyHasPossession(rsd->getRobotByPosition(CREATOR), rsd->getSystemParams()) ||
        friendlyHasPossession(rsd->getRobotByPosition(SPECIAL_OP_DEFENDER), rsd->getSystemParams())
      ) &&
      rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT
    )
	{
  	setPlay(SingleAssistOffense::playNum, rsd);
	}

  //if the strip skill thinks it has stripped the ball, then go to single assist offense
  if(rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT &&
     ((StripBallSkill*)rsd->getStrategyModule().getSkillSet(rsd->getRobotByPosition(AGGRESSOR))->
     getSkill(StripBallSkill::skillNum))->ballIsStripped())
  {
    GUI_Record.debuggingInfo.addDebugMessage("Aggressor believes he has stripped the ball, going into offense.");
  	setPlay(SingleAssistOffense::playNum, rsd);
  }

	//Otherwise if the ball is on our side from the half line
	//transition to looseDefense or three man defense
	else if(ballPos.getX() < (rsd->getSystemParams().field.HALF_LINE - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS))
	{
    if(ballPos.getY() < rsd->getSystemParams().field.LEFT_MIDDLE_SECTOR - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS &&
      ballPos.getY() > rsd->getSystemParams().field.RIGHT_MIDDLE_SECTOR + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS &&
      specopID != NO_ROBOT &&
      getLocation(specopID,field,rsd->getSystemParams()).getX() <= rsd->getSystemParams().field.DEFENSE_ZONE_LINE
      ){
      setPlay(ThreeManDefense::playNum, rsd);
    }else{
  		setPlay(LooseDefense::playNum, rsd);
    }
	}

}


//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void UpfieldDefenseTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/UpfieldDefenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

