//*********************************************************
#include "penaltyShotThemSmartRush2002.h"
#include "../basic_actions.h"
Play PenaltyShotThemSmartRush2002::playNum = -1;

PenaltyShotThemSmartRush2002::PenaltyShotThemSmartRush2002(VisionModule *vm) : BasePlay(vm)
{
   loadValues();
}

//*********************************************************
// Initialize Play
void PenaltyShotThemSmartRush2002::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
 
{
  timer->resetTimer();
  setPlayState(NORMAL_PLAY);
  
  rsd->setActionFunction(BLOCKER, getActionFunction(BLOCKER) );
  rsd->setActionFunction(DEFENDER, getActionFunction(DEFENDER) );
  rsd->setActionFunction(AGGRESSOR, getActionFunction(AGGRESSOR) );
  rsd->setActionFunction(CREATOR, getActionFunction(CREATOR) );
  rsd->setActionFunction(SPECIAL_OP_DEFENDER, getActionFunction(SPECIAL_OP_DEFENDER));
  rsd->setActionFunction(SPECIAL_OP_AGGRESSOR,getActionFunction(SPECIAL_OP_AGGRESSOR) );
  rsd->setActionFunction(SPECIAL_OP_CREATOR, getActionFunction(SPECIAL_OP_CREATOR) );
 }             
 
//*********************************************************
// Execute Play
void PenaltyShotThemSmartRush2002::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  standardExecution(*vision, rsd);
}


//*********************************************************
// Get Action Function
ActionFunction PenaltyShotThemSmartRush2002::getActionFunction(Position pos)
{
  switch(pos)
  {
    case BLOCKER: return Blocker::penaltyShotThemSmartRush2002;  
    case DEFENDER: return Defender::penaltyShotThem2002;
    case AGGRESSOR: return Aggressor::penaltyShotThem2002;
    case CREATOR: return Creator::penaltyShotThem2002; 
    case SPECIAL_OP_DEFENDER: return Special_Op_Defender::dummyActionFunction;
    case SPECIAL_OP_AGGRESSOR: return Special_Op_Aggressor::dummyActionFunction;
    case SPECIAL_OP_CREATOR: return Special_Op_Creator::dummyActionFunction;
    default: return stopRobot;
  }
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void PenaltyShotThemSmartRush2002::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/2002plays/PenaltyShotThemSmartRush2002Params.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************
