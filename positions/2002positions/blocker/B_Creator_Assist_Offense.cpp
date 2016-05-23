//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemBlockerSkill.h"

/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Blocker::creatorAssistOffense(RobotIndex ID,
                           BasePlay* play,
                           const VisionData& field,
                           RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "BLOCKER Creator Assist Offense");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on creator assist offense blocker skill
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
  //run Blocker Creator Assist Offense skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------