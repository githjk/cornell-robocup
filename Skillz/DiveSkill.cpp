#include "DiveSkill.h"
#include "MoveToBallSkill.h"
#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "math/dist.h"
#include "intersect/intersect.h"
#include "math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"

int DiveSkill::skillNum = -1;

///Constructor.  Calls the base class Skill constructor
DiveSkill::DiveSkill(SystemParameters *sp,
                     VisionModule *vm, 
                     RoboCupStrategyModule *strat,
                     RobotIndex id, 
                     SkillSet* set) : Skill(sp,vm,strat,id, set)
{
  loadValues();
  initialized = false;
}

/// Check to see if we have the ball or not.
bool DiveSkill::isValid() 
{ 
  return true;
}

void DiveSkill::initialize() {
  initialize(getLocation(robotID,*currentVisionData,*sp).getX());
}
///resets timer
void DiveSkill::initialize(float lineX) 
{
  timer->resetTimer();
  initialized=true;
  destPoint.setPos(getLocation(robotID,*currentVisionData,*sp));
  destPoint.setRotation(0.0f);
  hold=false;
  line_x=lineX;
}


//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void DiveSkill::execute()
{    
  ///If not active, dont do anything!
  if(!initialized) return; 
  
  intersectFastMovingBall( *currentVisionData,*sp, line_x,command);
  strategy->getCurrentFrame()->setMessage(robotID,"Diving to catch ball");
  if(strategy->getCurrentRoboCupFrame()->getPositionOfRobot(robotID) != BLOCKER &&
     ABS(getRotation(robotID,*currentVisionData,*sp)) < KICK_ANGLE){
    command->setKick(KICK_SHOT);
  }else{
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
  }
}

///always true for now.
bool DiveSkill::evaluate() {
  return true;
}

//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void DiveSkill::update() {
  
}

///check if we've reached our target destination and rotation
bool DiveSkill::isFinished(float tolerance)
{
  return friendlyHasPossession(robotID, *sp, *currentVisionData, *(RobocupStrategyData*)strategy->getCurrentFrame(), false);
}




///Reads in the values of the parameters from the skill's parameter text file.
void DiveSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/DiveSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
  READ_FLOAT(KICK_ANGLE);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}