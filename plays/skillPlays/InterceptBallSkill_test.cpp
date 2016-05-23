#include "InterceptBallSkill_Test.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
#include "common/util/info/ball/ball.h"
#include "action/action.h"
#include "common/util/math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play InterceptBallSkillTest::playNum = -1;
//====================================
void InterceptBallSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
}
//====================================
ActionFunction InterceptBallSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void InterceptBallSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  ID=rsd->getRobotByPosition(AGGRESSOR);
  if(ID!= NO_ROBOT)
  {
      Pair robotLoc = getLocation(ROBOT0, *vision, rsd->getSystemParams());
      Pair ballLoc=getBallLocation(*vision);
      if(getBallSpeed(*vision) > .01f)
      {
        //Get a handle on the skill set.  Always useful to have...
        SkillSet* skills = rsd->getStrategyModule().getSkillSet(ID);

        //Get a handle on the skill
        Skill* skillHandle = (InterceptBallSkill*)(skills->getSkill(InterceptBallSkill::skillNum));

        //initialize skill
        if(!skillHandle->isInitialized())
	        skillHandle->initialize();

        skillHandle->run();
      }
      else
      {
        rsd->getDestination(ID)->setPos(-1.0f, .5f);
        rsd->getDestination(ID)->setRotation(0.0f);
      }

      //rsd->getDestination(ID)->setRotation(angleToBall(robotLoc, *vision));
  }
}
//====================================
