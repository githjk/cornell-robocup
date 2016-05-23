#include "SimpleDefenseSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/info/ball/ball.h"
#include "GUI/GUI.h"
int SimpleDefenseSkill::skillNum = -1;
//=====================================================
///Constructor.  Calls the base class Skill constructor
SimpleDefenseSkill::SimpleDefenseSkill(SystemParameters *sp,
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
bool SimpleDefenseSkill::isValid() 
{ 
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------
	return true;
  // The skill must be initialized to do this check!
	if(!initialized)
	{
  	return false;
	}
  else
	{
		/// The skill is valid only if the ball is found
			return isRobocupBallFound(*currentVisionData);
	}
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void SimpleDefenseSkill::initialize(float DefenseX)
{
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------
	//reset timer
	timer->resetTimer();

 //if valid X specified
	if((DefenseX <= sp->field.THEIR_GOAL_LINE)	&&	(DefenseX >= sp->field.OUR_GOAL_LINE))
     constX = DefenseX;
  else
     constX = DEFAULT_DEFENSE_X;

	initialized = true;
}
//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void SimpleDefenseSkill::execute()
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
	else
	{
		//grab the ball location
		Pair result;
		result = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();
  	int dI = 0;
		Pair dest;
		/// Set the x-co-ordinate to the line we want to defend
	  dest.setX(constX);
    /// Set the y-co-ordinate to the ball's co-ordinate
	  dest.setY(result.getY() );
		command->setPos(dest);
    if(result.getX() < constX)
        command->setRotation(3.14f);
     else
        command->setRotation(0.0f);
	}
}
//=====================================================
///If the timer goes past 3 secs, reverse rotation
bool SimpleDefenseSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void SimpleDefenseSkill::update() 
{
	///at this time simple screening does not "learn"
}

///Screen is never finished, it can always continue
bool SimpleDefenseSkill::isFinished(float tolerance)
{
	return false;
}

///Reads in the values of the parameters from the skill's parameter text file.
void SimpleDefenseSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
  mainFile.open("Params/Skill_Params/SimpleDefenseSkillParams.txt", ios::nocreate);	
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.	See params.h and params.cpp for good examples.

  //################	ADD PARAMETERS BELOW ################
	//default X position if not specified when skill initialized or the X position is not within the range required
	READ_FLOAT(DEFAULT_DEFENSE_X);		
	//################	ADD PARAMETERS ABOVE ################

	mainFile.close();
}
