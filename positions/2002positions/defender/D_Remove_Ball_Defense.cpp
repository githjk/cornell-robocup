/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/StiffarmOffenseSkill.h"
#include "Skillz/RemoveBallHelpSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Defender::removeBallDefense(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "DEFENDER Remove Ball Defense");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on remove Ball defense defender skill
  //-------------------
  RemoveBallHelpSkill* skillHandle = 
  (RemoveBallHelpSkill*)skills->getSkill(RemoveBallHelpSkill::skillNum);

  //-------------------
  //initialize skill if it has not been initialized.
  //-------------------
  if(!skillHandle->isInitialized())
  {
    skillHandle->initialize(); //play on opposite side of field than ball
  }
  
  //-------------------
  //run Defender remove ball defense skill
  //-------------------
  skillHandle->run();
  //===================

  
  //===================
  //Handle the Special Op Defender if he exists
  //===================
  //===================
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  if(SpecialOpID != NO_ROBOT)
  {
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(SpecialOpID, "SPECIAL OP DEFENDER Remove Ball Defense");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on remove ball defense special Op skill
    //-------------------
    StiffarmOffenseSkill* specialOpSkillHandle = 
      (StiffarmOffenseSkill*)skills->getSkill(StiffarmOffenseSkill::skillNum);

    //-------------------
    //initialize skill if it is not initialized
    //-------------------
    if(!specialOpSkillHandle->isInitialized())
      specialOpSkillHandle->initialize(BLOCKER); //do stiffarm for blocker

    //-------------------
    //run skill
    //-------------------
    specialOpSkillHandle->run();
  }

}
//----------------------------------------------------------------------------