/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/AcquirePossessionSkill.h"
#include "Skillz/BehindScreenOffenseSkill.h"
#include "common/util/info/ball/ball.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::upfieldDefense(RobotIndex ID,
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
  sd->setMessage(ID, "AGGRESSOR upfield Defense");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on upfield defense aggressor skill
  //-------------------
  AcquirePossessionSkill* skillHandle = 
  (AcquirePossessionSkill*)skills->getSkill(AcquirePossessionSkill::skillNum);

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
    //if the ball is in their goalie box, aggress will be going after it. thus set
    //our special op in line with the nearest free kick mark, in case he gets tagged
    Pair ballLoc = getBallLocation(field);
    if(ballLoc.getX() >= sd->getSystemParams().field.THEIR_GOALIE_BOX &&
       ballLoc.getY() >= sd->getSystemParams().field.RIGHT_GOALIE_BOX &&
       ballLoc.getY() <= sd->getSystemParams().field.LEFT_GOALIE_BOX)
    {
      sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR scared of free kick");

      //look straight at free kick mark
      sd->getDestination(SpecialOpID)->setRotation(0.0f);

      //left
      if(ballLoc.getY() > sd->getSystemParams().field.SPLIT_LINE)
      {
        sd->getDestination(SpecialOpID)->setPos(sd->getSystemParams().field.THEIR_GOAL_LINE - 
                                                sd->getSystemParams().field.FREE_KICK_X_OFFSET -
                                                sd->getSystemParams().general.OPPONENT_RADIUS -
                                                sd->getSystemParams().general.PLAYER_RADIUS - 
                                                0.20f,
                                                sd->getSystemParams().field.LEFT_SIDE_LINE -
                                                sd->getSystemParams().field.FREE_KICK_Y_OFFSET);
      } 
      //right
      else
      {
        sd->getDestination(SpecialOpID)->setPos(sd->getSystemParams().field.THEIR_GOAL_LINE - 
                                                sd->getSystemParams().field.FREE_KICK_X_OFFSET -
                                                sd->getSystemParams().general.OPPONENT_RADIUS -
                                                sd->getSystemParams().general.PLAYER_RADIUS - 
                                                0.20f,
                                                sd->getSystemParams().field.RIGHT_SIDE_LINE +
                                                sd->getSystemParams().field.FREE_KICK_Y_OFFSET);
      }
    }
    else
    {
      //-------------------
      //Display message
      //-------------------
      sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR upfield Defense");

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
}
//----------------------------------------------------------------------------