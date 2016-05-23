//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemDefenderSkill.h"

/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Defender::doublePickOffense(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "Double Pick Offense");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on double pick offense defender skill
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
  //run Defender double pick offense skill
  //-------------------
  skillHandle->run();
}
//----------------------------------------------------------------------------