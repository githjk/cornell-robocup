#include "TightDefenseSpecialOpDSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "PassBlockSkill.h"
#include "ShotBlockSkill.h"

int TightDefenseSpecialOpDSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
TightDefenseSpecialOpDSkill::TightDefenseSpecialOpDSkill( SystemParameters *sp,
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
bool TightDefenseSpecialOpDSkill::isValid() 
{ 
  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void TightDefenseSpecialOpDSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
}
void TightDefenseSpecialOpDSkill::initialize(TightDType opponents, RobotIndex ID){
 	//reset timer
	timer->resetTimer();
	initialized = true;
  type=opponents;
  opponentID=ID;
  switch(type){
  case NO_OPPONENTS:
    break;
  case ONE_OPPONENT:
    subSkill=(ShotBlockSkill *)skillSet->getSkill(ShotBlockSkill::skillNum);
    ((ShotBlockSkill *)subSkill)->initialize(opponentID);
    break;
  case MANY_OPPONENTS:
    subSkill=(PassBlockSkill *)skillSet->getSkill(PassBlockSkill::skillNum);
    ((PassBlockSkill *)subSkill)->initialize(opponentID);
    break;

  }
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void TightDefenseSpecialOpDSkill::execute()
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

  strategy->getCurrentFrame()->setMessage(robotID, "special op d in tight defense");


  //if no opponents go to default position
  if(type == NO_OPPONENTS)
  {
    Pair ballLoc = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();

    //ultimate default destination
    Pair dest;
    dest.setX(sp->field.DEFENSE_ZONE_LINE);
    
    //current robot location
    Pair robotLoc = getLocation(robotID, *currentVisionData, *sp);
    
    if(ballLoc.getY() < sp->field.SPLIT_LINE)
      dest.setY(sp->field.SPLIT_LINE);
    else
      dest.setY(sp->field.SPLIT_LINE);

    command->setPos(dest);

    //look at the ball
    float angleToBall = angleBetween(robotLoc, ballLoc);
    command->setRotation(angleToBall);
  }
  else
    subSkill->run();
}
//=====================================================
bool TightDefenseSpecialOpDSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void TightDefenseSpecialOpDSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool TightDefenseSpecialOpDSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void TightDefenseSpecialOpDSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/TightDefenseSpecialOpDSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	

	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
