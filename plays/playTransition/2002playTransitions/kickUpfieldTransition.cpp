#include "kickUpfieldTransition.h"
#include "info/ball/ball.h"

#include "plays/2002plays/upfieldDefense.h"

void KickUpfieldTransition::executePlayTransition(VisionData& field, 
                                                     RobocupStrategyData* rsd,
                                                     BasePlay* currentPlay)
{	
  //after 1 second transition ot upfield defense
  if(currentPlay->readTimer() >= TIMEOUT)
  {
  		setPlay(UpfieldDefense::playNum, rsd);
  }

  RobotIndex activeRobotID;
 //If no robot (But blocker) has possesion of ball, then go to upfield defense
  if(
     !weHavePossession(field,rsd->getMutableSystemParams(),*rsd, &activeRobotID) ||
     activeRobotID == rsd->getRobotByPosition(BLOCKER) ||
     activeRobotID == rsd->getRobotByPosition(DEFENDER)
    )
  {
    setPlay(UpfieldDefense::playNum, rsd);
  }
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void KickUpfieldTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/KickUpfieldTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(TIMEOUT);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

