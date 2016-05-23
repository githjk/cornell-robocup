#include "AggressiveJamAndShootSkill.h"
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
#include "common/util/intersect/intersect.h"

int AggressiveJamAndShootSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
AggressiveJamAndShootSkill::AggressiveJamAndShootSkill(SystemParameters *sp,
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
bool AggressiveJamAndShootSkill::isValid() 
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
void AggressiveJamAndShootSkill::initialize() 
{
  timer->resetTimer();
  initialized = true;
  target.set(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE);
  kicked=false;
  gaveup=false;
  aimed=false;
  driftdir=0;
  kickFrames=0;
  //choose an ideal radius
  ideal_radius= rand()*(IDEAL_RADIUS_HIGH-IDEAL_RADIUS_LOW) / RAND_MAX + IDEAL_RADIUS_LOW;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void AggressiveJamAndShootSkill::execute()
{    
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }
  
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
      //GUI_Record.debuggingInfo.setDebugPoint(robotID,4,upperbound,testline);
      //GUI_Record.debuggingInfo.setDebugPoint(robotID,5,lowerbound,testline);

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
        float baseDistance=currentPos.distanceTo(target);
        changeAngle= DRIFT_DIST/baseDistance*driftdir;

        char msg[80];
        sprintf(msg, "No Shot - drifting %s",( driftdir > 0 ? "right" : "left"));
        strategy->getCurrentFrame()->setMessage(robotID,msg);
        //drift around goal 
        
        //start at ideal radius and move out until a clear path is found.
        rotateAboutPoint(currentPos,target,changeAngle,dest);
        extendPoint(dest,target,-ideal_radius,dest);
        GUI_Record.debuggingInfo.setDebugPoint(robotID,8,dest);
        GUI_Record.debuggingInfo.setDebugPoint(robotID,9,target);

        RobotIndex blockID;
        int team;
        Pair block;
        Line AO;
        Line GD;
        Pair above;
        Pair newDest;
        float alpha;
        float laneWidth;
        int loopCount=0;
        //While we're still blocked, and haven't extended too far, keep searching for a point
        while(dest.distanceTo(target) < baseDistance+MAXIMUM_BACKUP &&
               !getBlockingRobot(currentPos,
                                 dest,
                                 sp->general.PLAYER_RADIUS*DRIFT_LANE_FACTOR,
                                 *currentVisionData,
                                 *sp,
                                 blockID,
                                 team)
             )
        {
          //find the distance we want to avoid this guy by.
          if(team==sp->general.TEAM){
            laneWidth=sp->general.PLAYER_RADIUS*DRIFT_LANE_FACTOR + sp->general.PLAYER_RADIUS+ 2*sp->general.BALL_RADIUS;
          }else{
            laneWidth=sp->general.PLAYER_RADIUS*DRIFT_LANE_FACTOR + sp->general.OPPONENT_RADIUS+ 2*sp->general.BALL_RADIUS;
          }
          //find his location
          block=getLocation(team,blockID,*currentVisionData);
          //the angle from him to the avoidance lane
          alpha=-ASIN(laneWidth/currentPos.distanceTo(block)) * driftdir;

          //find the point we want to drive to past him
          rotateAboutPoint(block,currentPos,alpha,above);
          extendPoint(above,currentPos,-(currentPos.distanceTo(block) * COS(alpha)), above);

          //extend that point until it hits the line we want to drive to
          AO.setA(currentPos);
          AO.setB(above);
          GD.setA(target);
          GD.setB(dest);
          newDest=findLineIntersection(AO,GD);
          if(newDest.distanceTo(target) <= dest.distanceTo(target)){
            extendPoint(dest,target,-(2*baseDistance+MAXIMUM_BACKUP),dest);
          }else{
            dest=newDest;
          }
          loopCount++;
          if(loopCount>50){
            GUI_Record.debuggingInfo.addDebugMessage("Jam & Shoot went into an infinite loop.  oops.");
            break;
          }
        }
        
        if(dest.distanceTo(target) > baseDistance+MAXIMUM_BACKUP){
          extendPoint(dest,target,-(baseDistance+MAXIMUM_BACKUP),dest);
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
Pair AggressiveJamAndShootSkill::getTargetPoint(){
  return target;
}
//===============================================================================
bool AggressiveJamAndShootSkill::evaluate() 
{
  return false;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void AggressiveJamAndShootSkill::update() 
{
}
//===============================================================================
bool AggressiveJamAndShootSkill::isFinished(float tolerance)
{
  TurnAndKickSkill *kicker=(TurnAndKickSkill *)skillSet->getSkill(TurnAndKickSkill::skillNum);
	return ((gaveup && timer->currentTime() > FINISHED_TIME) ||
          (kicked && kicker->isFinished(FINISHED_TIME)));
}
//===============================================================================
bool AggressiveJamAndShootSkill::hasKicked()
{
  return kicked;
}
//===============================================================================
void AggressiveJamAndShootSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/AggressiveJamAndShootSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(WAIT_TIME);
  READ_FLOAT(DRIFT_DIST);
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
  READ_FLOAT(IDEAL_RADIUS_LOW);
  READ_FLOAT(IDEAL_RADIUS_HIGH);
  READ_FLOAT(MAXIMUM_BACKUP);
  //READ_FLOAT(SAMPLE_DIST);
  READ_FLOAT(DRIFT_LANE_FACTOR);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
