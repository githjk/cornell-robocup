#include "SimpleDribble_Test.h"
#include "info/robot/robot.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "math/dist.h"
#include "GUI/GUI.h"
Play SimpleDribbleTest::playNum = -1;
//====================================
SimpleDribbleTest::SimpleDribbleTest(VisionModule *vm) : BasePlay(vm)
{

}

void SimpleDribbleTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
 //Have robot 0 Dribble
  ID = ROBOT0;
}
//====================================
ActionFunction SimpleDribbleTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void SimpleDribbleTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
	Pair result;
	result = getBallLocation((*vision));
    rsd->getDestination(ID)->setPos(result.getX(),result.getY());
	rsd->getDestination(ID)->setDribble(FAST_DRIBBLE);
}
//====================================
