#include "FollowDefenderSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"

int FollowDefenderSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
FollowDefenderSkill::FollowDefenderSkill(SystemParameters *sp,
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
bool FollowDefenderSkill::isValid() 
{ 
  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void FollowDefenderSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
}


//=====================================================
///The robot will drive forward with its kicker on.
void FollowDefenderSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

  Pair defenderLoc;
  RobotIndex defenderID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER);
  if(defenderID != NO_ROBOT)
  {
    defenderLoc.set(strategy->getCurrentRoboCupFrame()->getDestination
         (strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER))->getPos());
  }
  else
  {
    defenderLoc.set(sp->field.OUR_GOALIE_BOX + 2*sp->general.PLAYER_RADIUS,
                    sp->field.SPLIT_LINE);
  }
  Pair ballLoc=strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();

  if(robotFound(defenderID,*currentVisionData,*sp)){
    command->setXPos(defenderLoc.getX() + X_OFFSET);
    if(ballLoc.getY() > sp->field.SPLIT_LINE){
      command->setYPos(defenderLoc.getY() -
                       sp->general.PLAYER_RADIUS -
                       Y_OFFSET);
    }else{
      command->setYPos(defenderLoc.getY() + 
                       sp->general.PLAYER_RADIUS +
                       Y_OFFSET);
    }
  }
	strategy->getCurrentRoboCupFrame()->setMessage(robotID, "Following Defender");
  //attempt to not drift
  command->setRotation(angleBetween(robotID,ballLoc,*currentVisionData,*sp));
}
//=====================================================
bool FollowDefenderSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void FollowDefenderSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool FollowDefenderSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void FollowDefenderSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/FollowDefenderSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(X_OFFSET);
  READ_FLOAT(Y_OFFSET);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
