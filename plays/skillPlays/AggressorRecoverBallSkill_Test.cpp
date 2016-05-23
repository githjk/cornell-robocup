#include "AggressorRecoverBallSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play AggressorRecoverBallSkillTest::playNum = -1;
//====================================
void AggressorRecoverBallSkillTest::initializePlay(const VisionData& vision, 
                                                   RobocupStrategyData* rsd)
{
  ID = ROBOT0;

  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getParams().RCStrategyModule->getSkillSet(ID);
  //Get a handle on the screen skill
  recoverBallSkill = (AggressorRecoverBallSkill*)(skills->getSkill(AggressorRecoverBallSkill::skillNum));
  //Initialize the skill
  recoverBallSkill->initialize();
}
//====================================
ActionFunction AggressorRecoverBallSkillTest::getActionFunction(Position pos)
{
	switch(pos)
	{
  	default: return stopRobot;
	}
}
//====================================
void AggressorRecoverBallSkillTest::executePlay(VisionData* vision, 
                                                RobocupStrategyData* rsd)
{
  recoverBallSkill->run();
}
//====================================


