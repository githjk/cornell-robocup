#include "PullBallOffWallSkill_Test.h"
#include "Skillz/PullBallOffWallSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
Play PullBallOffWallSkillTest::playNum = -1;
//====================================
void PullBallOffWallSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Have robot0 try to get the ball
  ID = ROBOT0;
      
  //Get a handle on the skill set.  Always useful to have...
    skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the PullBallOffWall skill
  pullBallOffWallSkill = (PullBallOffWallSkill*)(skills->getSkill(PullBallOffWallSkill::skillNum));

  //Just initialize the skill
  pullBallOffWallSkill->initialize();
}
//====================================
ActionFunction PullBallOffWallSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void PullBallOffWallSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skills->getSkill(PullBallOffWallSkill::skillNum)->isValid()) 
  {
    
      
  ///run the skill
  if(!skills->getSkill(PullBallOffWallSkill::skillNum)->isInitialized()){
    skills->getSkill(PullBallOffWallSkill::skillNum)->initialize();
  }

    skills->getSkill(PullBallOffWallSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage((RobotIndex)0, "PullBallOffWall Not Valid..."); 
  }
}
//====================================
