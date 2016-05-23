/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TightDefenseBlockerSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Blocker::tightDefense(RobotIndex ID,
                           BasePlay* play,
                           const VisionData& field,
                           RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "BLOCKER Tight Defense");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  TightDefenseBlockerSkill* skillHandle = 
  (TightDefenseBlockerSkill*)skills->getSkill(TightDefenseBlockerSkill::skillNum);

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