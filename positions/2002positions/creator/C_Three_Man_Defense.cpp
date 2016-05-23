/***********************************************************************************/
//	HEADER FILES
#include "Creator.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/CreatorLookForwardPassSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Creator::threeManDefense(RobotIndex ID,
                           BasePlay* play,
                           const VisionData& field,
                           RobocupStrategyData* sd)
{
  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "CREATOR Three Man Defense");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on creator look forward pass skill
  //-------------------
  CreatorLookForwardPassSkill* skillHandle = 
  (CreatorLookForwardPassSkill*)skills->getSkill(CreatorLookForwardPassSkill::skillNum);

  //-------------------
  //initialize skill if it is not initialized
  //-------------------
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();

  //-------------------
  //run skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------
