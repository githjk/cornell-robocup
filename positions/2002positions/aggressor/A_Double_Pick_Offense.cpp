/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/DribblePickScoreSkill.h"
#include "Skillz/AdvanceBallUpfieldSkill.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::doublePickOffense(RobotIndex ID,
                            BasePlay* play,
                            const VisionData& field,
                            RobocupStrategyData* sd)
{
  //-------------------
  //Display message
  //-------------------
  sd->setMessage(ID, "AGGRESSOR double Pick Offense");

  //-------------------
  //get a handle on skillset for this robot
  //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on double pick offense aggressor 
  //skill or advance ball upfield skill, depending
  //on circumstance
  //-------------------
  Skill* skillHandle;

  //assume by default we'll advane the ball
  bool dribbleAround = false;

  //if creator exists, and creator is 
  //between me and the close goal post, then
  //dribble around
  RobotIndex CreatorID = sd->getRobotByPosition(CREATOR);
  RobotIndex AggressorID = sd->getRobotByPosition(AGGRESSOR);
  if(AggressorID != NO_ROBOT)
  {
        if(CreatorID != NO_ROBOT)
        {
          Pair creatorLoc = getLocation(CreatorID,
                                        field,
                                        sd->getSystemParams());               
    
          Pair aggressorLoc = getLocation(AggressorID,
                                          field,
                                          sd->getSystemParams());               


          //used to test if creator is betweeen 
          //aggressor and this yCoorindate of post
          float yPost;

          if(creatorLoc.getY() > sd->getSystemParams().field.LEFT_GOAL_POST)
          {
            yPost = sd->getSystemParams().field.LEFT_GOAL_POST;
          }
          else if(creatorLoc.getY() < sd->getSystemParams().field.RIGHT_GOAL_POST)
          {
            yPost = sd->getSystemParams().field.RIGHT_GOAL_POST;
          }

          //two inbetween cases
          if(
              (creatorLoc.getY() > aggressorLoc.getY() &&
               yPost > creatorLoc.getY()) ||
              (creatorLoc.getY() > yPost &&
               aggressorLoc.getY() > creatorLoc.getY())
            )
          {
            dribbleAround = true;
          }
        }
  
        //if(dribbleAround)
          skillHandle = skills->getSkill(DribblePickScoreSkill::skillNum);
        //else
        //  skillHandle = skills->getSkill(AdvanceBallUpfieldSkill::skillNum);

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