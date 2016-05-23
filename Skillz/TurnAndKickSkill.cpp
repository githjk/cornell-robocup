#include "TurnAndKickSkill.h"
#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "math/dist.h"
#include "math/angle.h"
#include "pair/pair.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"





int TurnAndKickSkill::skillNum = -1;



///Constructor.  Calls the base class Skill constructor
TurnAndKickSkill::TurnAndKickSkill(SystemParameters *sp,
                                   VisionModule *vm, 
                                   RoboCupStrategyModule *strat,
                                   RobotIndex id, 
                                   SkillSet* set) : Skill(sp,vm,strat,id, set)
{
   loadValues();
   initialized = false;
   updateComputed=false;
   kicked=false;
}

/// Check to see if we have the ball or not.
bool TurnAndKickSkill::isValid() 
{
  //check for possession
  return friendlyHasPossession(robotID, *sp, *visionModule->getCurrentFrame(), *(RobocupStrategyData*)strategy->getCurrentFrame(), false);
  
  //testing only:
  return true;
}

///resets timer
void TurnAndKickSkill::initialize() 
{
   spinSkill=(SpinAroundBallSkill*)skillSet->getSkill(SpinAroundBallSkill::skillNum);
   maneuverSkill=(ManeuverWithBallSkill*)skillSet->getSkill(ManeuverWithBallSkill::skillNum);
   spinSkill->initialize(targetPoint);
}

//common initializations
void TurnAndKickSkill::initialize(KickSpeed k,bool precise) 
{
  timer->resetTimer();
  kicked=false;
  kickSpeed = k;
  updateComputed=false;
  float ballspeed=getBallSpeed(*currentVisionData);
  if(ballspeed<=SPEED_THRESHHOLD){ 
	  wasMoving=false;
  }else{
    wasMoving=true;  
    //wasMoving=false; ///this is a test, since maneuver doesn't seem to work so well for kicking.
  }
  preciseKick=precise;
  initialize();
}

///Initialize giving a pair to kick to.
void TurnAndKickSkill::initialize(Pair face, KickSpeed k,bool precise) 
{
  targetPoint=face;
  isRobot=false;
  initialize(k,precise);
}
///initialize giving a robot to kick to
void TurnAndKickSkill::initialize(RobotIndex robot, KickSpeed k,bool precise) 
{
  faceRobot=robot;
  isRobot=true;
  initialize(k,precise);
}


//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void TurnAndKickSkill::execute()
{    
  if (kicked){
    doKick(targetPoint);
    return;
  }
  
  //always slow rotate
  command->setSpeed(SLOW_ROTATE);
 
  if(isRobot && !kicked){
    targetPoint=getLocation(faceRobot,*currentVisionData,*sp);
    spinSkill->initialize(targetPoint);
  }
  float aimOff=angleDifference(angleBetween(getLocation(robotID,*currentVisionData,*sp),targetPoint),
                              getRotation(robotID,*currentVisionData,*sp));
  char msg[80];
  //see if we're moving fast or slow
  if(!wasMoving){
    //use SpinAroundBall

    if((!preciseKick && spinSkill->isFinished(STILL_KICK_THRESHHOLD))
      || preciseKick && spinSkill->isFinished(PRECISION_ANGLE)){
      //Kick time
      wasMoving=false;
      doKick(targetPoint);
    }else{
      spinSkill->run();
      sprintf(msg,"Kick->Spin: off by %f",aimOff);
		  strategy->getCurrentFrame()->setMessage(robotID,msg);
    }

  }  else  {
    //use ManeuverWithBall
    Pair extension;
    extendPoint(getLocation(robotID,*currentVisionData,*sp),targetPoint, .01f,extension);
    maneuverSkill->initialize(targetPoint,extension);

    if(
        ABS(angleDifference(
                             angleBetween(robotID,targetPoint,*currentVisionData,*sp),
                             getRotation(robotID,*currentVisionData,*sp)
                           )) <= MOVING_KICK_THRESHHOLD
      )
    {
      //kick time
      wasMoving=true;
      doKick(targetPoint);
    }else{
      maneuverSkill->run();
    //Enter The goalie box if their goalie is far enough away.
      RobotIndex goalie;
      if(getLocation(robotID,*currentVisionData,*sp).getX() > sp->field.HALF_LINE){
        if(getTheirGoalie(*currentVisionData,*sp,&goalie)){
          if(getLocation(sp->general.OTHER_TEAM,goalie,*currentVisionData).distanceTo(getLocation(robotID,*currentVisionData,*sp)) >= 
              ENTERBOX_CAUTION_DIST+sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS){
            command->setControl(OMNI_FAST_ENTERBOX);
          }
        }else{
          //if no goalie, enter the box
          command->setControl(OMNI_FAST_ENTERBOX);
        }
      }
      sprintf(msg,"Kick->Maneuver: off by %f",aimOff);
		  strategy->getCurrentFrame()->setMessage(robotID,msg);

    }

  }

  //always dribble.
  //command->setDribble();
  //command->setVerticalDribble();


}

/**  doKick:  do the commands necessary for kicking */
void TurnAndKickSkill::doKick(Pair targetPoint)
{
  if(!kicked){
    kicked=true;
    kickLoc=getBallLocation(*currentVisionData);
    timer->resetTimer();
  }
  Pair newdest;
  //extrapolateForY(getLocation(robotID,*currentVisionData,*sp),targetPoint,sp->field.THEIR_GOALIE_BOX,newdest);
  extendPoint(targetPoint,getLocation(robotID,*currentVisionData,*sp),-sp->general.PLAYER_RADIUS,newdest);
  command->setPos(newdest);
  command->setRotation(angleBetween(robotID,targetPoint,*currentVisionData,*sp));
  command->setKick(kickSpeed);
  if(kickSpeed !=NO_KICK){
    command->setDribble(NO_DRIBBLE);
    command->setVerticalDribble(NO_V_DRIBBLE);
  }else{
    command->setDribble(DRIBBLE_DEFAULT);
    command->setVerticalDribble(V_DRIBBLE_DEFAULT);
  }
  command->setControl(OMNI_NORMAL);
  strategy->getCurrentFrame()->setMessage(robotID,"Kick: Kicking");

}

///calculate the update if the ball's gone far enough
bool TurnAndKickSkill::evaluate() 
{
  Pair ballLoc=getBallLocation(*currentVisionData);
  if(kicked && !updateComputed && dist(ballLoc,kickLoc)>=UPDATE_DISTANCE && !preciseKick){
    //ball has gone far enough, lets update
    
    //see if it went mostly the right direction
    float kickAngle=angleBetween(kickLoc,ballLoc);
    float correctAngle=angleBetween(kickLoc,targetPoint);
    float errAngle=(float)fabs(angleDifference(kickAngle,correctAngle));
    if(errAngle>=IGNORE_ANGLE){
      updateComputed=false;
      kicked=false;
    }else{
      //Calculate Update
      float oldValue;
      if(wasMoving){
        oldValue=MOVING_KICK_THRESHHOLD;
      }else {
        oldValue=STILL_KICK_THRESHHOLD;
      }
      updateVal=(oldValue+errAngle)/2;
      updateComputed=true;
    }
  }else if(kicked && timer->currentTime()>=UPDATE_TIMEOUT){
    //been too long, give up on updating
    kicked=false;
    updateComputed=false;
  }

  return true;
}

//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void TurnAndKickSkill::update() 
{
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------
  if(kicked && updateComputed){
    char msg[80];
    if(wasMoving){
      MOVING_KICK_THRESHHOLD=updateVal;
      sprintf(msg,"Kick: Moving Updated: %f",updateVal);
    }else{
      STILL_KICK_THRESHHOLD=updateVal;
      sprintf(msg,"Kick: Stationary Updated: %f",updateVal);
    }
    GUI_Record.debuggingInfo.addDebugMessage(msg);
    kicked=false;
    updateComputed=false;
  }  
}

///check if we've kicked at least tolerance seconds ago
/// (allows if to push through the ball for a certain amount of time.)
bool TurnAndKickSkill::isFinished(float tolerance)
{
  return kicked && timer->currentTime()>=tolerance;
}




///Reads in the values of the parameters from the skill's parameter text file.
void TurnAndKickSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/TurnAndKickSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  
  ///Read in a test param
  READ_FLOAT(UPDATE_DISTANCE);
  READ_FLOAT(STILL_KICK_THRESHHOLD);
  READ_FLOAT(MOVING_KICK_THRESHHOLD);
  READ_FLOAT(SPEED_THRESHHOLD);
  READ_FLOAT(UPDATE_TIMEOUT);
  READ_FLOAT(IGNORE_ANGLE);
  READ_FLOAT(PRECISION_ANGLE);
  READ_FLOAT(ENTERBOX_CAUTION_DIST);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}