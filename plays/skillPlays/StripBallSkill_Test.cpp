#include "info/robot/robot.h"
#include "math/dist.h"
#include "StripBallSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include <iostream.h>

#include "GUI/GUI.h"
Play StripBallSkillPlay::playNum = -1;

void StripBallSkillPlay::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{

}


ActionFunction StripBallSkillPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void StripBallSkillPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  ///Unless we can get the ID of a certain position, assume this skill works on robot 0
  RobotIndex ID = ROBOT0;
        
  //Get a handle on the skill set.  Always useful to have...
  SkillSet* skills = rsd->getStrategyModule().getSkillSet(ID); 
  Destination *command = rsd->getDestination(ROBOT0); 
  
  //Get a handle on the screen skill
  stripBallSkill = (StripBallSkill*)(skills->getSkill(StripBallSkill::skillNum));

  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( stripBallSkill->isValid() ) {
     ///If the skill is not active, initialize it and activate it.  This 
    ///way we only initialize it once.  (This obviously brings up the question
    ///of how we initialize a skill only once).
    if ( !stripBallSkill->isInitialized() ) 
    {
      // Our ROBOT1 will be the handler, and their ROBOT0 will be the opponent we pick
      stripBallSkill->initialize();
    }      
    ///run the skill!
    stripBallSkill->run();
    //rsd->setPassDest(ID,pickSkill->getPickLoc());
  }
  
  
  

  ///The evaluation and updating methods for the SkillSet objects should be called
  ///elsewhere in the AI.  This needs to be implemented.  - EvanK, 10/21/01

}
