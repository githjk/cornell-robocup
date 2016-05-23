#include "ThreeManDefenseTransition.h"
#include "info/ball/ball.h"

#include "plays/2002plays/upfieldDefense.h"
#include "plays/2002plays/singleAssistOffense.h"
#include "plays/2002plays/cageDefense.h"
#include "plays/2002plays/looseDefense.h"

void ThreeManDefenseTransition::executePlayTransition(VisionData& field, 
                                                   RobocupStrategyData* rsd,
												                           BasePlay *currentPlay)
{


  Pair ballPos = rsd->getDefensiveBallLocation();
	RobotIndex opponentRobotID;

	//If any robot (but blocker, defender, or special op defender, the guys that form the cage) 
  //has possesion of ball and there is an aggressor
	//then play single assist offense
	//If a non defendering robot has possesion of ball and there is an aggressor
	//then play single assist offense
  if(
      !theyHavePossession(field, rsd->getMutableSystemParams(), &opponentRobotID) &&
	    (
        friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR), rsd->getSystemParams()) ||
        friendlyHasPossession(rsd->getRobotByPosition(CREATOR), rsd->getSystemParams()) 
      ) &&
      rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT
    )
	{
		setPlay(SingleAssistOffense::playNum, rsd);
	}

	//Ball is off to side, go to loose defense.
	else if(
          (
            ballPos.getY() > (rsd->getSystemParams().field.LEFT_MIDDLE_SECTOR + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)||
            ballPos.getY() < (rsd->getSystemParams().field.RIGHT_MIDDLE_SECTOR - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)
          ) &&
          ballPos.getX() < (rsd->getSystemParams().field.HALF_LINE - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) 
         )

	{
		setPlay(LooseDefense::playNum, rsd);
	}

	//Otherwise if ball is in death zone
	//transition to cage defense
	else if(ballPos.getX() < (rsd->getSystemParams().field.DEATH_ZONE_LINE - 
                            rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) &&
          rsd->getRobotByPosition(BLOCKER) != NO_ROBOT)
	{
		setPlay(CageDefense::playNum, rsd);
	}

    //If ball is above half line then transition to upfield defense
	else if(ballPos.getX() > (rsd->getSystemParams().field.HALF_LINE + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS))
	{
		setPlay(UpfieldDefense::playNum, rsd);
	}	   

}


//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void ThreeManDefenseTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/ThreeManDefenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

