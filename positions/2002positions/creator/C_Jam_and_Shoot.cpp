/***********************************************************************************/
//	HEADER FILES
#include "Creator.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/OppositeReboundOffenseSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Creator::jamAndShoot(RobotIndex ID,
                          BasePlay* play,
                          const VisionData& field,
                          RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "CREATOR Jam and Shoot");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  OppositeReboundOffenseSkill* skillHandle = 
  (OppositeReboundOffenseSkill*)skills->getSkill(OppositeReboundOffenseSkill::skillNum);

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