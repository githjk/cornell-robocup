#include "CreatorLookForwardPassSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play CreatorLookForwardPassSkillTest::playNum = -1;
//====================================
void CreatorLookForwardPassSkillTest::initializePlay(const VisionData& vision, 
                                                   RobocupStrategyData* rsd)
{
  ID = ROBOT0;

  //Get a handle on the skill set.  Always useful to have...
    skills = rsd->getStrategyModule().getSkillSet(ID);


  //Get a handle on the screen skill
  lookForwardPassSkill = (CreatorLookForwardPassSkill*)(skills->getSkill(CreatorLookForwardPassSkill::skillNum));

  //Initialize the skill
  lookForwardPassSkill->initialize();
}
//====================================
ActionFunction CreatorLookForwardPassSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void CreatorLookForwardPassSkillTest::executePlay(VisionData* vision, 
                                                  RobocupStrategyData* rsd)
{
  lookForwardPassSkill->run();
}
//====================================

