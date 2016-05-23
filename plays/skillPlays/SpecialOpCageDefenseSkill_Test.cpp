#include "SpecialOpCageDefenseSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/SpecialOpCageDefenseSkill.h"
Play SpecialOpCageDefenseSkillTest::playNum = -1;
//====================================
void SpecialOpCageDefenseSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (SpecialOpCageDefenseSkill*)(skills->getSkill(SpecialOpCageDefenseSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction SpecialOpCageDefenseSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void SpecialOpCageDefenseSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

