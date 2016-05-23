#include "SimpleKick_Test.h"


#include "info/robot/robot.h"


#include "RoboCup_modules/RoboCupStrategyModule.h"



#include "math/dist.h"



#include "GUI/GUI.h"



Play SimpleKickTest::playNum = -1;



//====================================

SimpleKickTest::SimpleKickTest(VisionModule *vm) : BasePlay(vm)
{

}


void SimpleKickTest::initializePlay(const VisionData& vision, 



                                           RobocupStrategyData* rsd)



{
  //Have robot 0 kick



  ID = ROBOT0;

}



//====================================



ActionFunction SimpleKickTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================



void SimpleKickTest::executePlay(VisionData* vision, 



                                        RobocupStrategyData* rsd)



{

	Pair result;
	result = getBallLocation((*vision));
	rsd->getDestination(ID)->setPos(result.getX(),result.getY());
    rsd->getDestination(ID)->setKick(KICK_SHOT);






}



//====================================



