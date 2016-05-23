//----------------------------------------------------------------------------

/***********************************************************************************/

//	HEADER FILES

#include "Creator.h"                  

#include "RoboCup_modules/RoboCupStrategyModule.h"

#include "Skillz/CreatorLookForwardPassSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Creator::removeBallDefense(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "CREATOR remove Ball Defense");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on remove ball defense creator skill
  //-------------------
  CreatorLookForwardPassSkill* skillHandle = 
  (CreatorLookForwardPassSkill*)skills->getSkill(CreatorLookForwardPassSkill::skillNum);

  //-------------------
  //initialize skill if it has not been initialized.
  //-------------------
  //indiciate skill to look for a chip kick on opposite side of the field as the ball
  if(!skillHandle->isInitialized())
  {
    skillHandle->initialize();
  }
  
  //-------------------
  //run Creator remove ball defense skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------