#include "DribblePickScoreSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/math/dist.h"
#include "common/util/shoot/shoot.h"
#include "SpinSkill.h"
#include "ManeuverSkill.h"

int DribblePickScoreSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
DribblePickScoreSkill::DribblePickScoreSkill(SystemParameters *sp,
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
bool DribblePickScoreSkill::isValid() 
{ 
  return true;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void DribblePickScoreSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
  state=1;
  direction=0;
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void DribblePickScoreSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

	
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  float currentRot = angleToBall(robotID,*currentVisionData,*sp);

  //find end of pick wall:
  RobotIndex pickID;
  pickID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_CREATOR);
  if(!robotFound(pickID,*currentVisionData,*sp)) pickID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(CREATOR);
  if(!robotFound(pickID,*currentVisionData,*sp)){
    //no wall. hmmm.
    destPos.set(sp->field.THEIR_GOALIE_BOX - PICK_BUFFER - sp->general.PLAYER_RADIUS,sp->field.SPLIT_LINE);
  }else{
    //set destination to be behind the wall
    //destPos=getLocation(pickID,*currentVisionData,*sp);
    //if(destPos.getX() < currentPos.getX()) {
      //robot is behind us.  go around its destination.
      destPos=strategy->getCurrentFrame()->getDestination(pickID)->getPos();
    //}
    destPos.setX(destPos.getX() -2*sp->general.PLAYER_RADIUS - PICK_BUFFER);
  }
  
  if (state<=1){    //Spin!
    if(destPos.getY() < currentPos.getY()) direction=-1;
    else direction = 1;

    SpinAroundBallSkill * spin = (SpinAroundBallSkill *)skillSet->getSkill(SpinAroundBallSkill::skillNum);
    spin->initialize(destPos);
    if(!spin->isFinished(TURN_HAPPY_ANGLE)){
      spin->run();
      strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Facing Pick");
    }else{
//      state++;
    
//  }
//  if (state==1){    //Drive to pick, until we're there or past our dest y-wise.
      if(dist(currentPos,destPos) < CLOSE_DIST  || currentPos.getY()*direction > destPos.getY()*direction){
        state++;
      }else{
        Pair facePos;
        facePos.set(destPos.getX(),.5f*direction + destPos.getY());

        ManeuverWithBallSkill * drive = (ManeuverWithBallSkill *)skillSet->getSkill(ManeuverWithBallSkill::skillNum);
        drive->initialize(destPos, facePos);
        drive->run();
        strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Driving to Pick");
      }
    }
  }
  if (state==2){    //Drive around pick
    Pair facePos;
    destPos.setX(destPos.getX() + FORWARD_DIST);
    destPos.setY(destPos.getY() + PAST_DIST * direction);
    facePos.set(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE);
    ManeuverWithBallSkill * drive = (ManeuverWithBallSkill *)skillSet->getSkill(ManeuverWithBallSkill::skillNum);
    drive->initialize(destPos, facePos);
    drive->run();
    strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Driving Around Pick");
  }
  if(state>2){
    strategy->getCurrentRoboCupFrame()->setMessage(robotID,"State got messed up.  Oops.");
  }

}
//=====================================================
bool DribblePickScoreSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void DribblePickScoreSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool DribblePickScoreSkill::isFinished(float tolerance)
{
  Pair target;
	return (state==2 && (dist(getLocation(robotID,*currentVisionData,*sp),destPos) < tolerance ||
                       calcShot(robotID,sp->field.THEIR_GOAL_LINE,sp->strategy2002.SHOOT_LANE_THRESH,
                                sp->field.RIGHT_GOAL_POST,sp->field.LEFT_GOAL_POST,NO_ROBOT,
                                *currentVisionData,*sp,&target)));
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void DribblePickScoreSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/DribblePickScoreSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(TURN_HAPPY_ANGLE);
  READ_FLOAT(PAST_DIST);
  READ_FLOAT(FORWARD_DIST);
  READ_FLOAT(PICK_BUFFER);
  READ_FLOAT(CLOSE_DIST);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
