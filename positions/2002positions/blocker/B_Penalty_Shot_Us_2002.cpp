/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemBlockerSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Blocker::penaltyShotUs2002(RobotIndex ID,
                                  BasePlay* play,
                                  const VisionData& field,
                                  RobocupStrategyData* sd)
{
  //===================
  //===================
  //Handle the Aggressor
  //===================
  //===================

  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "BLOCKER psUS");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on upfield defense aggressor skill
  //-------------------
  TandemBlockerSkill* skillHandle = 
  (TandemBlockerSkill*)skills->getSkill(TandemBlockerSkill::skillNum);

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