#include "AggressorRecoverBallSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"

int AggressorRecoverBallSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
AggressorRecoverBallSkill::AggressorRecoverBallSkill(const SystemParameters *rp,
                                                     VisionModule *vm, 
                                                     RoboCupStrategyModule *strat,
                                                     RobotIndex id, 
                                                     SkillSet* set) : 
                                                     Skill(rp,vm,strat,id,set) 
{
  initialized = false;
	ID = id;
}
//=====================================================
///Call this function to check the prerequisites of the skill.	This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool AggressorRecoverBallSkill::isValid() 
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
void AggressorRecoverBallSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void AggressorRecoverBallSkill::execute()
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

	
  Pair currentPos(getLocation(robotID,*currentVisionData,*params));
  float currentRot = angleToBall(robotID,*currentVisionData,*params);
  command->setPos(currentPos);
  command->setRotation(currentRot);
}
//=====================================================
bool AggressorRecoverBallSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void AggressorRecoverBallSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool AggressorRecoverBallSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void AggressorRecoverBallSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/AggressorRecoverBallSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	

	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
