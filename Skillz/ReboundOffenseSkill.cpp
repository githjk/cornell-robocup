#include "ReboundOffenseSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"

int ReboundOffenseSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
ReboundOffenseSkill::ReboundOffenseSkill(SystemParameters *sp,
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
bool ReboundOffenseSkill::isValid() 
{ 
	//----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
	currentVisionData = visionModule->getCurrentFrame();
	///ALWAYS call the previous 2 functions to get the latest data
	//----------------------------

  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void ReboundOffenseSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void ReboundOffenseSkill::execute()
{		 
  //----------------------------
	///ALWAYS call the following 2 functions to get the latest data
	currentVisionData = visionModule->getCurrentFrame();
	command = strategy->getCurrentFrame()->getDestination(robotID);
	///ALWAYS call the previous 2 functions to get the latest data
	//----------------------------

	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

	Pair goalPos(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE);
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  float currentRot = angleBetween(currentPos,goalPos);
  command->setPos(currentPos);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=
  Pair centerGoal(sp->field.THEIR_GOAL_LINE, sp->field.SPLIT_LINE);
  command->setRotation(angleBetween(getLocation(robotID,
                                                *currentVisionData,
                                                *sp),
                                                centerGoal));
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=
  //if the angle is between 3PI/8 and -3PI/8 arm the kicker
  if(
      getRotation(robotID, *currentVisionData, *sp) < 3.0f*PI/8.0f &&
      getRotation(robotID, *currentVisionData, *sp) > -3.0f*PI/8.0f
    )
  {
      if(ABS(command->getRotation()) < (PI/2)*KICK_RATIO)
      {command->setKick(KICK_SHOT);}
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=
}
//=====================================================
bool ReboundOffenseSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void ReboundOffenseSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool ReboundOffenseSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void ReboundOffenseSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/ReboundOffenseSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(KICK_RATIO);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
