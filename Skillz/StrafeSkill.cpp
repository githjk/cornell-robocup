#include "StrafeSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/shoot/shotquality.h"

int StrafeSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
StrafeSkill::StrafeSkill(SystemParameters *sp,
                                 VisionModule *vm, 
                                 RoboCupStrategyModule *strat,
                                 RobotIndex id, 
                                 SkillSet* set) : Skill(sp,vm,strat,id,set) 
{
  initialized = false;
}
//=====================================================
///Call this function to check the prerequisites of the skill.	This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool StrafeSkill::isValid() 
{ 
  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void StrafeSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
  kick = false;
  kickCounter = 0;

  //if looking left, go right, if looking right, go left
  firstRot = getRotation(robotID, *currentVisionData, *sp);
  if(firstRot > 0.0f)
  {
    left = false;
  }
  else
  {
    left = true;
  }

  finished = false;
}
//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void StrafeSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized) return;

  if(left)
  {
    command->setPos(sp->field.THEIR_GOAL_LINE,
                    sp->field.LEFT_GOALIE_BOX);
  }
  else
  {
    command->setPos(sp->field.THEIR_GOAL_LINE,
                    sp->field.RIGHT_GOALIE_BOX);
  }

  command->setRotation(firstRot);
  command->setControl(OMNI_GOALIE_FAST);
  command->setSpeed(DEFAULT_SPEED);

  if(goodImmediateShot(sp->general.TEAM,
                       robotID,
                       sp->strategy2002.KICK_VELOCITY,
                       *currentVisionData,
                       *sp,
                       sp->strategy2002.SHOOT_LANE_THRESH)
    )
  {
    kick = true;
    kickCounter = -5;
  }


  if(kick) 
  {
    command->setKick(KICK_SHOT_IMMEDIATE);
  }
  else
  {  
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
  }

  if(kickCounter < 0) kickCounter++;

  if(kickCounter == -1)
    finished = true;

}
//=====================================================
bool StrafeSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void StrafeSkill::update() 
{
}
//=====================================================
bool StrafeSkill::isFinished(float tolerance)
{
	return finished;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void StrafeSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/StrafeSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	

	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
