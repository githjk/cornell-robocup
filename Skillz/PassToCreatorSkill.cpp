#include "PassToCreatorSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/shoot/shoot.h"
#include "SpinSkill.h"

int PassToCreatorSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
PassToCreatorSkill::PassToCreatorSkill(SystemParameters *sp,
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
bool PassToCreatorSkill::isValid() 
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
void PassToCreatorSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
  kicking=false;
  spinning=true;
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void PassToCreatorSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }
  
	
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  creatorID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(CREATOR);
  if(creatorID==NO_ROBOT) return;

  Pair creatorPos(strategy->getCurrentRoboCupFrame()->getPassDest(creatorID));
  /*if(kicking){
    //We're kicking, so kick.

    //This should not be done.  We should transition into TouchShoot at this point.
    command->setPos(creatorPos);
    command->setRotation(angleBetween(currentPos,creatorPos));
    command->setDribble(NO_DRIBBLE);
    command->setVerticalDribble(NO_V_DRIBBLE);
    command->setKick(KICK_SHOT);
    strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Passing to Creator");
  }else{*/
    //still aiming
    if(spinning){
      //use SpinAroundBall to face creator
      SpinAroundBallSkill * spin=(SpinAroundBallSkill *)skillSet->getSkill(SpinAroundBallSkill::skillNum);
      spin->initialize(creatorPos);
      spin->run();
      strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Aiming for Creator with Spin");
      //We continue in this until the play tranistions, so never switch out of the aiming section
      if(spin->isFinished(HAPPY_ANGLE)) spinning=false;
    }else{
      //face creator and wait for open lane
      command->setRotation(angleBetween(currentPos,creatorPos));
      command->setDribble(DRIBBLE_DEFAULT);
      command->setVerticalDribble(V_DRIBBLE_DEFAULT);
      strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Aiming for Creator");
      if(isLane(currentPos,
                creatorPos,
                getLocation(creatorID,*currentVisionData,*sp),
                PASS_LANE_FACTOR*sp->strategy2002.PASS_LANE_THRESH,
                *currentVisionData,*sp,true)){
        //we can see our friend!  kick!  kick like the wind!
        kicking=true;
      }
    }
  //}




}
//=====================================================
bool PassToCreatorSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void PassToCreatorSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool PassToCreatorSkill::isFinished(float tolerance)
{
  return kicking;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void PassToCreatorSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/PassToCreatorSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(PASS_LANE_FACTOR);
  READ_FLOAT(HAPPY_ANGLE);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
