/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemDefenderSkill.h"
#include "common/util/info/ball/ball.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Defender::singleAssistOffense(RobotIndex ID,
                                   BasePlay* play,
                                   const VisionData& field,
                                   RobocupStrategyData* sd)
{
  Pair ballPos = getBallLocation(field);

  //run tight defense skill
  if(
       (ballPos.getX() < (sd->getSystemParams().field.DEFENSE_ZONE_LINE - sd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)) &&
	     ((ballPos.getY() > (sd->getSystemParams().field.LEFT_MIDDLE_SECTOR + sd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)) ||
	     (ballPos.getY() < (sd->getSystemParams().field.RIGHT_MIDDLE_SECTOR - sd->getSystemParams().general.PLAY_TRANSITION_HISTORISIS)))
    )
    tightDefense(ID, play, field, sd);
  else
  {
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(ID, "DEFENDER Single Assist Offense");

    //-------------------  //get a handle on skillset for this robot
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
}
//----------------------------------------------------------------------------

