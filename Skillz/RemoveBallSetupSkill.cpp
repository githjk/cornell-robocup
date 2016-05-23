#include "RemoveBallSetupSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"
#include "common/util/shoot/shoot.h"

int RemoveBallSetupSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
RemoveBallSetupSkill::RemoveBallSetupSkill(SystemParameters *sp,
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
bool RemoveBallSetupSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void RemoveBallSetupSkill::initialize() 
{
  timer->resetTimer();
  initialized = true;
  side = LEFT;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void RemoveBallSetupSkill::execute()
{    
  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("RemoveBallSetupSkill not initialized!");    
    return;  
  }
  else
  {

    
     // Intended Position and Rotation of aggressor
     Pair intendedPos;
     float angle;
    
    
    // Get the Blocker Location and Rotation
    RobotIndex blockerID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER);

    // Check where is the current position of aggressor ( Used if no blocker)
    Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
    // Blocker's location
    Pair blockerLoc;
  
   // If we do not have a blocker we are screwed and we just stay where we are
  if(blockerID != NO_ROBOT)
  {
  
      blockerLoc = getLocation(blockerID,*currentVisionData,*sp);
      float blockerRot = getRotation(blockerID,*currentVisionData,*sp);

    //Historesis: Check to see which side we should be on, and if we should switch
      if(side == LEFT && blockerLoc.getY() < sp->field.SPLIT_LINE - SWITCH_THRESH)
      {side = RIGHT;}
      else if(side == RIGHT && blockerLoc.getY() > sp->field.SPLIT_LINE + SWITCH_THRESH)
      {side = LEFT;}

      if(side == LEFT)
      {
         intendedPos.setY(sp->field.LEFT_GOALIE_BOX + OFFSET_GOAL_WALL);
         
         //angle = -FACE_ANGLE;
      }
      else
      {
         intendedPos.setY(sp->field.RIGHT_GOALIE_BOX - OFFSET_GOAL_WALL);
        //angle = FACE_ANGLE;
      }
 
  }
  else
  {

    if(currentPos.getY() > sp->field.SPLIT_LINE) {
      intendedPos.setY(sp->field.LEFT_GOALIE_BOX + OFFSET_GOAL_WALL);
      //angle = -FACE_ANGLE;
    } else {
      intendedPos.setY(sp->field.RIGHT_GOALIE_BOX - OFFSET_GOAL_WALL);
      //angle = FACE_ANGLE;
    }
  }

    
  
  
    intendedPos.setX(sp->field.OUR_GOAL_LINE+sp->general.PLAYER_RADIUS + OFFSET_GOAL_LINE);
    command->setPos(intendedPos);

    // If there is an opponent present in your location stay on top.
     
     for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
     {
	      Pair B(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));

        if((dist(command->getPos(),B) < sp->general.MIN_SEPARATION_DISTANCE  && 
        (dist(command->getPos(),currentPos) > sp->general.MIN_SEPARATION_DISTANCE)))
		    {
          command->setXPos(command->getXPos() + (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS + sp->general.OBSTACLE_CLEARANCE));
		    }
     }

     Pair upfieldPoint(sp->field.THEIR_GOAL_LINE,currentPos.getY());
     angle = findshootAngle(currentPos,upfieldPoint,blockerLoc,BIAS);
     command->setRotation(angle);

  // Turn the kicker on if the rotaton is close to intended rotation

     if(ABS(angleDifference(angle,command->getRotation())) < ANGLE_DIFF &&
       currentPos.distanceTo(command->getPos()) < CLOSE_DIST)
     {
       if(ABS(command->getRotation()) < (PI/2)*KICK_RATIO)
       {command->setKick(KICK_SHOT);}
     }

  }

  
}

///If the timer goes past 3 secs, reverse rotation
bool RemoveBallSetupSkill::evaluate() 
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
void RemoveBallSetupSkill::update() 
{
  ///If not initialized, dont do anything!
  if(!initialized) return;    
}



//===============================================================================
///test skill goes forever
bool RemoveBallSetupSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void RemoveBallSetupSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/RemoveBallSetupSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  ///Read in a test param
  READ_FLOAT(OFFSET_GOAL_LINE);
  READ_FLOAT(OFFSET_GOAL_WALL);
  READ_FLOAT(FACE_ANGLE);
  READ_FLOAT(ANGLE_DIFF);
  READ_FLOAT(CLOSE_DIST);
  READ_FLOAT(SWITCH_THRESH);
  READ_FLOAT(BIAS);
  READ_FLOAT(KICK_RATIO);


  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================
