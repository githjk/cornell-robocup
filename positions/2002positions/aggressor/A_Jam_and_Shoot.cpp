/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/JamAndShootSkill.h"
#include "Skillz/BehindScreenOffenseSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::jamAndShoot(RobotIndex ID,
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
  //set default message
  //-------------------
  sd->setMessage(ID, "AGGRESSOR Jam and Shoot");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  JamAndShootSkill* skillHandle = 
  (JamAndShootSkill*)skills->getSkill(JamAndShootSkill::skillNum);

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
  //Handle the Special Op Aggressor if he exists
  //===================
  //===================
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  if(SpecialOpID != NO_ROBOT)
  {
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR Jam and Shoot");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on loose defense special Op skill
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