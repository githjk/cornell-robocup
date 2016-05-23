#include "SmartRotateAroundBallSkill.h"
#include "info/robot/robot.h"
#include "info/ball/ball.h"
#include "math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/pair/pair.h"
#include "common/util/math/angle.h"
#include "common/util/math/cmath.h"

int SmartRotateAroundBallSkill::skillNum = -1;

//===============================================================================
///Constructor.  Calls the base class Skill constructor
SmartRotateAroundBallSkill::SmartRotateAroundBallSkill(SystemParameters *sp,
                                             VisionModule *vm, 
                                             RoboCupStrategyModule *strat,
                                             RobotIndex id, 
                                             SkillSet* set) : Skill(sp,vm,strat,id, set)
{
  loadValues();
}
//===============================================================================
//Always valid
bool SmartRotateAroundBallSkill::isValid() 
{ 
  return true;
}
//===============================================================================
//does nothing, since needs an ultimate destination
void SmartRotateAroundBallSkill::initialize() 
{

}
//===============================================================================
//resets timer, sets ultimate destination
void SmartRotateAroundBallSkill::initialize(Pair dest) 
{
  ultimateDestination.set(dest);
  timer->resetTimer();
  initialized = true;
  finished = false;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void SmartRotateAroundBallSkill::execute()
{    
  ///If not active, dont do anything!
  if(!initialized) return; 

  command->setDribble(SLOW_DRIBBLE);
  command->setVerticalDribble(SLOW_V_DRIBBLE);

  if(USE_BALL_FOR_PIVOT)
  {
    pivotPoint.set(strategy->getCurrentRoboCupFrame()->getOffensiveBallLocation());     
  }
  else
  {
    pivotPoint.set(frontOfRobot(robotID, *currentVisionData, *sp));
  }

  ultimateAngle = angleBetween(pivotPoint,ultimateDestination);

  //get robots current location and angle
  robotLoc.set(getLocation(robotID,*currentVisionData,*sp));
  robotAngle = angleBetween(robotLoc, pivotPoint);

  //if finished, then keep position and rotation
  if(isFinished())
  {
    command->setPos(robotLoc);
    command->setRotation(angleBetween(robotLoc, ultimateDestination));
    strategy->getCurrentFrame()->setMessage(robotID, "Smart Rotate Done");
    return;
  }

  strategy->getCurrentFrame()->setMessage(robotID, "Smart Rotate...");

  //determine next angle robot should be at
  nextAngle = ultimateAngle;

  //determine angle diff 
  angleDiff = angleDifference(ultimateAngle, robotAngle);
  
  //above safe line, rotate fastest way around
  if(
      robotLoc.getX() > sp->field.HALF_LINE + SAFE_LINE_HALF_LINE_OFFSET
    ) 
  {
    if( angleDiff > ROTATION_STEP )
    {
      nextAngle = normalizeAngle(robotAngle + ROTATION_STEP);
      angleDiff = ROTATION_STEP;
      strategy->getCurrentFrame()->setMessage(robotID, "SRotate Counter (1)");
    }
    else if (angleDiff < -ROTATION_STEP)
    {
      nextAngle = normalizeAngle(robotAngle - ROTATION_STEP);
      angleDiff = -ROTATION_STEP;
      strategy->getCurrentFrame()->setMessage(robotID, "SRotate Clock (2)");
    }
  }
  //below, go safer way around
  else
  {
     //bias your rotation by current rotation velocity, and determine
     //direction or rotation around ball to avoid switching because
     //of noise.
     Pair front = frontOfRobot(robotID, *currentVisionData, *sp);
     Pair centerGoal(sp->field.OUR_GOAL_LINE,
                     sp->field.SPLIT_LINE);
     float cross = crossProduct(robotLoc, front, centerGoal);
     float angleToGoal = normalizeAngle(ASIN(cross) + 
                                        (
                                          getRotVel(robotID,
                                                   *currentVisionData, 
                                                   *sp) / 
                                          sp->general.FRAME_RATE
                                        )
                                       ); 
     //fastest way when so far away from goal
     if(ABS(angleDifference(angleBetween(robotLoc, pivotPoint),
                            angleBetween(robotLoc, centerGoal))) > PI/2.0f)
     {
       if( angleDiff > ROTATION_STEP )
       {
         nextAngle = normalizeAngle(robotAngle + ROTATION_STEP);
         angleDiff = ROTATION_STEP;
         strategy->getCurrentFrame()->setMessage(robotID, "SRotate Counter (3)");
       }
       else if (angleDiff < -ROTATION_STEP)
       {
         nextAngle = normalizeAngle(robotAngle - ROTATION_STEP);
         angleDiff = -ROTATION_STEP;
         strategy->getCurrentFrame()->setMessage(robotID, "SRotate Clock (4)");
       }
     }
     else
     {    
       if(angleToGoal > 0.0f)
       {
         if(ABS(angleDiff) > ROTATION_STEP)
         {
           nextAngle = normalizeAngle(robotAngle - ROTATION_STEP);
           angleDiff = -ROTATION_STEP;
           strategy->getCurrentFrame()->setMessage(robotID, "SRotate Clock (5)");
         }
       }
       else
       {
         if(ABS(angleDiff) > ROTATION_STEP)
         {
           nextAngle = normalizeAngle(robotAngle + ROTATION_STEP);
           angleDiff = ROTATION_STEP;
           strategy->getCurrentFrame()->setMessage(robotID, "SRotate Counter (6)");
         }
       }
     }
  }

  if(ABS(angleDiff) < FINISHED_ROTATION_THRESHOLD)
  {
    finished = true;
  }

  Pair currLoc;
  extendPoint(robotLoc, 
              pivotPoint,
              -(sp->general.PLAYER_RADIUS - PUSH_FACTOR),
              currLoc);

  rotateAboutPoint(currLoc, 
                   pivotPoint,
                   normalizeAngle(nextAngle - robotAngle), 
                   rotatedDestination);

  destRot = angleBetween(rotatedDestination, pivotPoint);

  command->setPos(rotatedDestination);
  command->setRotation(destRot);

  GUI_Record.debuggingInfo.setDebugPoint(robotID,1,ultimateDestination);
  GUI_Record.debuggingInfo.setDebugPoint(robotID,2,robotLoc);
  GUI_Record.debuggingInfo.setDebugPoint(robotID,3,rotatedDestination);

}
//===============================================================================
///always true for now.
bool SmartRotateAroundBallSkill::evaluate() 
{
  return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void SmartRotateAroundBallSkill::update() 
{
  
}
//===============================================================================
/// determines if the robot is facing the right direction
bool SmartRotateAroundBallSkill::isFinished(float tolerance)
{  
  return finished;
}
//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void SmartRotateAroundBallSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/SmartRotateAroundBallSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(ROTATION_STEP);
  ROTATION_STEP = ROTATION_STEP * PI;

  READ_FLOAT(FINISHED_ROTATION_THRESHOLD);

  READ_FLOAT(PUSH_FACTOR);

  READ_INT(USE_BALL_FOR_PIVOT);

  READ_FLOAT(SAFE_LINE_HALF_LINE_OFFSET);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================
//returns true if b is between a1 and a2
bool SmartRotateAroundBallSkill::angleIsBetween(float a1, float a2, float b)
{
  //make a1 smaller
  if(a1 > a2)
  {
    float temp = a1;
    a1 = a2;
    a2 = temp;
  }

  //determine if smaller difference goes across
  //-PI to PI boundary
  if(ABS(a1) + ABS(a2) >= angleDifference(a1, a2))
  {
    return (b < a1 ||
           b > a2);
  }
  //else, see if b is between two values
  else
  {
    return (b > a1 &&
           b < a2);
  }
}
//===============================================================================
