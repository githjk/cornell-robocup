#include "ReceivePassSkill_Test.h"
#include "Skillz/ReceivePassSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
Play ReceivePassSkillTest::playNum = -1;
//====================================
void ReceivePassSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Have robot 1 screen
  ID = ROBOT1;
      
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the Receive skill
  screenSkill = (ReceivePassSkill*)(skills->getSkill(ReceivePassSkill::skillNum));

  //Initialize the skill to screen points A, B, look to C, and grace factor
  
  //Initialize the skill to:
  //-screen opponent robot 0
  //-from robot friendly robot 0
  //-and look toward the ball at all times
  //-with grace distance 0.2 meters
  screenSkill->initialize();
    
}
//====================================
ActionFunction ReceivePassSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void ReceivePassSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  //screenSkill->initialize();
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skills->getSkill(ReceivePassSkill::skillNum)->isValid() ) 
  {
    ///Set the message of screening robot
    //rsd->setMessage((RobotIndex)0, "Being scared...");  
      
    ///run the skill
    rsd->setMessage((RobotIndex)1, "Receive Crease Pass  Valid...");
    skills->getSkill(ReceivePassSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage((RobotIndex)1, "Receive Crease Pass Not Valid...");  
      
  }
}
//====================================
