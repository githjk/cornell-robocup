#include "puntKickTransition.h"

#include "GUI/GUI.h"

void PuntKickTransition::executePlayTransition(VisionData& field, 
                                                     RobocupStrategyData* rsd,
                                                     BasePlay* currentPlay)
{

}


//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void PuntKickTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/PuntKickTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

