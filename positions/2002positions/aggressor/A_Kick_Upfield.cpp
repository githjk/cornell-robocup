/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/SimpleKickSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::kickUpfield(RobotIndex ID,
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
  sd->setMessage(ID, "AGGRESSOR kick upfield");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on upfield defense aggressor skill
  //-------------------
  SimpleKickSkill* skillHandle = 
  (SimpleKickSkill*)skills->getSkill(SimpleKickSkill::skillNum);

  //-------------------
  //initialize skill if it is not initialized
  //-------------------
  if(!skillHandle->isInitialized())
	  skillHandle->initialize(KICK_PASS);

  //-------------------
  //run skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------