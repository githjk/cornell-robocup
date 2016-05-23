//----------------------------------------------------------------------------

/***********************************************************************************/

//	HEADER FILES

#include "Creator.h"                  

#include "RoboCup_modules/RoboCupStrategyModule.h"

#include "Skillz/DoublePickBlockSkill.h"



/***********************************************************************************/

using namespace ScratchVars;



//----------------------------------------------------------------------------

void Creator::doublePickOffense(RobotIndex ID,

                             BasePlay* play,

                             const VisionData& field,

                             RobocupStrategyData* sd)

{
  //Display Message
  sd->setMessage(ID, "CREATOR Double Pick Offense");   

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on double pick offense creator skill
  //-------------------
  DoublePickBlockSkill* skillHandle = 
  (DoublePickBlockSkill*)skills->getSkill(DoublePickBlockSkill::skillNum);

  //-------------------
  //initialize skill if it has not been initialized.
  //-------------------
  if(!skillHandle->isInitialized())
  {
      // if special op exist, push up a little since working in tandem, ow match
      // x-coord of opponent exactly since picking alone
      RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_CREATOR);
      if(SpecialOpID != NO_ROBOT)
        skillHandle->initialize(NO_POSITION,
                                sd->getSystemParams().general.PLAYER_RADIUS);
      else
        skillHandle->initialize(NO_POSITION,
                                0.0f);
  }

  //-------------------
  //run Creator double pick offense skill
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
    sd->setMessage(SpecialOpID, "SPECIAL OP CREATOR double pick offense");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);


    //-------------------
    //get a handle on double pick offense special Op skill
    //-------------------
    DoublePickBlockSkill* specialOpSkillHandle = 
    (DoublePickBlockSkill*)skills->getSkill(DoublePickBlockSkill::skillNum);

    //-------------------
    //initialize skill if it is not initialized
    //-------------------
    if(!specialOpSkillHandle->isInitialized())
    {
  	  //specialOpSkillHandle->initialize();
      specialOpSkillHandle->initialize(CREATOR,0);
    }

    //-------------------
    //run skill
    //-------------------
    specialOpSkillHandle->run();

  }
}

//----------------------------------------------------------------------------