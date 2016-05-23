/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/ThreeManSideSkill.h"
#include "Skillz/TandemDefenderSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Defender::threeManDefense(RobotIndex ID,
                            BasePlay* play,
                            const VisionData& field,
                            RobocupStrategyData* sd)
{
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_DEFENDER);

  //if the defender and specialop are close to each other, run 3man.
  //Otherwise, run tandem and send the special op over to the defender.

  if(getLocation(ID,field,sd->getSystemParams()).distanceTo(
          getLocation(SpecialOpID,field,sd->getSystemParams())) <= 
    4*sd->getSystemParams().general.PLAYER_RADIUS){
    //===================
    //===================
    //Handle the Defender
    //===================
    //===================
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(ID, "DEFENDER Three Man Defense");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
      SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

    //-------------------
    //get a handle on loose defense defender skill
    //-------------------
    ThreeManSideSkill* skillHandle = 
    (ThreeManSideSkill*)skills->getSkill(ThreeManSideSkill::skillNum);

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
    if(SpecialOpID != NO_ROBOT)
    {
      //-------------------
      //Display message
      //-------------------
      sd->setMessage(SpecialOpID, "SPECIAL OP DEFENDER Three Man Defense");

      //-------------------
      //get a handle on skillset for this robot
      //-------------------
      SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

      //-------------------
      //get a handle on loose defense special Op skill
      //-------------------
      ThreeManSideSkill* specialOpSkillHandle = 
      (ThreeManSideSkill*)skills->getSkill(ThreeManSideSkill::skillNum);

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
  }else{
    //===================
    //===================
    //Handle the Defender
    //===================
    //===================
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(ID, "DEFENDER Three Man Defense");

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
    if(SpecialOpID != NO_ROBOT)
    {
      //-------------------
      //Display message
      //-------------------
      sd->setMessage(SpecialOpID, "SPECIAL OP DEFENDER Running back.");
      
      sd->getDestination(SpecialOpID)->setXPos(sd->getDestination(ID)->getXPos());
      //run to defender's side
      if(sd->getDefensiveBallLocation().getY() > sd->getSystemParams().field.SPLIT_LINE){
        sd->getDestination(SpecialOpID)->setYPos(sd->getDestination(ID)->getYPos() - 
          2*sd->getSystemParams().general.PLAYER_RADIUS);
      }else{
        sd->getDestination(SpecialOpID)->setYPos(sd->getDestination(ID)->getYPos() + 
          2*sd->getSystemParams().general.PLAYER_RADIUS);
      }
    }
  }

}
//----------------------------------------------------------------------------