//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Creator.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/CreatorCreateSkill.h"
#include "Skillz/SpecialOpBlockSkill.h"

/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Creator::creatorAssistOffense(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "CREATOR Assist Offense");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on assist offense creator skill
  //-------------------
  CreatorCreateSkill* skillHandle = 
  (CreatorCreateSkill*)skills->getSkill(CreatorCreateSkill::skillNum);

  //-------------------
  //initialize skill if it has not been initialized.
  //-------------------
  if(!skillHandle->isInitialized())
  {
    skillHandle->initialize();
  }
  
  //-------------------
  //run Creator assist offense skill
  //-------------------
  skillHandle->run();


  //===================
  //===================
  //Handle the Special Op Creator if he exists
  //===================
  //===================
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_CREATOR);
  if(SpecialOpID != NO_ROBOT)
  {
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(SpecialOpID, "SPECIAL OP CREATOR creator assist offense");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);
  
    //-------------------
    //get a handle on creator assist offense special Op skill
    //-------------------
   SpecialOpBlockSkill* specialOpSkillHandle = 
   (SpecialOpBlockSkill*)skills->getSkill(SpecialOpBlockSkill::skillNum);
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