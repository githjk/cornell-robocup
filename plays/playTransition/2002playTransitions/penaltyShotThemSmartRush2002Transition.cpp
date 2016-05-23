#include "penaltyShotThemSmartRush2002Transition.h"
#include "plays/2002plays/upfieldDefense.h"

//*********************************************************
void PenaltyShotThemSmartRush2002Transition::executePlayTransition(VisionData& field, 
                                                     RobocupStrategyData* rsd,
                                                     BasePlay* currentPlay)
{
  RobotIndex BlockerID = rsd->getRobotByPosition(BLOCKER);

  //if timer runs out or blocker gets within a tolerance of the ball
  if(
      currentPlay->readTimer() >= TIMEOUT ||
      (
        BlockerID != NO_ROBOT &&
        getLocation(BlockerID, field, rsd->getSystemParams()).distanceTo
           (rsd->getStrategyModule().getVisionModule()->getLastUnpredictedBall()) <=
           (
             rsd->getSystemParams().general.PLAYER_RADIUS +
             rsd->getSystemParams().general.BALL_RADIUS +
             BALL_THRESH
           )
      )
    )
  {
  		setPlay(UpfieldDefense::playNum, rsd);
  }
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void PenaltyShotThemSmartRush2002Transition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/PenaltyShotThemSmartRush2002TransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(TIMEOUT);
  READ_FLOAT(BALL_THRESH);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

