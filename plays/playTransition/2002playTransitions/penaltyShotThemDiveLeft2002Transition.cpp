#include "penaltyShotThemDiveRight2002Transition.h"
#include "info/ball/ball.h"
#include "plays/2002plays/upfieldDefense.h"

//*********************************************************
void PenaltyShotThemDiveRight2002Transition::executePlayTransition(VisionData& field, 
                                                     RobocupStrategyData* rsd,
                                                     BasePlay* currentPlay)
{
  if( currentPlay->readTimer() >= TIMEOUT )
  {
  		setPlay(UpfieldDefense::playNum, rsd);
  }

}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void PenaltyShotThemDiveRight2002Transition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/PenaltyShotThemDiveRight2002TransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(TIMEOUT);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

