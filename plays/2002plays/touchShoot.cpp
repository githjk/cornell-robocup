//*********************************************************
#include "touchShoot.h"
#include "../basic_actions.h"
Play TouchShoot::playNum = -1;

TouchShoot::TouchShoot(VisionModule *vm) : BasePlay(vm)
{
   loadValues();
}

//*********************************************************
// Initialize Play
void TouchShoot::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
 
{
  timer->resetTimer();
  RobotIndex specop=rsd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  RobotIndex creator=rsd->getRobotByPosition(CREATOR);
  //If special op is asking for a pass, and creator isn't, pass to spec op.
  if((specop != NO_ROBOT && rsd->getPassValue(specop)) &&
     !(creator != NO_ROBOT && rsd->getPassValue(creator))){
    setPlayState(PASS_TO_SPECIAL_OP);
  }else{
    setPlayState(PASS_TO_CREATOR);
  }

  
  rsd->setActionFunction(BLOCKER, getActionFunction(BLOCKER) );
  rsd->setActionFunction(DEFENDER, getActionFunction(DEFENDER) );
  rsd->setActionFunction(AGGRESSOR, getActionFunction(AGGRESSOR) );
  rsd->setActionFunction(CREATOR, getActionFunction(CREATOR) );
  rsd->setActionFunction(SPECIAL_OP_DEFENDER, getActionFunction(SPECIAL_OP_DEFENDER)
);
  rsd->setActionFunction(SPECIAL_OP_AGGRESSOR,getActionFunction(SPECIAL_OP_AGGRESSOR) );
  rsd->setActionFunction(SPECIAL_OP_CREATOR, getActionFunction(SPECIAL_OP_CREATOR) );
 }             
 
//*********************************************************
// Execute Play
void TouchShoot::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  standardExecution(*vision, rsd);
}


//*********************************************************
// Get Action Function
ActionFunction TouchShoot::getActionFunction(Position pos)
{
  switch(pos)
  {
    case BLOCKER: return Blocker::touchShoot;  
    case DEFENDER: return Defender::touchShoot;
    case AGGRESSOR: return Aggressor::touchShoot;
    case CREATOR: return Creator::touchShoot; 
    case SPECIAL_OP_DEFENDER: return Special_Op_Defender::dummyActionFunction;
    case SPECIAL_OP_AGGRESSOR: return Special_Op_Aggressor::dummyActionFunction;
    case SPECIAL_OP_CREATOR: return Special_Op_Creator::dummyActionFunction;
    default: return stopRobot;
  }
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void TouchShoot::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/2002plays/TouchShootParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //READ_FLOAT(EXAMPLE_VARIABLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************
