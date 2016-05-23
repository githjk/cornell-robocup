#include "AggressorCageDefenseSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/CageDefenseSkill.h"
Play AggressorCageDefenseSkillTest::playNum = -1;
//====================================
void AggressorCageDefenseSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (CageDefenseSkill*)(skills->getSkill(CageDefenseSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction AggressorCageDefenseSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void AggressorCageDefenseSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

