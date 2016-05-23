//*********************************************************
#include "freeKick.h"
#include "../basic_actions.h"

Play FreeKick::playNum = -1;

FreeKick::FreeKick(VisionModule *vm) : BasePlay(vm)
{
   loadValues();
}

//*********************************************************
// Initialize Play
void FreeKick::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
 
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
void FreeKick::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  //run normal stuff
  standardExecution(*vision, rsd);
  
  //set the kick speed of the aggressor to kick shot
  RobotIndex aggressorID = rsd->getRobotByPosition(AGGRESSOR);

  if(aggressorID != NO_ROBOT)
  {
    rsd->setMessage(aggressorID, "KICKING HARD!");
    rsd->getDestination(aggressorID)->setKick(KICK_SHOT);
  }
}
//*********************************************************
// Get Action Function
ActionFunction FreeKick::getActionFunction(Position pos)
{
  switch(pos)
  {
    case BLOCKER: return Blocker::kickUpfield;  
    case DEFENDER: return Defender::kickUpfield;
    case AGGRESSOR: return Aggressor::kickUpfield;
    case CREATOR: return Creator::kickUpfield; 
    case SPECIAL_OP_DEFENDER: return Special_Op_Defender::dummyActionFunction;
    case SPECIAL_OP_AGGRESSOR: return Special_Op_Aggressor::dummyActionFunction;
    case SPECIAL_OP_CREATOR: return Special_Op_Creator::dummyActionFunction;
    default: return stopRobot;
  }
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void FreeKick::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/2002plays/KickUpfieldParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************
