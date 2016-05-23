//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemDefenderSkill.h"

/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Defender::touchShoot(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "DEFENDER Touch Shoot");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on touch shoot defender skill
  //-------------------
  TandemDefenderSkill* skillHandle = 
  (TandemDefenderSkill*)skills->getSkill(TandemDefenderSkill::skillNum);

  //-------------------
  //initialize skill if it has not been initialized.
  //-------------------
  if(!skillHandle->isInitialized())
  {
    skillHandle->initialize();
  }
  
  //-------------------
  //run Defender touch shoot skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------