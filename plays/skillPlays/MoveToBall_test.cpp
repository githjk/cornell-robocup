#include "MoveToBall_test.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "Skillz/MoveToBallSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include <iostream.h>

#include "GUI/GUI.h"
Play MoveToBallPlay::playNum = -1;

void MoveToBallPlay::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{

}


ActionFunction MoveToBallPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void MoveToBallPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  ///Unless we can get the ID of a certain position, assume this skill works on robot 0
  RobotIndex ID = rsd->getRobotByPosition(AGGRESSOR);   
  if(ID == NO_ROBOT)
    ID = ROBOT0;

  //Get a handle on the skill set.  Always useful to have...
  SkillSet* skills = rsd->getStrategyModule().getSkillSet(ID);
  moveToBallSkill = (MoveToBallSkill*)(skills->getSkill(MoveToBallSkill::skillNum));
  if (!moveToBallSkill->isInitialized()) 
  {
    moveToBallSkill->initialize(false);
  }      
  moveToBallSkill->run();
 
}
