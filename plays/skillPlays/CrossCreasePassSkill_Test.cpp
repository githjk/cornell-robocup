#include "CrossCreasePassSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/JamAndShootSkill.h"
Play CrossCreasePassSkillTest::playNum = -1;
//====================================
void CrossCreasePassSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the skill
  skillHandle = (CrossCreasePassSkill*)(skills->getSkill(CrossCreasePassSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction CrossCreasePassSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void CrossCreasePassSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

