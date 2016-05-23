#include "SimpleKickSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"

int SimpleKickSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
SimpleKickSkill::SimpleKickSkill(SystemParameters *sp,
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
bool SimpleKickSkill::isValid() 
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
void SimpleKickSkill::initialize()
{
  GUI_Record.debuggingInfo.addDebugMessage("Please initialize SimpleKick with a kick speed");
  initialize(KICK_SHOT);
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void SimpleKickSkill::initialize(KickSpeed k)
{
	//reset timer
	timer->resetTimer();
	initialized = true;
  kickSpeed=k;
  rotation = getRotation(robotID, *currentVisionData, *sp);
}


//=====================================================
///The robot will drive forward with its kicker on.
void SimpleKickSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

	strategy->getCurrentRoboCupFrame()->setMessage(robotID, "Kicking Forward");
  command->setPos(frontOfRobot(robotID,*currentVisionData,*sp));
  //attempt to not drift
  command->setRotation(rotation);
  command->setDribble(NO_DRIBBLE);
  command->setVerticalDribble(NO_V_DRIBBLE);
  command->setKick(kickSpeed);
  //so we can push in with the ball if necessary
  command->setControl(OMNI_NORMAL_ENTERBOX);
}
//=====================================================
bool SimpleKickSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void SimpleKickSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool SimpleKickSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void SimpleKickSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/SimpleKickSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	

	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
