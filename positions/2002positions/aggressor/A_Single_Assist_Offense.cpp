/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/AdvanceBallUpfieldSkill.h"
#include "Skillz/StiffarmOffenseSkill.h"
#include "Skillz/BehindScreenOffenseSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::singleAssistOffense(RobotIndex ID,
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
  sd->setMessage(ID, "AGGRESSOR Single Assist Offense");

  //-------------------
  //get a handle on this robot ID's skillset
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on the tight defense blocker skill
  //-------------------
  AdvanceBallUpfieldSkill* skillHandle = 
  (AdvanceBallUpfieldSkill*)skills->getSkill(AdvanceBallUpfieldSkill::skillNum);

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
    sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR Single Assist Offense");
    
    //-------------------
    //get a handle on skillset for this robot
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //this needs to be fixed to use play parameter
    bool useStiffarmOffenseSkill;
    
    //once aggressor past halfline, use behind screen
    float xLoc;
    if(sd->getRobotByPosition(AGGRESSOR) != NO_ROBOT)
      xLoc = getLocation(sd->getRobotByPosition(AGGRESSOR), field, sd->getSystemParams()).getX();
    else
      xLoc = getRobocupBall(field).getXPos();


    useStiffarmOffenseSkill = xLoc < sd->getSystemParams().field.HALF_LINE;

    if(useStiffarmOffenseSkill)
    {
      //-------------------
      //get a handle on loose defense special Op skill
      //-------------------
      StiffarmOffenseSkill* skillHandle = 
      (StiffarmOffenseSkill*)skills->getSkill(StiffarmOffenseSkill::skillNum);

      //-------------------
      //initialize skill if it is not initialized
      //-------------------
      if(!skillHandle->isInitialized())
    	  skillHandle->initialize();

      //-------------------
      //run skill
      //-------------------
      skillHandle->run();
    }
    else
    {
      //-------------------
      //get a handle on loose defense special Op skill
      //-------------------
      BehindScreenOffenseSkill* skillHandle = 
      (BehindScreenOffenseSkill*)skills->getSkill(BehindScreenOffenseSkill::skillNum);

      //-------------------
      //initialize skill if it is not initialized
      //-------------------
      if(!skillHandle->isInitialized())
    	  skillHandle->initialize();

      //-------------------
      //run skill
      //-------------------
      skillHandle->run();
    }
  }
}
//----------------------------------------------------------------------------