/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/BlockerGainPossessionSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Blocker::cageDefense(RobotIndex ID,
                          BasePlay* play,
                          const VisionData& field,
                          RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "BLOCKER Cage Defense");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  BlockerGainPossessionSkill* skillHandle = 
  (BlockerGainPossessionSkill*)skills->getSkill(BlockerGainPossessionSkill::skillNum);

  //-------------------
  //initialize skill if unitialized
  //-------------------
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();

  //-------------------
  //run skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------