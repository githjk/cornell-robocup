/***********************************************************************************/
//	HEADER FILES
#include "Creator.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/CrossCreaseCreatorSkill.h"
#include "Skillz/MidReboundOffenseSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Creator::crossCreasePass(RobotIndex ID,
                              BasePlay* play,
                              const VisionData& field,
                              RobocupStrategyData* sd)
{
  //===================
  //===================
  //Handle the Creator
  //===================
  //===================

  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "CREATOR Cross Crease Pass");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  CrossCreaseCreatorSkill* skillHandle = 
  (CrossCreaseCreatorSkill*)skills->getSkill(CrossCreaseCreatorSkill::skillNum);

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
  //Handle the Special Op Creator if he exists
  //===================
  //===================
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_CREATOR);
  if(SpecialOpID != NO_ROBOT)
  {
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(SpecialOpID, "SPECIAL OP CREATOR Cross Crease Pass");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on loose defense special Op skill
    //-------------------
    MidReboundOffenseSkill* specialOpSkillHandle = 
    (MidReboundOffenseSkill*)skills->getSkill(MidReboundOffenseSkill::skillNum);

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