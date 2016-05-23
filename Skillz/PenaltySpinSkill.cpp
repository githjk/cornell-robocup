#include "PenaltySpinSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"

int PenaltySpinSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
PenaltySpinSkill::PenaltySpinSkill(SystemParameters *sp,
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
bool PenaltySpinSkill::isValid() 
{ 
  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void PenaltySpinSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
  finished = false;
 
  //pick at random
  if(PENALTY_SPIN_DIRECTION == 0)
  {
    float random = ((float)rand())/RAND_MAX;
  
    if(random > 0.5)
      direction = 1;
    else
      direction = -1;
  }
  else
  {
    direction = PENALTY_SPIN_DIRECTION;
  }

  theta = 0.0f;
  thetaVel = 0.0f;
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void PenaltySpinSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized) return;

  thetaVel = thetaVel + direction*ACCEL/sp->general.FRAME_RATE;
  
  //bound max theta vel
  if(thetaVel > MAX_ROT_VEL)
    thetaVel = MAX_ROT_VEL;
  if(thetaVel < -MAX_ROT_VEL)
    thetaVel = -MAX_ROT_VEL;

  theta+=thetaVel/sp->general.FRAME_RATE;
 
  if(ABS(theta) > THETA_THRESH)
  {
    thetaVel = 0.0f;
    finished = true;
  }

  command->setRotVel(thetaVel);
  command->setXVel(0.0f);
  command->setYVel(0.0f);
  command->setControl(MANUAL_CONTROL);
  command->setDribble(FAST_DRIBBLE);
  command->setVerticalDribble(FAST_V_DRIBBLE);


  if(direction == 1)
  {
    strategy->getCurrentRoboCupFrame()->setMessage(robotID, "going left");
  }
  else if(direction == -1)
  {
    strategy->getCurrentRoboCupFrame()->setMessage(robotID, "going right");
  }
  else
  {
    strategy->getCurrentRoboCupFrame()->setMessage(robotID, "error!!");
  }
}
//=====================================================
bool PenaltySpinSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void PenaltySpinSkill::update() 
{
}
//=====================================================
bool PenaltySpinSkill::isFinished(float tolerance)
{
	return finished;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void PenaltySpinSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/PenaltySpinSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(ACCEL);
  READ_FLOAT(THETA_THRESH);
  READ_FLOAT(MAX_ROT_VEL);
  READ_INT(PENALTY_SPIN_DIRECTION);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
