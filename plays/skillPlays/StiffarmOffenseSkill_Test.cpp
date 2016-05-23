#include "StiffarmOffenseSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/StiffarmOffenseSkill.h"
Play StiffarmOffenseSkillTest::playNum = -1;
//====================================
void StiffarmOffenseSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (StiffarmOffenseSkill*)(skills->getSkill(StiffarmOffenseSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction StiffarmOffenseSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void StiffarmOffenseSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

