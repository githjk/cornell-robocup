
#include "random_play.h"
#include "../basic_actions.h"
Play RandomPlay::playNum = -1;

RandomPlay::RandomPlay(VisionModule *vm) : BasePlay(vm)
{

}


void RandomPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)  {


}

ActionFunction RandomPlay::getActionFunction(Position pos)
{
  return stopRobot;
}