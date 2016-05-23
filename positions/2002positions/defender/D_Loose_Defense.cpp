/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TandemDefenderSkill.h"
#include "Skillz/LooseDSpecialOpSkill.h"
#include "Skillz/FollowDefenderSkill.h"
#include "common/util/info/ball/ball.h"

/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Defender::looseDefense(RobotIndex ID,
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
  sd->setMessage(ID, "DEFENDER Loose Defense");

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
    Pair specialOpDefenderLocation=getLocation(SpecialOpID,field,sd->getSystemParams());
    Pair defenderLocation=getLocation(ID,field,sd->getSystemParams());

    if(
       (specialOpDefenderLocation.getY() > getBallLocation(field).getY() && 
          defenderLocation.getY() > sd->getSystemParams().field.SPLIT_LINE) ||
       (specialOpDefenderLocation.getY() < getBallLocation(field).getY() && 
          defenderLocation.getY() < sd->getSystemParams().field.SPLIT_LINE)
      )
    {
      sd->setMessage(SpecialOpID, "SPECIAL OP DEFENDER Taking over for Defender");
      sd->getDestination(SpecialOpID)->setPos(sd->getDestination(ID)->getPos());
    }

    //-------------------
    //Display message
    //-------------------
    sd->setMessage(SpecialOpID, "SPECIAL OP DEFENDER Loose Defense");


    //-------------------
    //get a handle on skillset for this robot
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on loose defense special Op skill
    //-------------------
    Skill* soSkillHandle;

    if(sd->getSystemParams().strategy2002.ENABLE_FOLLOW_DEFNDER_SKILL)
    {
      soSkillHandle = skills->getSkill(FollowDefenderSkill::skillNum);
    }
    else
    {
      soSkillHandle = skills->getSkill(LooseDSpecialOpSkill::skillNum);
    }

    //-------------------
    //initialize skill if it is not initialized
    //-------------------
    if(!soSkillHandle->isInitialized())
  	  soSkillHandle->initialize();

    //-------------------
    //run skill
    //-------------------
    soSkillHandle->run();
  }
}
//----------------------------------------------------------------------------