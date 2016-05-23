//	HEADER FILES
#include "Creator.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/CreatorLookForwardPassSkill.h"

using namespace ScratchVars;

//----------------------------------------------------------------------------
void Creator::kickUpfield(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "CREATOR kickUpfield");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on upfield defense creator skill
  //-------------------
  CreatorLookForwardPassSkill* skillHandle = 
  (CreatorLookForwardPassSkill*)skills->getSkill(CreatorLookForwardPassSkill::skillNum);

  //-------------------
  //initialize skill if it has not been initialized.
  //-------------------
  if(!skillHandle->isInitialized())
  {
    skillHandle->initialize();
  }
  
  //-------------------
  //run Creator upfield defense skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------