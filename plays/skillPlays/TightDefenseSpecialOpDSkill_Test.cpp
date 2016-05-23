#include "TightDefenseSpecialOpDSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play TightDefenseSpecialOpDSkillTest::playNum = -1;
//====================================
void TightDefenseSpecialOpDSkillTest::initializePlay(const VisionData& vision, 
                                                   RobocupStrategyData* rsd)
{
  ID = ROBOT0;
 //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the screen skill
  specialOpDSkill = (TightDefenseSpecialOpDSkill*)(skills->getSkill(TightDefenseSpecialOpDSkill::skillNum));

  //Initialize the skill
  specialOpDSkill->initialize();
}
//====================================
ActionFunction TightDefenseSpecialOpDSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void TightDefenseSpecialOpDSkillTest::executePlay(VisionData* vision, 
                                                RobocupStrategyData* rsd)
{
  specialOpDSkill->run();
}
//====================================

