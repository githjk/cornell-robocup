/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/AcquirePossessionSkill.h"
#include "Skillz/SupplementThreeManSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::threeManDefense(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "AGGRESSOR Three Man Defense");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on Agressor Recover Ball skill
  //-------------------
//  Skill* skillHandle = skills->getSkill(AcquirePossessionSkill::skillNum);
  Skill* skillHandle = skills->getSkill(SupplementThreeManSkill::skillNum);
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
