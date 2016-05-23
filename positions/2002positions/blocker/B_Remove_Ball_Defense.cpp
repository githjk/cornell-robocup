/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/RemoveBallSkill.h"

/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Blocker::removeBallDefense(RobotIndex ID,
                           BasePlay* play,
                           const VisionData& field,
                           RobocupStrategyData* sd)
{
  sd->setMessage(ID, "BLOCKER Remove Ball Defense");   

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the remove ball defense blocker skill
  //-------------------
  RemoveBallSkill* skillHandle = 
   (RemoveBallSkill*)skills->getSkill(RemoveBallSkill::skillNum);

  //-------------------
  //initialize skill if it has not been initialized.
  //-------------------
  if(!skillHandle->isInitialized())
  {
    skillHandle->initialize();
  }
  
  //-------------------
  //run Blocker remove ball Defense skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------