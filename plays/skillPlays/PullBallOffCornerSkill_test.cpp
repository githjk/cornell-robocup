#include "PullBallOffCornerSkill_Test.h"
#include "Skillz/PullBallOffCornerSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play PullBallOffCornerSkillTest::playNum=-1;
//====================================
void PullBallOffCornerSkillTest::initializePlay(const VisionData& vision, 
                                                RobocupStrategyData* rsd)
{
  //Have robot0 try to get the ball
  ID = ROBOT0;
      
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the PullBallOffWall skill
  pullBallOffCornerSkill = (PullBallOffCornerSkill*)(skills->getSkill(PullBallOffCornerSkill::skillNum));

  //Just initialize the skill
  pullBallOffCornerSkill->initialize();
}
//====================================
ActionFunction PullBallOffCornerSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void PullBallOffCornerSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skills->getSkill(PullBallOffCornerSkill::skillNum)->isValid()) 
  {      
    ///run the skill
    skills->getSkill(PullBallOffCornerSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage((RobotIndex)0, "PullBallOffCorner Not Valid..."); 
  }
}
//====================================
