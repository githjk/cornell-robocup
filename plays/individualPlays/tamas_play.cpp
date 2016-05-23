
#include "tamas_play.h"
#include "../basic_actions.h"
#include "info/robot/robot.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play TamasPlay::playNum = -1;

TamasPlay::TamasPlay(VisionModule *vm) : BasePlay(vm)
{
  loadValues();
}

void TamasPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)  {

}

ActionFunction TamasPlay::getActionFunction(Position pos)
{
  return stopRobot;
}




