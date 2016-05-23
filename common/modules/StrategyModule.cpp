

#include "StrategyModule.h"
#include "common/util/info/ball/possession.h"


Pair StrategyModule::getLastUnpredictedBall()
{
  return vision->getLastUnpredictedBall();
}


StrategyData::StrategyData()
{
  //initialize all messsages
	for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
	  strcpy(robotMessages[i], "hanging out...");
    friendlyPossession[i] = false;
    opponentPossession[i] = false;
  }
}
void StrategyModule::setStandardModule(StandardModule* standardModule){
    this->standardModule = standardModule;
}


// executes one frame of strategy
void StrategyModule::executeStrategy(){
  updateRobotsPossession(*vision->getRawFrame(),*SP, getCurrentFrame());
  
  StrategyData* strategyFrame = getCurrentFrame();

	strategyFrame->getDestination(0)->initialize();
	strategyFrame->getDestination(1)->initialize();
	strategyFrame->getDestination(2)->initialize();
	strategyFrame->getDestination(3)->initialize();
	strategyFrame->getDestination(4)->initialize();

	switch (hack_Formation){
		default:
		case 0:	strategyFrame->getDestination(0)->setPos(0.8f, 0);
				strategyFrame->getDestination(1)->setPos(-1, 0);
				strategyFrame->getDestination(2)->setPos(-1,  0.5f);
				strategyFrame->getDestination(3)->setPos(-0.4f,0.5f);
				strategyFrame->getDestination(4)->setPos(-0.6f,-0.5f);

				strategyFrame->getDestination(0)->setRotation(PI/2);
				strategyFrame->getDestination(1)->setRotation(PI/2);
				strategyFrame->getDestination(2)->setRotation(PI/2);
				strategyFrame->getDestination(3)->setRotation(PI/2);
				strategyFrame->getDestination(4)->setRotation(PI/2);
				return;
		case 1:	strategyFrame->getDestination(0)->setPos(-2.45f,1.63f);
				strategyFrame->getDestination(1)->setPos(-2.15f, 1.53f);
				strategyFrame->getDestination(2)->setPos(-1.85f,  1.63f);
				strategyFrame->getDestination(3)->setPos(-1.55f,1.53f);
				strategyFrame->getDestination(4)->setPos(-1.25f,1.63f);
				return;
		case 2:	strategyFrame->getDestination(0)->setPos(-.1f,0.6f);
				strategyFrame->getDestination(1)->setPos(-.1f, 1);
				strategyFrame->getDestination(2)->setPos(-0.8f,  -0.2f);
				strategyFrame->getDestination(3)->setPos(-0.8f,-1.3f);
				strategyFrame->getDestination(4)->setPos(-2,0.2f);
				return;
	}
}

// executes one frame of strategy
void StrategyModule::executeNoStrategy()
{
  updateRobotsPossession(*vision->getRawFrame(),*SP, getCurrentFrame());
}

//=======================================================================================
//=======================================================================================
//set if a robot has possession
void StrategyData::setFriendlyPossession(RobotIndex i, bool value)
{
  if(i >= ROBOT0 && i < NUM_ROBOTS)
    friendlyPossession[i] = value;
}

void StrategyData::setOpponentPossession(RobotIndex i, bool value)
{
  if(i >= ROBOT0 && i < NUM_ROBOTS)
    opponentPossession[i] = value;
}
//=======================================================================================
//=======================================================================================
//get if a robot has possession
bool StrategyData::getFriendlyPossession(RobotIndex i)
{
  if(i >= ROBOT0 && i < NUM_ROBOTS)
    return friendlyPossession[i];
  else
    return false;
}

bool StrategyData::getOpponentPossession(RobotIndex i)
{
  if(i >= ROBOT0 && i < NUM_ROBOTS)
    return opponentPossession[i];
  else
    return false;
}
//=======================================================================================
//=======================================================================================
RobotTypeParams * StrategyModule::getRobotTrajParams(RobotIndex robotID){
  return standardModule->getRobotTrajParams(robotID);
}

