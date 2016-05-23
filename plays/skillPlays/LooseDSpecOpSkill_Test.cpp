#include "LooseDSpecOpSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include <iostream.h>
#include "GUI/GUI.h"

Play LooseDSpecOpTest::playNum = -1;



void LooseDSpecOpTest::initializePlay(const VisionData& vision, 

                                           RobocupStrategyData* rsd)

{



  // Our ROBOT1 will be the handler, and their ROBOT0 will be the opponent we pick

  ID = ROBOT0;

  skills = rsd->getStrategyModule().getSkillSet(ID);

  skill = (LooseDSpecialOpSkill*)(skills->getSkill(LooseDSpecialOpSkill::skillNum));    

  skill->initialize();



}





ActionFunction LooseDSpecOpTest::getActionFunction(Position pos)

{
  return stopRobot;
}



void LooseDSpecOpTest::executePlay(VisionData* vision, RobocupStrategyData* rsd) 

{

  ///Unless we can get the ID of a certain position, assume this skill works on robot 0

  //RobotIndex ID = ROBOT0;

        

  //Get a handle on the skill set.  Always useful to have...

  Destination *command = rsd->getDestination(ROBOT0); 

  

  

  ///Always check to see if the skill is valid.  If it isn't, we would make some

  ///high-level decision about what to do.

  if( skill->isValid() ) {

    ///run the skill!    

    skill->run();

    rsd->setPassDest(ID,command->getPos());

  }

  



}

