#include "MoveAroundPickSkill_Test.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "Skillz/MoveAroundPickSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include <iostream.h>

#include "GUI/GUI.h"
Play MoveAroundPickSkillPlay::playNum = -1;

void MoveAroundPickSkillPlay::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{

}


ActionFunction MoveAroundPickSkillPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void MoveAroundPickSkillPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  ///Unless we can get the ID of a certain position, assume this skill works on robot 0
  RobotIndex ID = ROBOT0;
    
  //Get a handle on the skill set.  Always useful to have...
  SkillSet* skills = rsd->getStrategyModule().getSkillSet(ID);
  Destination *command = rsd->getDestination(ROBOT0); 
  
  dest.set(rsd->getSystemParams().field.THEIR_GOAL_LINE, 0.0f);

  //Get a handle on the screen skill
  moveAroundPickSkill = (MoveAroundPickSkill*)(skills->getSkill(MoveAroundPickSkill::skillNum));
  
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( moveAroundPickSkill->isValid() ) {
    
    
    ///If the skill is not active, initialize it and activate it.  This 
    ///way we only initialize it once.  (This obviously brings up the question
    ///of how we initialize a skill only once).
    if ( !moveAroundPickSkill->isInitialized() ) 
    {
      // Our ROBOT1 will be the handler, and their ROBOT0 will be the opponent we pick
      moveAroundPickSkill->initialize(ROBOT1, ROBOT0, rsd->getSystemParams().general.TEAM, 
                            rsd->getSystemParams().general.OTHER_TEAM, dest);
    }      
    ///run the skill!
    moveAroundPickSkill->run(); 
    
  }
  
  

}
