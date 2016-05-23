#include "RemoveBallHelpSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"


int RemoveBallHelpSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
RemoveBallHelpSkill::RemoveBallHelpSkill(SystemParameters *sp,
                                         VisionModule *vm, 
                                         RoboCupStrategyModule *strat,
                                         RobotIndex id, 
                                         SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();
  initialized = false;
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool RemoveBallHelpSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void RemoveBallHelpSkill::initialize() 
{
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------
  timer->resetTimer();
  initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void RemoveBallHelpSkill::execute()
{    

  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("RemoveBallHelpSkill not initialized!");    
    return;  
  }

  // Get the Blocker Location and Rotation
  RobotIndex blockerID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER);
  
  
  // If we do not have a blocker we are screwed and we just stay where we are
  if(blockerID != NO_ROBOT)
  {
  
      Pair blockerLoc(getLocation(blockerID,*currentVisionData,*sp));
      float blockerRot = getRotation(blockerID,*currentVisionData,*sp);

      // All decisions made based on which side the blocker is facing
      command->setXPos(sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS + OFFSET_X);
      strategy->getCurrentFrame()->setMessage(robotID,"Following Blocker");
  
      if(blockerRot > ANGLE_BOUNDARY*PI)
      {
        command->setYPos(blockerLoc.getY() - OFFSET_Y);
        command->setRotation(ANGLE_OFFSET*PI);
      }
      else if(blockerRot < -ANGLE_BOUNDARY*PI)
      {
        command->setYPos(blockerLoc.getY() + OFFSET_Y);
        command->setRotation(-ANGLE_OFFSET*PI);
      }else{
        strategy->getCurrentFrame()->setMessage(robotID,"Waiting for Blocker to turn");
      }

      // If there is an opponent present in your location stay on top.
     Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
     for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
     {
	      Pair B(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));

        if((dist(command->getPos(),B) < sp->general.MIN_SEPARATION_DISTANCE  && 
        (dist(command->getPos(),currentPos) > sp->general.MIN_SEPARATION_DISTANCE)))
		    {
          //command->setYPos(B.getX() + (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS + sp->general.OBSTACLE_CLEARANCE));
		    }
     }
  }else{
    strategy->getCurrentFrame()->setMessage(robotID,"Blocker not found");
  }
}

///If the timer goes past 3 secs, reverse rotation
bool RemoveBallHelpSkill::evaluate() 
{
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------
  ///If not initialized, dont do anything!
  if(!initialized) return true;  

  return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void RemoveBallHelpSkill::update() 
{
  ///If not initialized, dont do anything!
  if(!initialized) return;    
}



//===============================================================================
///test skill goes forever
bool RemoveBallHelpSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void RemoveBallHelpSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/RemoveBallHelpSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(OFFSET_X);
  READ_FLOAT(OFFSET_Y);
  READ_FLOAT(ANGLE_OFFSET);
  READ_FLOAT(ANGLE_BOUNDARY);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================
