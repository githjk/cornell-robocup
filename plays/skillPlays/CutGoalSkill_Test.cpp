#include "CutGoalSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/CutGoalSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
Play CutGoalSkillTest::playNum = -1;
//====================================
void CutGoalSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Have robot 1 screen
  ID = ROBOT1;
      
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);


  //Get a handle on the screen skill
  cutGoalSkill = (CutGoalSkill*)(skills->getSkill(CutGoalSkill::skillNum));
  //Initialize the skill for the goalie or the defender


  float y1;
  float y2;
  y1=rsd->getSystemParams().field.GOAL_WIDTH/2;
  y2=-y1;

  float UPPER_LIMIT = 0.0f;
  float LOWER_LIMIT = rsd->getSystemParams().field.OUR_GOAL_LINE;

  cutGoalSkill->initialize(y1, y2, UPPER_LIMIT, LOWER_LIMIT) ;
}
//====================================
ActionFunction CutGoalSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void CutGoalSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skills->getSkill(CutGoalSkill::skillNum)->isValid() ) 
  {
    ///Set the message of screening robot
    rsd->setMessage((RobotIndex)0, "Ball not found...");  
      
    ///run the skill
    skills->getSkill(CutGoalSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage((RobotIndex)ID, "Cut Goal Skill Not Valid...");  
      
  }
}
//====================================