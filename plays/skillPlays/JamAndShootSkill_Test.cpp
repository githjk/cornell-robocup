#include "JamAndShootSkill_test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/JamAndShootSkill.h"
Play JamAndShootSkillTest::playNum = -1;
//====================================
void JamAndShootSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
      SkillSet* skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (JamAndShootSkill*)(skills->getSkill(JamAndShootSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction JamAndShootSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void JamAndShootSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

