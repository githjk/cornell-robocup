//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/ThreeManBlockerSkill.h"
#include "Skillz/TandemBlockerSkill.h"

/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Blocker::threeManDefense(RobotIndex ID,
                           BasePlay* play,
                           const VisionData& field,
                           RobocupStrategyData* sd)
{
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  RobotIndex DefenderID = sd->getRobotByPosition(DEFENDER);

  //if the defender and specialop are close to each other, run 3man.
  //Otherwise, run tandem and send the special op over to the defender.

  if(getLocation(DefenderID,field,sd->getSystemParams()).distanceTo(
          getLocation(SpecialOpID,field,sd->getSystemParams())) <= 
        4*sd->getSystemParams().general.PLAYER_RADIUS){

    //Display Message
    sd->setMessage(ID, "BLOCKER Three Man Defense");   

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
      SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

    //-------------------
    //get a handle on loose defense blocker skill
    //-------------------
    ThreeManBlockerSkill* skillHandle = 
    (ThreeManBlockerSkill*)skills->getSkill(ThreeManBlockerSkill::skillNum);

    //-------------------
    //initialize skill if it has not been initialized.
    //-------------------
    if(!skillHandle->isInitialized())
    {
      skillHandle->initialize();
    }
  
    //-------------------
    //run Blocker Loose Defense skill
    //-------------------
    skillHandle->run();
  }else{

    //Display Message
    sd->setMessage(ID, "BLOCKER Three Man Defense");   

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
      SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

    //-------------------
    //get a handle on loose defense blocker skill
    //-------------------
    TandemBlockerSkill* skillHandle = 
    (TandemBlockerSkill*)skills->getSkill(TandemBlockerSkill::skillNum);

    //-------------------
    //initialize skill if it has not been initialized.
    //-------------------
    if(!skillHandle->isInitialized())
    {
      skillHandle->initialize();
    }
  
    //-------------------
    //run Blocker Loose Defense skill
    //-------------------
    skillHandle->run();
  }
}
//----------------------------------------------------------------------------