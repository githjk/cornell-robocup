#include "ShotBlockSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/defense/defending.h"
#include "common/util/pair/pair.h"
#include "common/util/math/angle.h"
#include "common/util/info/ball/ball.h"

int ShotBlockSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
ShotBlockSkill::ShotBlockSkill(SystemParameters *sp,
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
bool ShotBlockSkill::isValid() { return true; }




//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void ShotBlockSkill::initialize() {
  GUI_Record.debuggingInfo.addDebugMessage("Please initialize ShotBlock with an opponent's ID");
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void ShotBlockSkill::initialize(RobotIndex opponent) {
  initialized = true;
  opponentID=opponent;
  
}




//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void ShotBlockSkill::execute()
{    
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data

  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("ShotBlock not initialized!");    
    return;  
  }
  
  //find opponent's shot:
  float y1,y2, y;
  if(findOpShot(opponentID,robotID,y1,y2,*currentVisionData,*sp)){
    //shot found
    y=(y1+y2)/2;
  }else{
    //opponent has no shot, block him anyways.
    y=(sp->field.RIGHT_GOAL_POST + sp->field.LEFT_GOAL_POST)/2;
  }

  Pair opLoc=getTheirRobotLocation(opponentID,*currentVisionData,*sp);
  Pair blockLoc;
  blockLoc.set(sp->field.OUR_GOAL_LINE,y);

  Pair target;
  extendPoint(blockLoc,opLoc,-(sp->general.PLAYER_RADIUS+sp->general.OPPONENT_RADIUS+GRACE_FACTOR),target);
  if(target.getX() < sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS + sp->general.BALL_RADIUS)
  {
    target.setX(sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS + sp->general.BALL_RADIUS);
  }
  command->setPos(target);
  command->setRotation(angleBetween(robotID,getBallLocation(*currentVisionData),*currentVisionData,*sp));
  strategy->getCurrentFrame()->setMessage(robotID,"Performing Shot Block");
}

///If the timer goes past 3 secs, reverse rotation
bool ShotBlockSkill::evaluate() {
  return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void ShotBlockSkill::update() {
  ///If not initialized, dont do anything!
  if(!initialized) return;    
}



//===============================================================================
///test skill goes forever
bool ShotBlockSkill::isFinished(float tolerance){
  return false;
}




//===============================================================================
void ShotBlockSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/ShotBlockSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(GRACE_FACTOR);


  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}