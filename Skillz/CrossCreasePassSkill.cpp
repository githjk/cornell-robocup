#include "CrossCreasePassSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/shoot/shoot.h"
#include "TurnAndKickSkill.h"
#include "ManeuverSkill.h"

int CrossCreasePassSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
CrossCreasePassSkill::CrossCreasePassSkill(SystemParameters *sp,
                                           VisionModule *vm, 
                                           RoboCupStrategyModule *strat,
                                           RobotIndex id, 
                                           SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();
  initialized = false;
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool CrossCreasePassSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void CrossCreasePassSkill::initialize() 
{
  receiver=CREATOR;
  timer->resetTimer();
  initialized = true;
  kicking=false;
  delay=0;

}
void CrossCreasePassSkill::initialize(Position receiverPosition){
  initialize();
  receiver=receiverPosition;
}

//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void CrossCreasePassSkill::execute()
{    

	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }
  if(kicking) delay--;

  creatorID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(receiver);
  
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  Pair creatorPos=strategy->getCurrentRoboCupFrame()->getPassDest(creatorID);
  
  //use turn and kick skill, but set isfinished when aimed
  //we will re-initialize the skill every frame.
  TurnAndKickSkill * kick =(TurnAndKickSkill *)skillSet->getSkill(TurnAndKickSkill::skillNum);
  kick->initialize(creatorPos,NO_KICK,true);
  kick->run();
  float angleDiff=angleDifference(getRotation(robotID,*currentVisionData,*sp),
                          angleBetween(currentPos,creatorPos));
  strategy->getCurrentFrame()->setMessage(robotID,"Aiming at Creator with T&K");

  if(ABS(angleDiff) <= HAPPY_ANGLE){
    kicking=true;
    delay=DELAY_FRAMES;
  }

  //Not used anymore:
  /*
  if(kicking){
    //we will transition into touch shoot, so we shouldn't kick here
    command->setDribble(SLOW_DRIBBLE);
    command->setVerticalDribble(SLOW_V_DRIBBLE);
    command->setRotation(angleBetween(currentPos,creatorPos));
    strategy->getCurrentFrame()->setMessage(robotID,"Ready to Pass");
  }else{

    SpinAroundBallSkill * spin=(SpinAroundBallSkill *)skillSet->getSkill(SpinAroundBallSkill::skillNum);
    
    //if creator changes, update TurnAndKick
    spin->initialize(creatorPos);
    spin->run();
    strategy->getCurrentFrame()->setMessage(robotID,"Aim at Creator");
    if(spin->isFinished(HAPPY_ANGLE)){
      kicking=true;
    }
    
    /*else{
      //go to corner of goalieBox
      Pair dest;
      dest.setX(sp->field.THEIR_GOALIE_BOX - sp->general.PLAYER_RADIUS - POS_OFFSET_X);
      if(currentPos.getY()< sp->field.SPLIT_LINE){
        dest.setY(sp->field.RIGHT_GOALIE_BOX - sp->general.PLAYER_RADIUS - POS_OFFSET_Y);
      }else{
        dest.setY(sp->field.LEFT_GOALIE_BOX + sp->general.PLAYER_RADIUS + POS_OFFSET_Y);
      }
      ManeuverWithBallSkill * maneuver=(ManeuverWithBallSkill *)skillSet->getSkill(ManeuverWithBallSkill::skillNum);
      maneuver->initialize(dest,creatorPos);
      maneuver->run();
    }*/
  //}
  //*/
  if(timer->currentTime() >= TIME_OUT){
    kicking=true;
  }
	
}
//===============================================================================
bool CrossCreasePassSkill::evaluate() 
{
	return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void CrossCreasePassSkill::update() 
{
}
//===============================================================================
///test skill goes forever
bool CrossCreasePassSkill::isFinished(float tolerance)
{
  return kicking && isLane(getLocation(robotID,*currentVisionData,*sp),
                           strategy->getCurrentRoboCupFrame()->getPassDest(creatorID),
                           getLocation(creatorID,*currentVisionData,*sp),
                           sp->strategy2002.PASS_LANE_THRESH/3.0f,
                           *currentVisionData,
                           *sp,
                           true) &&
         delay<=0;
}
//===============================================================================
void CrossCreasePassSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/CrossCreasePassSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(POS_OFFSET_X);
  READ_FLOAT(POS_OFFSET_Y);
  READ_FLOAT(TIME_OUT);
  READ_FLOAT(HAPPY_ANGLE);
  READ_INT(DELAY_FRAMES);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
