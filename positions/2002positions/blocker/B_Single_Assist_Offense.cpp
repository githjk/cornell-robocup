/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemBlockerSkill.h"
#include "common/util/info/ball/ball.h"

/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Blocker::singleAssistOffense(RobotIndex ID,
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
    //set default message
    //-------------------
    sd->setMessage(ID, "BLOCKER Single Assist Offense");

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
}
//----------------------------------------------------------------------------