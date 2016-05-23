/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/RemoveBallSetupSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::removeBallDefense(RobotIndex ID,
                            BasePlay* play,
                            const VisionData& field,
                            RobocupStrategyData* sd)
{
  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "AGGRESSOR remove Ball Defense");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on remove Ball defense aggressor skill
  //-------------------
  RemoveBallSetupSkill* skillHandle = 
  (RemoveBallSetupSkill*)skills->getSkill(RemoveBallSetupSkill::skillNum);

  //-------------------
  //initialize skill if it is not initialized
  //-------------------
  //indiciate skill to look for a chip kick on same side of the field as the ball
  if(!skillHandle->isInitialized())
	  skillHandle->initialize(); 

  //-------------------
  //run skill
  //-------------------
  skillHandle->run();
}