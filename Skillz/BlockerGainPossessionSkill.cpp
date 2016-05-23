#include "BlockerGainPossessionSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/math/dist.h"
#include "common/util/pair/pair.h"
#include "common/util/info/ball/ball.h"

int BlockerGainPossessionSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
BlockerGainPossessionSkill::BlockerGainPossessionSkill(SystemParameters *sp,
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
bool BlockerGainPossessionSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void BlockerGainPossessionSkill::initialize() 
{
  timer->resetTimer();
  initialized = true;
  finished=false;
  frames=0;
  backingUp = false;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void BlockerGainPossessionSkill::execute()
{    
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }


  command->setSpeed(GOALIE_SPEED);
	backingUp = false;
  Pair ballLoc=strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();
  Pair rLoc=getLocation(robotID,*currentVisionData,*sp);

  /*
  //if ball within robot radius, assume it has it, prediction error, say finished
  if(ballLoc.distanceTo(rLoc) <= sp->general.PLAYER_RADIUS)
  {
    strategy->getCurrentFrame()->setMessage(robotID,"Have Ball");
    command->setPos(rLoc);
    command->setRotation(getRotation(robotID, *currentVisionData, *sp));
    
    //finished = true;
    return;
  }
  */

  int side=(ballLoc.getY() > sp->field.SPLIT_LINE ? 1 : -1);
  if(ballLoc.getX() < sp->field.OUR_GOALIE_BOX - sp->general.BALL_RADIUS){
    
    // *************activate ball-suck here************
    
    if(ballLoc.getX() <= rLoc.getX() + BEHIND_DIST){

      //be careful not to knock ball into goal
        command->setSpeed(CAREFUL_SPEED);

      if(ballLoc.getY() < sp->field.RIGHT_GOAL_POST || ballLoc.getY() > sp->field.LEFT_GOAL_POST){
        //suck up ball from wall        
        if(ABS(angleDifference(angleBetween(rLoc,ballLoc),
            getRotation(robotID,*currentVisionData,*sp))) < FACING_BALL_ANGLE){
          command->setXPos(ballLoc.getX() + sp->general.DIST_TO_DRIBBLER);
        }else{
          command->setXPos(ballLoc.getX() + sp->general.PLAYER_RADIUS + sp->general.BALL_RADIUS);
        }
        command->setYPos(ballLoc.getY());
        strategy->getCurrentFrame()->setMessage(robotID,"Grabbing Ball");
        if(friendlyHasPossession(robotID,*sp,*currentVisionData,*strategy->getCurrentRoboCupFrame(),true)){
          command->setRotation(side * PI/3.0f);
          strategy->getCurrentFrame()->setMessage(robotID,"Facing out");
        }
      }else{
        //go carefully around ball
        Pair dest;
          //carefully circle
        if(ABS(angleDifference(angleBetween(rLoc,ballLoc),
          getRotation(robotID,*currentVisionData,*sp))) < FACING_BALL_ANGLE){
          ball_cushion_dist=sp->general.DIST_TO_DRIBBLER+BALL_CUSHION;
          command->setSpeed(CAREFUL_SPEED);
        }else{
          //face ball quickly
          ball_cushion_dist=sp->general.PLAYER_RADIUS+ sp->general.BALL_RADIUS+BALL_CUSHION;
          command->setSpeed(GOALIE_SPEED);
        }


        extendPoint(rLoc,ballLoc,-(ball_cushion_dist),dest);
        float angle;
        if(rLoc.getY()<ballLoc.getY()){
          if(dist(sp->field.OUR_GOAL_LINE,sp->field.RIGHT_GOAL_POST,ballLoc.getX(),ballLoc.getY())
              >(sp->general.PLAYER_RADIUS+ball_cushion_dist)){
            angle=-SPIN_ANGLE;
          }else angle=SPIN_ANGLE;

        }else{
          if(dist(sp->field.OUR_GOAL_LINE,sp->field.LEFT_GOAL_POST,ballLoc.getX(),ballLoc.getY())
              >(sp->general.PLAYER_RADIUS+ball_cushion_dist)){
            angle=SPIN_ANGLE;
          }else angle=-SPIN_ANGLE;
        }
        rotateAboutPoint(dest,ballLoc,angle,dest);
        command->setPos(dest);
        command->setRotation(angleBetween(dest,ballLoc));//frontOfRobot(robotID,*currentVisionData,*sp)));
        strategy->getCurrentFrame()->setMessage(robotID,"Going Around Ball");
      }
    }else{
      //suck up ball
      strategy->getCurrentFrame()->setMessage(robotID,"Sucking up the ball");
      command->setPos(ballLoc);
      if(friendlyHasPossession(robotID,*sp)){
        finished=true;
      }
    }
  }else{
    Pair goal(sp->field.OUR_GOAL_LINE,sp->field.SPLIT_LINE);
    Pair dest;
    //go grab ball
    if(friendlyHasPossession(robotID,*sp)){
      frames++;
    }else{
      frames=0;
    }
    if(frames>=POS_FRAMES){
      //backing up
      dest.setY(rLoc.getY());
      dest.setX(sp->field.OUR_GOALIE_BOX - sp->general.PLAYER_RADIUS - 2.0f*sp->general.BALL_RADIUS - BACKUP_DIST);
      command->setPos(dest);
      command->setRotation(0.0f);
      command->setSpeed(CAREFUL_SPEED); //go slowly backing up
      backingUp = true;
      strategy->getCurrentFrame()->setMessage(robotID,"Backing up");
      command->setDribble(SLOW_DRIBBLE);
      command->setVerticalDribble(SLOW_V_DRIBBLE);

    }else{
      //heading to ball
      dest.set(ballLoc.getX() - sp->general.PLAYER_RADIUS - 2* sp->general.BALL_RADIUS,
               ballLoc.getY());
      if(ABS(ballLoc.getY() - rLoc.getY()) <= sp->general.PLAYER_RADIUS){
        command->setRotation(angleBetween(rLoc,ballLoc));
        if(ABS(angleDifference(getRotation(robotID,*currentVisionData,*sp),angleBetween(rLoc,ballLoc))) < PI/8){
          dest=robotPositionGivenFront(ballLoc,0.0f,*sp);
        }
      }
      command->setPos(dest);
      strategy->getCurrentFrame()->setMessage(robotID,"Going to ball");
    }
  }
  if(dist(ballLoc,rLoc)-sp->general.PLAYER_RADIUS<DRIBBLE_DIST  &&  !backingUp){
    command->setDribble(DRIBBLE_DEFAULT);
    command->setVerticalDribble(V_DRIBBLE_DEFAULT);
  }
  command->setControl(OMNI_GOALIE);

}
//===============================================================================
bool BlockerGainPossessionSkill::evaluate() 
{
	return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void BlockerGainPossessionSkill::update() 
{
}
//===============================================================================
///test skill goes forever
bool BlockerGainPossessionSkill::isFinished(float tolerance)
{
  return finished;
}
//===============================================================================
void BlockerGainPossessionSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/BlockerGainPossessionSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(BACKUP_DIST);
  READ_FLOAT(BACKUP_SPEED);
  READ_FLOAT(BALL_CUSHION);
  READ_FLOAT(DRIBBLE_DIST);
  READ_FLOAT(SPIN_ANGLE);
  READ_FLOAT(BEHIND_DIST);
  READ_FLOAT(FACING_BALL_ANGLE);
  READ_FLOAT(POS_FRAMES);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
bool BlockerGainPossessionSkill::isBackingUp(){

  return backingUp;
}
//===============================================================================
