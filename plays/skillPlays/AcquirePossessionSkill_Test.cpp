#include "AcquirePossessionSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/AcquirePossessionSkill.h"
Play AcquirePossessionSkillTest::playNum = -1;
//====================================

void AcquirePossessionSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (AcquirePossessionSkill*)(skills->getSkill(AcquirePossessionSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction AcquirePossessionSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void AcquirePossessionSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

