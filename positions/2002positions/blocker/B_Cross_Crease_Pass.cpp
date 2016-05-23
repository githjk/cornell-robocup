/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemBlockerSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Blocker::crossCreasePass(RobotIndex ID,
                              BasePlay* play,
                              const VisionData& field,
                              RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "BLOCKER Cross Crease Pass");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  TandemBlockerSkill* skillHandle = 
  (TandemBlockerSkill*)skills->getSkill(TandemBlockerSkill::skillNum);

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