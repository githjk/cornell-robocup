#include "SimpleScreenSkill_Test.h"
#include "Skillz/SimpleScreenSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
Play SimpleScreenSkillTest::playNum = -1;
//====================================
void SimpleScreenSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Have robot 1 screen
  ID = ROBOT1;
      
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the screen skill
  screenSkill = (SimpleScreenSkill*)(skills->getSkill(SimpleScreenSkill::skillNum));

  //Initialize the skill to screen points A, B, look to C, and grace factor
  
  //Initialize the skill to:
  //-screen opponent robot 0
  //-from robot friendly robot 0
  //-and look toward the ball at all times
  //-with grace distance 0.2 meters
  screenSkill->initialize(SCREEN_OPPONENT_ROBOT, 
                          0, 
                          SCREEN_FRIENDLY_ROBOT, 
                          0, 
                          SCREEN_BALL, 
                          0,
                          rsd->getSystemParams().general.OBSTACLE_CLEARANCE);
    
}
//====================================
ActionFunction SimpleScreenSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void SimpleScreenSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skills->getSkill(SimpleScreenSkill::skillNum)->isValid() ) 
  {
    ///Set the message of screening robot
    rsd->setMessage((RobotIndex)0, "Being scared...");  
      
    ///run the skill
    skills->getSkill(SimpleScreenSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage((RobotIndex)0, "Simple Screen Not Valid...");  
    rsd->setMessage((RobotIndex)1, "Simple Screen Not Valid...");  
  }
}
//====================================
