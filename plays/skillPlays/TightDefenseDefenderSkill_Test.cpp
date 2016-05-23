#include "TightDefenseDefenderSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play TightDefenseDefenderSkillTest::playNum = -1;
//====================================
void TightDefenseDefenderSkillTest::initializePlay(const VisionData& vision, 
                                                   RobocupStrategyData* rsd)
{
   ID = ROBOT0;
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the screen skill
  defenderSkill = (TightDefenseDefenderSkill*)(skills->getSkill(TightDefenseDefenderSkill::skillNum));

  //Initialize the skill
  defenderSkill->initialize();
}
//====================================
ActionFunction TightDefenseDefenderSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void TightDefenseDefenderSkillTest::executePlay(VisionData* vision, 
                                                RobocupStrategyData* rsd)
{
  defenderSkill->run();
}
//====================================

