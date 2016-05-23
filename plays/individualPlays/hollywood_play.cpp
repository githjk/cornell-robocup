
#include "hollywood_Play.h"
#include "../basic_actions.h"
#include "info/robot/robot.h"
#include "action/dribble.h"
#include "info/ball/possession.h"
#include "info/ball/ball.h"
#include "shoot/shoot.h"
#include "math/angle.h"
#include "math/dist.h"

Play HollywoodPlay::playNum = -1;


HollywoodPlay::HollywoodPlay(VisionModule *vm) : BasePlay(vm)
{
  loadValues();
}

void HollywoodPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)  
{

}

ActionFunction HollywoodPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void HollywoodPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  standardExecution(*vision, rsd);
  rsd->resetChangePlay();
}


