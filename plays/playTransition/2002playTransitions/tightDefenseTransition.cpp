#include "tightDefenseTransition.h"
#include "info/ball/ball.h"
#include "common/util/info/ball/possession.h"

#include "plays/2002plays/singleAssistOffense.h"
#include "plays/2002plays/cageDefense.h"
#include "plays/2002plays/looseDefense.h"

void TightDefenseTransition::executePlayTransition(VisionData& field, 
                                                   RobocupStrategyData* rsd,
                                                   BasePlay* currentPlay)
{

  Pair ballPos = rsd->getDefensiveBallLocation();
  RobotIndex opponentRobotID;
	
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

	//Otherwise if the ball is in the middle sector 
	else if((ballPos.getY() < (rsd->getSystemParams().field.LEFT_MIDDLE_SECTOR - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)) && 
	        (ballPos.getY() > (rsd->getSystemParams().field.RIGHT_MIDDLE_SECTOR + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)))
	{
  	//and if the ball is in the death zone then transition to cage defense
	  if(ballPos.getX() < (rsd->getSystemParams().field.DEATH_ZONE_LINE - 
                         rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) &&
       rsd->getRobotByPosition(BLOCKER) != NO_ROBOT)

		{
  		setPlay(CageDefense::playNum, rsd);
		}
		
		//Otherwise transition to loose defense
		else
		{
  		setPlay(LooseDefense::playNum, rsd);
		}
	}

	//Otherwise if ball is not in death zones AND tight sector
	//transition to loose defense
	else if(
          ballPos.getX() > (rsd->getSystemParams().field.DEATH_ZONE_LINE + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) &&
          (
            ballPos.getY() > (rsd->getSystemParams().field.RIGHT_SWEET_SECTOR + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) &&
            ballPos.getY() < (rsd->getSystemParams().field.LEFT_SWEET_SECTOR - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)         
          )
         )
  {
		setPlay(LooseDefense::playNum, rsd);
	}

  //if the ball is forward of the defense zone, go to loose
	else if(
          ballPos.getX() > (rsd->getSystemParams().field.DEFENSE_ZONE_LINE + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)
         )
  {
		setPlay(LooseDefense::playNum, rsd);
	}

}


//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void TightDefenseTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/TightDefenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

