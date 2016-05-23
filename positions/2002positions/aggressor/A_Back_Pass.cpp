/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/CrossCreasePassSkill.h"
#include "Skillz/BackPassSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::backPass(RobotIndex ID,
                         BasePlay* play,
                         const VisionData& field,
                         RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "AGGRESSOR Back Pass");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  CrossCreasePassSkill* skillHandle = 
  (CrossCreasePassSkill*)skills->getSkill(CrossCreasePassSkill::skillNum);

  //-------------------
  //initialize skill if unitialized
  //-------------------
  if(!skillHandle->isInitialized())
	  skillHandle->initialize(SPECIAL_OP_AGGRESSOR);

  //-------------------
  //run skill
  //-------------------
  skillHandle->run();

  //===================
  //===================
  //Handle the Special op aggressor
  //===================
  //===================
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  if(SpecialOpID != NO_ROBOT){
    //-------------------
    //set default message
    //-------------------
    sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR Back Pass");

    //-------------------
    //get a handle on this robot ID's skillset
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on the tight defense blocker skill
    //-------------------
    BackPassSkill* skillHandle = 
    (BackPassSkill*)skills->getSkill(BackPassSkill::skillNum);

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