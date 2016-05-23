#include "LookForChipKickSkill.h"
#include "info/robot/robot.h"
#include "common/util/info/ball/ball.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "LookForChipKickSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"

int LookForChipKickSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
LookForChipKickSkill::LookForChipKickSkill( SystemParameters *sp,
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
bool LookForChipKickSkill::isValid() 
{ 
    //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------

  return initialized;
}


//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void LookForChipKickSkill::initialize(bool sameSide) 
{
  timer->resetTimer();
  this->sameSide = sameSide;
  initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void LookForChipKickSkill::execute()
{
	///If not active, dont do anything!
	if(!initialized) return;

  Pair ballLoc = getBallLocation(*currentVisionData);
  float currentRot = angleToBall(robotID,*currentVisionData,*sp);

  Pair dest((sp->field.HALF_LINE + sp->field.OUR_GOAL_LINE)/2.0f,
            (sp->field.SPLIT_LINE + sp->field.LEFT_SIDE_LINE)/2.0f);

  //setup on same side of field as ball
  if(ballLoc.getY() < sp->field.SPLIT_LINE)
    dest.setY(-dest.getY());

  command->setPos(dest);
  command->setRotation(currentRot);
}

//===============================================================================
bool LookForChipKickSkill::evaluate() 
{
	return true;
}

//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void LookForChipKickSkill::update() 
{
}

//===============================================================================
///test skill goes forever
bool LookForChipKickSkill::isFinished(float tolerance)
{
  return false;
}

//===============================================================================
void LookForChipKickSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/LookForChipKickSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
