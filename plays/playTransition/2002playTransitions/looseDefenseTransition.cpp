#include "looseDefenseTransition.h"
#include "info/ball/ball.h"

#include "plays/2002plays/upfieldDefense.h"
#include "plays/2002plays/singleAssistOffense.h"
#include "plays/2002plays/tightDefense.h"
#include "plays/2002plays/cageDefense.h"
#include "plays/2002plays/ThreeManDefense.h"

void LooseDefenseTransition::executePlayTransition(VisionData& field, 
                                                   RobocupStrategyData* rsd,
												                           BasePlay *currentPlay)
{
  Pair ballPos = rsd->getDefensiveBallLocation();

  RobotIndex opponentRobotID;
  RobotIndex specopID=rsd->getRobotByPosition(SPECIAL_OP_DEFENDER);


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

	//Ball is in death zones AND tight sector
	//so transition to tight defense
	else if(
          (
            ballPos.getX() < (rsd->getSystemParams().field.DEATH_ZONE_LINE - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) ||
            (
              ballPos.getY() > (rsd->getSystemParams().field.LEFT_SWEET_SECTOR + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)||
              ballPos.getY() < (rsd->getSystemParams().field.RIGHT_SWEET_SECTOR - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)
            )
          ) &&
          ballPos.getX() < (rsd->getSystemParams().field.DEFENSE_ZONE_LINE - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) 
         )

	{
		setPlay(TightDefense::playNum, rsd);
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
    //If ball is above death zone and in the middle, then transition to three man defense
	else if(ballPos.getX() > (rsd->getSystemParams().field.DEATH_ZONE_LINE + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) &&
          (ballPos.getY() < rsd->getSystemParams().field.LEFT_MIDDLE_SECTOR - rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS &&
           ballPos.getY() > rsd->getSystemParams().field.RIGHT_MIDDLE_SECTOR + rsd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS) &&
           specopID != NO_ROBOT &&
           getLocation(specopID,field,rsd->getSystemParams()).getX() <= rsd->getSystemParams().field.DEFENSE_ZONE_LINE
         )
	{
    setPlay(ThreeManDefense::playNum, rsd);
	}	   

}


//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void LooseDefenseTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/LooseDefenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

