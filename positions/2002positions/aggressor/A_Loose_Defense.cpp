/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/AcquirePossessionSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::looseDefense(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "AGGRESSOR Loose Defense");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on Agressor Recover Ball skill
  //-------------------
  AcquirePossessionSkill* skillHandle = 
  (AcquirePossessionSkill*)skills->getSkill(AcquirePossessionSkill::skillNum);

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
