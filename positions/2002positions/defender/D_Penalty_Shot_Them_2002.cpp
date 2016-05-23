/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/CageDefenseSkill.h"
#include "Skillz/SpecialOpCageDefenseSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Defender::penaltyShotThem2002(RobotIndex ID,
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
  //set default message
  //-------------------
  sd->setMessage(ID, "DEFENDER pstRUSH");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  CageDefenseSkill* skillHandle = 
  (CageDefenseSkill*)skills->getSkill(CageDefenseSkill::skillNum);

  //-------------------
  //initialize skill if unitialized
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
    sd->setMessage(SpecialOpID, "SPECIAL OP DEFENDER pstRUSH");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on loose defense special Op skill
    //-------------------
    CageDefenseSkill* specialOpSkillHandle = 
    (CageDefenseSkill*)skills->getSkill(CageDefenseSkill::skillNum);

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