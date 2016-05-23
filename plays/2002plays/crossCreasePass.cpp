#include "crossCreasePass.h"
#include "../basic_actions.h"
Play CrossCreasePass::playNum = -1;


CrossCreasePass::CrossCreasePass(VisionModule *vm) : BasePlay(vm)
{
   loadValues();
}

//*********************************************************
// Initialize Play
void CrossCreasePass::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)  
{
  timer->resetTimer();
  setPlayState(NORMAL_PLAY);
  rsd->setActionFunction(BLOCKER, getActionFunction(BLOCKER) );
  rsd->setActionFunction(DEFENDER, getActionFunction(DEFENDER) );
  rsd->setActionFunction(AGGRESSOR, getActionFunction(AGGRESSOR) );
  rsd->setActionFunction(CREATOR, getActionFunction(CREATOR) );
  rsd->setActionFunction(SPECIAL_OP_DEFENDER, getActionFunction(SPECIAL_OP_DEFENDER) );
  rsd->setActionFunction(SPECIAL_OP_AGGRESSOR, getActionFunction(SPECIAL_OP_AGGRESSOR) );
  rsd->setActionFunction(SPECIAL_OP_CREATOR, getActionFunction(SPECIAL_OP_CREATOR) );
}
//*********************************************************
// Execute Play
void CrossCreasePass::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  standardExecution(*vision, rsd);
}
//*********************************************************
// Get Action Function
ActionFunction CrossCreasePass::getActionFunction(Position pos)
{
	switch(pos)
	{
  	case BLOCKER: return Blocker::crossCreasePass;  
  	case DEFENDER: return Defender::crossCreasePass;
	  case AGGRESSOR: return Aggressor::crossCreasePass;
  	case CREATOR: return Creator::crossCreasePass; 
  	case SPECIAL_OP_DEFENDER: return Special_Op_Defender::dummyActionFunction;
  	case SPECIAL_OP_AGGRESSOR: return Special_Op_Aggressor::dummyActionFunction;
  	case SPECIAL_OP_CREATOR: return Special_Op_Creator::dummyActionFunction;
  	default: return stopRobot;
	}
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void CrossCreasePass::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/2002plays/CrossCreasePassParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************