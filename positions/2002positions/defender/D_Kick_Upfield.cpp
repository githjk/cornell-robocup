//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemDefenderSkill.h"
#include "Skillz/LooseDSpecialOpSkill.h"

/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Defender::kickUpfield(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //===================
  //===================
  //Handle the Defender
  //===================
  //===================

  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "DEFENDER Kick Upfield");

  //-------------------
  //get a handle on skillset for this robot
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
    sd->setMessage(SpecialOpID, "SPECIAL OP DEFENDER Kick Upfield");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on loose defense special Op skill
    //-------------------
    LooseDSpecialOpSkill* specialOpSkillHandle = 
    (LooseDSpecialOpSkill*)skills->getSkill(LooseDSpecialOpSkill::skillNum);

    //-------------------
    //initialize skill if it is not initialized
    //-------------------
    if(!specialOpSkillHandle->isInitialized())
  	  specialOpSkillHandle->initialize();

    //-------------------
    //run skill
    //-------------------
    specialOpSkillHandle->run();
  }
}
//----------------------------------------------------------------------------