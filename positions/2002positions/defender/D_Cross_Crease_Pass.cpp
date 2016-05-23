/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemDefenderSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Defender::crossCreasePass(RobotIndex ID,
                               BasePlay* play,
                               const VisionData& field,
                               RobocupStrategyData* sd)
{
  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "DEFENDER Cross Crease Pass");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on loose defense defender skill
  //-------------------
  TandemDefenderSkill* skillHandle = 
  (TandemDefenderSkill*)skills->getSkill(TandemDefenderSkill::skillNum);

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