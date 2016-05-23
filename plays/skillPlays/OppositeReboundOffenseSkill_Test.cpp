#include "OppositeReboundOffenseSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/OppositeReboundOffenseSkill.h"
Play OppositeReboundOffenseSkillTest::playNum = -1;
//====================================
void OppositeReboundOffenseSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (OppositeReboundOffenseSkill*)(skills->getSkill(OppositeReboundOffenseSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction OppositeReboundOffenseSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void OppositeReboundOffenseSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

