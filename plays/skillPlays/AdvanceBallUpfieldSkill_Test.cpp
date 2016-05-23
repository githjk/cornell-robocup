#include "AdvanceBallUpfieldSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/AcquirePossessionSkill.h"
Play AdvanceBallUpfieldSkillTest::playNum = -1;
//====================================
void AdvanceBallUpfieldSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (AdvanceBallUpfieldSkill*)(skills->getSkill(AdvanceBallUpfieldSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction AdvanceBallUpfieldSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void AdvanceBallUpfieldSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

