/***********************************************************************************/
//	HEADER FILES
#include "Creator.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/LookForChipKickSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Creator::penaltyShotThem2002 (RobotIndex ID,
                                   BasePlay* play,
                                   const VisionData& field,
                                   RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "CREATOR pstD LEFT");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  LookForChipKickSkill* skillHandle = 
  (LookForChipKickSkill*)skills->getSkill(LookForChipKickSkill::skillNum);

  //-------------------
  //initialize skill if unitialized
  //-------------------
  //look for a chip kick on opposite side of the field then the ball
  if(!skillHandle->isInitialized())
	  skillHandle->initialize(false);

  //-------------------
  //run skill
  //-------------------
  //setup on the opposite size of the field
  //aka, pass in setupOnLeft=true if the ball is on the right side line
  skillHandle->run();
}
//----------------------------------------------------------------------------