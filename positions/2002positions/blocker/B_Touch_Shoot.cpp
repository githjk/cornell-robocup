//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemBlockerSkill.h"

/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Blocker::touchShoot(RobotIndex ID,
                         BasePlay* play,
                         const VisionData& field,
                         RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "BLOCKER Touch Shoot");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);
  //-------------------
  //get a handle on touch shoot blocker skill
  //-------------------
  TandemBlockerSkill* skillHandle = 
  (TandemBlockerSkill*)skills->getSkill(TandemBlockerSkill::skillNum);

  //-------------------
  //initialize skill if it has not been initialized.
  //-------------------
  if(!skillHandle->isInitialized())
  {
    skillHandle->initialize();
  }
  
  //-------------------
  //run Blocker touch shoot skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------