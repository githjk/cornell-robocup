#include "SpinSkill.h"
#include "RotateAroundBallSkill.h"
#include "SmartRotateAroundBallSkill.h"
#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"

int SpinAroundBallSkill::skillNum = -1;

///Constructor.  Calls the base class Skill constructor
SpinAroundBallSkill::SpinAroundBallSkill(SystemParameters *sp,
                                         VisionModule *vm, 
                                         RoboCupStrategyModule *strat,
                                         RobotIndex id, 
                                         SkillSet* set) : Skill(sp,vm,strat,id, set)
{
  loadValues();
  initialized = false;
}

/// Check to see if we have the ball or not.
bool SpinAroundBallSkill::isValid() 
{ 
  //check for possession
  //return hasPossession(robotID, *visionModule->getCurrentFrame(), *sp,false);
  
  //testing only:
  return true;
}

///resets timer
void SpinAroundBallSkill::initialize() 
{
  timer->resetTimer();
}

///Perform any initializations for the skill, such as reseting the timer.
void SpinAroundBallSkill::initialize(Pair dest) 
{
  if(USE_ROTATE)
  {
    //check if should use smart rotate
    if(sp->strategy2002.ENABLE_SMART_ROTATE_AROUND_BALL)
    {
      ((SmartRotateAroundBallSkill*)skillSet->getSkill(SmartRotateAroundBallSkill::skillNum))->initialize(dest);
    }
    else
    {
      ((RotateAroundBallSkill*)skillSet->getSkill(RotateAroundBallSkill::skillNum))->initialize(dest);
    }
  }
  timer->resetTimer();
  destPoint=dest;
}

//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void SpinAroundBallSkill::execute()
{    
  if(USE_ROTATE)
  {
    //check if should use smart rotate
    if(sp->strategy2002.ENABLE_SMART_ROTATE_AROUND_BALL)
    {
      skillSet->getSkill(SmartRotateAroundBallSkill::skillNum)->run();
    }
    else
    {
      skillSet->getSkill(RotateAroundBallSkill::skillNum)->run();
    }
    return;
  }

  //stay a player radius away, but we are square in front, so multiply by 0.707,
  
  
  GUI_Record.debuggingInfo.setDebugPoint(robotID,1,destPoint);
  
  
  pivot = frontOfRobot(robotID, *currentVisionData, *sp);
  Pair robotLoc=getLocation(robotID,*currentVisionData,*sp);
  
  curAngle = getRotation(robotID, *currentVisionData, *sp);
  
  //Find angle at that point
  float finalAngle = angleBetween(pivot,destPoint);

  float dAngle = angleDifference(finalAngle, curAngle);
  //float dAngle=SPIN_AROUND_BALL_THRESHHOLD+1;
  //calculate velocities
  float ROTATION_RADIUS = sp->general.DIST_TO_DRIBBLER + sp->general.BALL_RADIUS +RADIUS_OFFSET; 
  float ROT_RATE = ABS(dAngle*ROTATION_GAIN+ROTATION_BIAS);// + (float)fabs(currentVisionData->getRobot(sp->general.TEAM,robotID).getRotVel()) ;

  if(ROT_RATE > sp->control.Omni2001.DEFAULT.MAX_THETA_VELOCITY) ROT_RATE=sp->control.Omni2001.DEFAULT.MAX_THETA_VELOCITY;
  float linearVelocity = ROT_RATE*ROTATION_RADIUS;

  command->setControl(MANUAL_CONTROL);
  command->setDribble(SLOW_DRIBBLE);
  command->setVerticalDribble(SLOW_V_DRIBBLE);

  command->setPos(robotLoc);
  float xDot=COS(curAngle)*PUSH_FACTOR*linearVelocity;  
  float yDot=SIN(curAngle)*PUSH_FACTOR*linearVelocity; 

  if( dAngle > SPIN_AROUND_BALL_THRESHHOLD ){
    //finalAngle = curAngle + SPIN_AROUND_BALL_THRESHHOLD;
    xDot+= SIN(curAngle+ANGLE_OFFSET) * linearVelocity;
    yDot+= -COS(curAngle+ANGLE_OFFSET) * linearVelocity;
    command->setVel(xDot,yDot);
    command->setRotVel(ROT_RATE);

 
  //If the difference in angle is greater than PI, go the opposite way
  }else if (dAngle < -SPIN_AROUND_BALL_THRESHHOLD){
    //finalAngle = curAngle - SPIN_AROUND_BALL_THRESHHOLD;
    xDot+= -SIN(curAngle-ANGLE_OFFSET) * linearVelocity;
    yDot+= COS(curAngle-ANGLE_OFFSET) * linearVelocity;
    command->setVel(xDot,yDot);
    command->setRotVel(-ROT_RATE);

  }else{
    //we're close, go straight there
    ROTATION_RADIUS-=.03f;
    
    finalAngle = (finalAngle+curAngle)/2.0f;
    normalizeAngle(finalAngle);
    command->setControl(OMNI_NORMAL);
    //float dx,dy;
    //dx=pivot.getX() - ROTATION_RADIUS * COS(finalAngle) - robotLoc.getX();
    //dy=pivot.getY() - ROTATION_RADIUS * SIN(finalAngle) - robotLoc.getY();
    rotDest = Pair(pivot.getX() - ROTATION_RADIUS * COS(finalAngle),pivot.getY() - ROTATION_RADIUS * SIN(finalAngle));
    command->setPos(rotDest);
    command->setDribble(FAST_DRIBBLE);
    command->setRotation( finalAngle );
  }



  //Never send off the field
  constrainToField(*sp, &rotDest);

  //Never go in goalie box
  if( nearTheirGoalieBox( rotDest, *sp, sp->general.PLAYER_RADIUS) ) {
    rotDest.setX( sp->field.OUR_GOALIE_BOX + (sp->general.PLAYER_RADIUS) );
  }

}

///always true for now.
bool SpinAroundBallSkill::evaluate() {
  return true;
}

//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void SpinAroundBallSkill::update() {
  
}

/// determines if the robot is facing the right direction
bool SpinAroundBallSkill::isFinished(float tolerance)
{
  return ABS(angleDifference(angleBetween(getLocation(robotID,* visionModule->getCurrentFrame(),*sp),destPoint),
          getRotation(robotID,* visionModule->getCurrentFrame(),*sp)))<=tolerance;
}


///Reads in the values of the parameters from the skill's parameter text file.
void SpinAroundBallSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/SpinSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  
  READ_FLOAT(ROTATION_GAIN);
  READ_FLOAT(ROTATION_BIAS);
  READ_FLOAT(RADIUS_OFFSET);
  READ_FLOAT(SPIN_AROUND_BALL_THRESHHOLD);
  READ_FLOAT(ANGLE_OFFSET);
  READ_FLOAT(RADIUS_ADJUST_FACTOR);
  READ_FLOAT(PUSH_FACTOR);
  READ_INT(USE_ROTATE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}