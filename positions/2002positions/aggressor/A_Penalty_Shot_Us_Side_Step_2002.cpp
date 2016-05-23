/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/PenaltyShotUsAimAndShootSkill.h"
#include "Skillz/BehindScreenOffenseSkill.h"
#include "Skillz/StrafeSkill.h"
#include "common/util/shoot/shotquality.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::penaltyShotUsSideStep2002(RobotIndex ID,
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
  sd->setMessage(ID, "AGGRESSOR Charge!");
  
  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on upfield defense aggressor skill
  //-------------------
  StrafeSkill* skillHandle = 
  (StrafeSkill*)skills->getSkill(StrafeSkill::skillNum);

  //-------------------
  //initialize skill if it is not initialized
  //-------------------
  if(!skillHandle->isInitialized())
	  skillHandle->initialize();

  //-------------------
  //run skill
  //-------------------
  skillHandle->run();
/*
  sd->getDestination(ID)->setXPos(sd->getSystemParams().field.THEIR_GOAL_LINE);// -
                                  //sd->getSystemParams().general.PLAYER_RADIUS);
  sd->getDestination(ID)->setYPos(sd->getSystemParams().field.LEFT_GOAL_POST);
  sd->getDestination(ID)->setDribble(FAST_DRIBBLE);
  sd->getDestination(ID)->setVerticalDribble(FAST_V_DRIBBLE);
  sd->getDestination(ID)->setControl(OMNI_NORMAL_ENTERBOX);
  if(goodImmediateShot(sd->getSystemParams().general.TEAM,
                       ID,
                       sd->getSystemParams().strategy2002.KICK_VELOCITY,
                       field,sd->getSystemParams(),
                       sd->getSystemParams().strategy2002.SHOOT_LANE_THRESH)
    )
  {
    sd->getDestination(ID)->setKick(KICK_SHOT_IMMEDIATE);
  }
*/
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