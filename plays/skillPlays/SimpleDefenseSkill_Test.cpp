#include "SimpleDefenseSkill_Test.h"
#include "Skillz/SimpleDefenseSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"

Play SimpleDefenseSkillTest::playNum = -1;

//====================================
void SimpleDefenseSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Have robot 1 screen
  ID = ROBOT1;

  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the screen skill
  defenseSkill = (SimpleDefenseSkill*)(skills->getSkill(SimpleDefenseSkill::skillNum));

 //Initialize the skill to:
  // friendly robot 0 trying to do line defense
  //-and look toward the ball at all times
  defenseSkill->initialize( 
						  -0.6f
	);
}
//====================================
ActionFunction SimpleDefenseSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void SimpleDefenseSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skills->getSkill(SimpleDefenseSkill::skillNum)->isValid() ) 
  {
    ///Set the message of screening robot
    rsd->setMessage((RobotIndex)0, "Ball not found...");  
      
    ///run the skill
    skills->getSkill(SimpleDefenseSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage((RobotIndex)0, "Simple Defense Skill Not Valid...");  
 }
}
//====================================
