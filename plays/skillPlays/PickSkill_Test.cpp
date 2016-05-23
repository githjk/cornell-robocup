#include "PickSkill_Test.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "Skillz/PickSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include <iostream.h>

#include "GUI/GUI.h"
Play PickSkillPlay::playNum = -1;

void PickSkillPlay::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{

  // Our ROBOT1 will be the handler, and their ROBOT0 will be the opponent we pick
  ID = ROBOT0;
  skills = rsd->getStrategyModule().getSkillSet(ID);
  pickSkill = (PickSkill*)(skills->getSkill(PickSkill::skillNum));    
  pickSkill->initialize(ROBOT1, 
	                    ROBOT0, 
						rsd->getSystemParams().general.TEAM, 
						rsd->getSystemParams().general.OTHER_TEAM, 
                        getBallLocation(vision));

}


ActionFunction PickSkillPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void PickSkillPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  ///Unless we can get the ID of a certain position, assume this skill works on robot 0
  //RobotIndex ID = ROBOT0;
        
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);
  Destination *command = rsd->getDestination(ROBOT0); 
  
  //Get a handle on the screen skill
  //pickSkill = (PickSkill*)(skills->getSkill(PickSkill::skillNum));
  
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( pickSkill->isValid() ) {
    ///run the skill!
    //pickSkill->initialize(ROBOT1, ROBOT0, rsd->getParams().TEAM, rsd->getParams().OTHER_TEAM, 
    //                        getBallLocation(*vision));

    pickSkill->run();
    //rsd->setPassDest(ID,pickSkill->getPickLoc());
    rsd->setPassDest(ID,command->getPos());
  }
  
  
  

  ///The evaluation and updating methods for the SkillSet objects should be called
  ///elsewhere in the AI.  This needs to be implemented.  - EvanK, 10/21/01

}
