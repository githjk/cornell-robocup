#include "LookForChipKickSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/JamAndShootSkill.h"
Play LookForChipKickSkillTest::playNum = -1;
//====================================
void LookForChipKickSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
    SkillSet* skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (LookForChipKickSkill*)(skills->getSkill(LookForChipKickSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize(true);
}

//====================================
ActionFunction LookForChipKickSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void LookForChipKickSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

