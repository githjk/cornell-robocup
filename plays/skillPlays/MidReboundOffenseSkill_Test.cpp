#include "MidReboundOffenseSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/MidReboundOffenseSkill.h"
Play MidReboundOffenseSkillTest::playNum = -1;
//====================================
void MidReboundOffenseSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (MidReboundOffenseSkill*)(skills->getSkill(MidReboundOffenseSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction MidReboundOffenseSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void MidReboundOffenseSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

