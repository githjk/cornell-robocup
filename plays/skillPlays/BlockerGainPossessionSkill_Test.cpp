#include "BlockerGainPossessionSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/JamAndShootSkill.h"
Play BlockerGainPossessionSkillTest::playNum = -1;
//====================================
void BlockerGainPossessionSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);


  //Get a handle on the skill
  skillHandle = (BlockerGainPossessionSkill*)(skills->getSkill(BlockerGainPossessionSkill::skillNum));

  //initialize skill
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();
}

//====================================
ActionFunction BlockerGainPossessionSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void BlockerGainPossessionSkillTest::executePlay(VisionData* vision, 
                                       RobocupStrategyData* rsd)
{
    skillHandle->run();
}
//====================================

