/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/PenaltyShotUsAimAndShootSkill.h"
#include "Skillz/BehindScreenOffenseSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::penaltyShotUsPivot2002(RobotIndex ID,
                                       BasePlay* play,
                                       const VisionData& field,
                                       RobocupStrategyData* sd)
{
  //===================
  //===================
  //Handle the Aggressor
  //===================
  //===================

  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "AGGRESSOR psUS");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on upfield defense aggressor skill
  //-------------------
  PenaltyShotUsAimAndShootSkill* skillHandle = 
  (PenaltyShotUsAimAndShootSkill*)skills->getSkill(PenaltyShotUsAimAndShootSkill::skillNum);

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
  //Handle the Special Op Aggressor if he exists
  //===================
  //===================
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  if(SpecialOpID != NO_ROBOT)
  {
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR psUS");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on upfield defense special Op skill
    //-------------------
    BehindScreenOffenseSkill* specialOpSkillHandle = 
    (BehindScreenOffenseSkill*)skills->getSkill(BehindScreenOffenseSkill::skillNum);

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