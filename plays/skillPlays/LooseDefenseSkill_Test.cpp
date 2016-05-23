#include "LooseDefenseSkill_Test.h"
#include "Skillz/TandemDefenderSkill.h"
#include "Skillz/TandemBlockerSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play LooseDefenseSkillTest::playNum = -1;
//====================================
void LooseDefenseSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Have robot 0 be goalie
  ID = ROBOT0;
      
  //Have robot 1 be defender
  defenderID = ROBOT1;
  //Get a handle on the skill set.  Always useful to have...
  skillsdefender = rsd->getStrategyModule().getSkillSet(defenderID);
  skillsblocker = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the TandemDefender skill
  tandemDefenderSkill = (TandemDefenderSkill*)(skillsdefender->getSkill(TandemDefenderSkill::skillNum));
  
  //Get a handle on the TandemBlocker skill
  tandemBlockerSkill = (TandemBlockerSkill*)(skillsblocker->getSkill(TandemBlockerSkill::skillNum));
  //Initialize the skill for the goalie or the defender

  tandemDefenderSkill->initialize() ;
  tandemBlockerSkill->initialize();
}
//====================================
ActionFunction LooseDefenseSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void LooseDefenseSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skillsdefender->getSkill(TandemDefenderSkill::skillNum)->isValid() ) 
  {
      
    ///run the skill
    skillsdefender->getSkill(TandemDefenderSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage((RobotIndex)defenderID, "TandemDefender Skill Not Valid...");  
      
  }


    if( skillsblocker->getSkill(TandemBlockerSkill::skillNum)->isValid() ) 
  {
      
    ///run the skill
    skillsblocker->getSkill(TandemBlockerSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage((RobotIndex)ID, "TandemBlocker Skill Not Valid...");  
      
  }



}
//====================================