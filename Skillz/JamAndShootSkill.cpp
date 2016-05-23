#include "JamAndShootSkill.h"
#include "AggressiveJamAndShootSkill.h"
#include "StupidJamAndShootSkill.h"
#include "SimpleKickSkill.h"
#include "TurnAndKickSkill.h"
#include "PullBallOffWallSkill.h"
#include "SpinSkill.h"
#include "info/robot/robot.h"
#include "common/util/defense/defending.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/pair/pair.h"
#include "common/util/shoot/shoot.h"
#include "common/util/shoot/shotQuality.h"
#include "common/util/math/dist.h"

int JamAndShootSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
JamAndShootSkill::JamAndShootSkill(SystemParameters *sp,
                                   VisionModule *vm, 
                                   RoboCupStrategyModule *strat,
                                   RobotIndex id, 
                                   SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool JamAndShootSkill::isValid() 
{ 
  Pair t,loc;
  float angle=getRotation(robotID,*currentVisionData,*sp);
  loc=getLocation(robotID,*currentVisionData,*sp);
  return (calcShot(robotID,
                  sp->field.THEIR_GOAL_LINE,
                  sp->strategy2002.SHOOT_LANE_THRESH,
                  sp->field.RIGHT_GOAL_POST,
                  sp->field.LEFT_GOAL_POST,
                  NO_ROBOT,
                  *currentVisionData,
                  *sp,&t) ||
             goodCurrentShot(loc,
                             angle,
                             *currentVisionData,
                             *sp,
                             sp->strategy2002.SHOOT_LANE_THRESH));// &&
          //ABS(angleDifference(angle,angleBetween(loc,t))) <= VALID_ANGLE; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void JamAndShootSkill::initialize() 
{
    initialized = true;
  //normal mode
  if(sp->strategy2002.JAM_AND_SHOOT_MODE == 0)
  {
    timer->resetTimer();
    target.set(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE);
    kicked=false;
    gaveup=false;
    aimed=false;
    driftdir=0;
    kickFrames=0;
  }
  //aggressive mode
  else if(sp->strategy2002.JAM_AND_SHOOT_MODE == 1)
  {   
    skillSet->getSkill(AggressiveJamAndShootSkill::skillNum)->initialize();
  }
  //stupid mode
  else
  {
    skillSet->getSkill(StupidJamAndShootSkill::skillNum)->initialize();
  }
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void JamAndShootSkill::execute()
{  
  ///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

  //aggresive mode
  if(sp->strategy2002.JAM_AND_SHOOT_MODE == 1)
  {
    skillSet->getSkill(AggressiveJamAndShootSkill::skillNum)->run();
    return;
  }
  //stupid mode
  else if(sp->strategy2002.JAM_AND_SHOOT_MODE == 2)
  {
    skillSet->getSkill(StupidJamAndShootSkill::skillNum)->run();
    return;
  }
  
  
  
  //else normal mode
    //decrease the shoot threshold linearly as the skill runs.
  shoot_threshold = sp->strategy2002.SHOOT_LANE_THRESH;// *
                    //(WAIT_TIME - (float)timer->currentTime()) / WAIT_TIME;
                   
  
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  float currentRot = getRotation(robotID,*currentVisionData,*sp);
  float good_shoot_thresh=shoot_threshold;
  if(kicked) good_shoot_thresh*=1.5f;
  else if(aimed) good_shoot_thresh=.05f;
  //finished waiting.  Shoot!
  //note, we only shoot if we're facing in the direction of the goal.
  kickFrames--;
  if(
      !gaveup  && 
      towardGoal(currentPos,
                 currentRot,sp->field.THEIR_GOAL_LINE,
                 sp->field.LEFT_GOAL_POST,
                 sp->field.RIGHT_GOAL_POST) &&
      (timer->currentTime() > WAIT_TIME || 
       goodImmediateShot(sp->general.TEAM,robotID,sp->strategy2002.KICK_VELOCITY,
                        *currentVisionData,*sp,good_shoot_thresh)
      //goodCurrentShot(currentPos,currentRot,*currentVisionData,*sp,good_shoot_thresh)
      )
    )
  {
    gaveup=true;
    if(driftdir != 0){
      kickFrames = PAUSE_FRAMES + 2*rand()*PAUSE_RANDOM/RAND_MAX - PAUSE_RANDOM;
      char msg[80];
      sprintf(msg,"Jam and Shoot pausing for %d frames",kickFrames);
      GUI_Record.debuggingInfo.addDebugMessage(msg);
    }
    kicked=false;
    timer->resetTimer();
  }


  if (gaveup && kickFrames <= 0){
    //we're out of time or have a good current shot, so just kick.
    SimpleKickSkill * kickSkill=(SimpleKickSkill *) skillSet->getSkill(SimpleKickSkill::skillNum);
    if(!kickSkill->isInitialized()){
      kickSkill->initialize(KICK_SHOT);
    }
    kickSkill->run();
    strategy->getCurrentFrame()->setMessage(robotID,"No Turn and Kick. Just Shooting.");
  }else if(skillSet->getSkill(PullBallOffWallSkill::skillNum)->isValid()){
    //deal with the ball in the corner or on the wall
    if(!skillSet->getSkill(PullBallOffWallSkill::skillNum)->isInitialized()){
      skillSet->getSkill(PullBallOffWallSkill::skillNum)->initialize();
    }
    skillSet->getSkill(PullBallOffWallSkill::skillNum)->run();
    
  }else if(kicked) {
    //We have a shot, let's aim and kick.
    TurnAndKickSkill *kicker=(TurnAndKickSkill *)skillSet->getSkill(TurnAndKickSkill::skillNum);
    kicker->run();
    //strategy->getCurrentFrame()->setMessage(robotID,"Have Shot - taking it");    
  }else{
	  
    bool isShot;
    //find best shot (if we're already drifting, use a larger threshold)
    if(aimed)isShot=calcShot(robotID,sp->field.THEIR_GOAL_LINE,sp->strategy2002.SHOOT_LANE_THRESH+2*sp->general.PLAYER_RADIUS,
      sp->field.RIGHT_GOAL_POST,sp->field.LEFT_GOAL_POST,NO_ROBOT,*currentVisionData,*sp,&target);
    else isShot=calcShot(robotID,sp->field.THEIR_GOAL_LINE,sp->strategy2002.SHOOT_LANE_THRESH,
      sp->field.RIGHT_GOAL_POST,sp->field.LEFT_GOAL_POST,NO_ROBOT,*currentVisionData,*sp,&target);

    if(isShot && driftdir==0)
    {
      //take the shot
      TurnAndKickSkill *kicker=(TurnAndKickSkill *)skillSet->getSkill(TurnAndKickSkill::skillNum);
      kicker->initialize(target,KICK_SHOT,true);
      kicker->run();
      kicked=true;
      strategy->getCurrentFrame()->setMessage(robotID,"Have a shot!");   
      kickFrames=0;
    }else{
      target.set(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE);
      //if no shot, start to wait, dodge if possible
      GUI_Record.debuggingInfo.setDebugPoint(robotID,4,upperbound,testline);
      GUI_Record.debuggingInfo.setDebugPoint(robotID,5,lowerbound,testline);

      if (driftdir==0){
        //Calculate which direction to drift

        //first, see if we can shoot by shifting to the near post.
        //if not, drift across the field.

        //Default to drift across.
        float goalPost;
        if(currentPos.getY()>sp->field.SPLIT_LINE){
          driftdir = 1;
          goalPost=sp->field.LEFT_GOAL_POST;
        }else{
          driftdir = -1;
          goalPost=sp->field.RIGHT_GOAL_POST;
        }
        upperbound=sp->field.THEIR_GOAL_LINE;
        lowerbound=sp->field.KILL_ZONE_LINE;
        testline=sp->field.SPLIT_LINE + driftdir*(SIDE_DIST);

        Pair loc;

        //check near-side shot.
        if(currentPos.getY() * driftdir >= goalPost*driftdir &&
           calcYShot(Pair(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE),
                     testline,
                     sp->strategy2002.SHOOT_LANE_THRESH*SIDE_LANE_FACTOR,
                     upperbound,
                     lowerbound,NO_ROBOT,
                     *currentVisionData,
                     *sp, 
                     &loc)){
          driftdir=-driftdir;
        }
      }
      //  printf("%f\n",ABS(angleDifference(angleBetween(currentPos,target),currentRot)));

      if(ABS(angleDifference(angleBetween(currentPos,target),currentRot)) < AIM_ANGLE  || aimed){
        aimed=true;
        //if(target.getX() != sp->field.THEIR_GOAL_LINE) target.setX(sp->field.THEIR_GOAL_LINE);
        //drift to the side to see if we have a shot.
        //calculate the direction to drift
        
        
        Pair dest;
        float changeAngle;
        //switch directions if we've gone too far.
        if(angleBetween(currentPos,target)*driftdir > BOUNCE_ANGLE ){
          driftdir=-driftdir;
        }
        target.setY(target.getY() - AIM_DISTANCE*driftdir);
        changeAngle= DRIFT_ANGLE*driftdir;

        char msg[80];
        sprintf(msg, "No Shot - drifting %s",( driftdir > 0 ? "right" : "left"));
        strategy->getCurrentFrame()->setMessage(robotID,msg);
         //drift around goal 
        //See if we can push forward:
        rotateAboutPoint(frontOfRobot(robotID,*currentVisionData,*sp), target,changeAngle, dest);

   //DUE TO J&S BACKING UP TOO MUCH, I'M GETTING RID OF THIS NEXT PART
        
        if(!isLane(currentPos,dest,sp->general.PLAYER_RADIUS,*currentVisionData,*sp,true)){
          //we can't.  :(  drift sideways.
          rotateAboutPoint(currentPos, target,changeAngle, dest);
        }
        
        
        //see if one of their robots is in the way, and back up if they are.
        if(!isLane(currentPos,dest,sp->general.PLAYER_RADIUS,*currentVisionData,*sp,true)){
          //move destination back
          extendPoint(target,dest,sp->general.PLAYER_RADIUS,dest);
        }
        
        command->setPos(dest);
        //Adjust angle to fast the direction we're drifting a bit.
        command->setRotation(angleBetween(currentPos,target) - driftdir*DRIFT_ADJUST_ANGLE);
        command->setDribble(DRIBBLE_DEFAULT);
        command->setVerticalDribble(V_DRIBBLE_DEFAULT);
        command->setSpeed(BALL_POSSESSION_SPEED);
      }else{
        strategy->getCurrentFrame()->setMessage(robotID,"No Shot - turning to goal");
        //turn to face goal
        SpinAroundBallSkill *spin=(SpinAroundBallSkill *)strategy->getSkillSet(robotID)->getSkill(SpinAroundBallSkill::skillNum);
        spin->initialize(target);
        spin->run();
      }
    }
  }
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


  GUI_Record.debuggingInfo.setDebugPoint(robotID,0,target);
  //command->setControl(OMNI_NORMAL_ENTERBOX);
  command->setRotation(angleBetween(currentPos,target));
}
//===============================================================================
Pair JamAndShootSkill::getTargetPoint(){
  return target;
}
//===============================================================================
bool JamAndShootSkill::evaluate() 
{
  return false;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void JamAndShootSkill::update() 
{
}
//===============================================================================
bool JamAndShootSkill::isFinished(float tolerance)
{
  if(sp->strategy2002.JAM_AND_SHOOT_MODE == 0){
    TurnAndKickSkill *kicker=(TurnAndKickSkill *)skillSet->getSkill(TurnAndKickSkill::skillNum);
	  return ((gaveup && timer->currentTime() > FINISHED_TIME) ||
            (kicked && kicker->isFinished(FINISHED_TIME)));

  }
  //aggressive mode
  else if(sp->strategy2002.JAM_AND_SHOOT_MODE == 1)
  {   
    return skillSet->getSkill(AggressiveJamAndShootSkill::skillNum)->isFinished();
  }
  //stupid mode
  else
  {
    return skillSet->getSkill(StupidJamAndShootSkill::skillNum)->isFinished();
  }
}
//===============================================================================
bool JamAndShootSkill::hasKicked()
{
  if(sp->strategy2002.JAM_AND_SHOOT_MODE == 0)
  {
    return kicked;
  }
  //aggressive mode
  else if(sp->strategy2002.JAM_AND_SHOOT_MODE == 1)
  {   
    return ((AggressiveJamAndShootSkill*)skillSet->getSkill(AggressiveJamAndShootSkill::skillNum))->hasKicked();
  }
  //stupid mode
  else
  {
    return ((StupidJamAndShootSkill*)skillSet->getSkill(StupidJamAndShootSkill::skillNum))->hasKicked();
  }
}
//===============================================================================
void JamAndShootSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/JamAndShootSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(WAIT_TIME);
  READ_FLOAT(DRIFT_ANGLE);
  READ_FLOAT(AIM_ANGLE);
  READ_FLOAT(RANDOM_ANGLE);
  READ_FLOAT(EDGE_ANGLE);
  READ_FLOAT(AIM_DISTANCE);
  READ_FLOAT(FINISHED_TIME);
  READ_FLOAT(VALID_ANGLE);
  READ_FLOAT(BOUNCE_ANGLE);
  READ_FLOAT(ENTERBOX_CAUTION_DIST);
  READ_FLOAT(SIDE_DIST);
  READ_FLOAT(SIDE_LANE_FACTOR);
  READ_FLOAT(DRIFT_ADJUST_ANGLE);
  READ_INT(PAUSE_FRAMES);
  READ_INT(PAUSE_RANDOM);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
