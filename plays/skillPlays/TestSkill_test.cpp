#include "TestSkill_test.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "Skillz/TestSkill.h"
#include <iostream.h>
#include "Skillz/TestSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"

#include "GUI/GUI.h"
Play TestSkillPlay::playNum = -1;

void TestSkillPlay::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{

}


ActionFunction TestSkillPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void TestSkillPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  ///Unless we can get the ID of a certain position, assume this skill works on robot 0
  RobotIndex ID = ROBOT0;

  ///Set the message so we know what's happening.
  rsd->setMessage(ID, "Test Skill");  
      
  //Get a handle on the skill set.  Always useful to have...
  SkillSet*   skills = rsd->getStrategyModule().getSkillSet(ID);
    
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skills->getSkill(TestSkill::skillNum)->isValid() ) {
     ///If the skill is not active, initialize it and activate it.  This 
    ///way we only initialize it once.  (This obviously brings up the question
    ///of how we initialize a skill only once).
    if ( !skills->getSkill(TestSkill::skillNum)->isInitialized() ) 
    {
      skills->getSkill(TestSkill::skillNum)->initialize();
    }
      
    ///run the skill!
    //printf("TestSkillNum = %d\n",TestSkill::skillNum);
    //printf("SimpleScreenSkillNum = %d\n",SimpleScreenSkill::skillNum);
    //printf("SpinAroundBallSkillNum = %d\n",SpinAroundBallSkill::skillNum);
    
    skills->getSkill(TestSkill::skillNum)->run();
  }
    
    
  ///The evaluation and updating methods for the SkillSet objects should be called
  ///elsewhere in the AI.  This needs to be implemented.  - EvanK, 10/21/01

}
