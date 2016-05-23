#include "TightDefenseBlockerSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play TightDefenseBlockerSkillTest::playNum = -1;
//====================================
void TightDefenseBlockerSkillTest::initializePlay(const VisionData& vision, 
                                                   RobocupStrategyData* rsd)
{
   ID = ROBOT0;
  //Get a handle on the skill set.  Always useful to have...
    skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the screen skill
  blockerSkill = (TightDefenseBlockerSkill*)(skills->getSkill(TightDefenseBlockerSkill::skillNum));

  //Initialize the skill
  blockerSkill->initialize();
}
//====================================
ActionFunction TightDefenseBlockerSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void TightDefenseBlockerSkillTest::executePlay(VisionData* vision, 
                                                RobocupStrategyData* rsd)
{
  blockerSkill->run();
}
//====================================

