#include "RotateAroundBallSkill.h"
#include "info/robot/robot.h"
#include "info/ball/ball.h"
#include "math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/pair/pair.h"

int RotateAroundBallSkill::skillNum = -1;

//===============================================================================
///Constructor.  Calls the base class Skill constructor
RotateAroundBallSkill::RotateAroundBallSkill(SystemParameters *sp,
                                             VisionModule *vm, 
                                             RoboCupStrategyModule *strat,
                                             RobotIndex id, 
                                             SkillSet* set) : Skill(sp,vm,strat,id, set)
{
  loadValues();
}
//===============================================================================
//Always valid
bool RotateAroundBallSkill::isValid() 
{ 
  return true;
}
//===============================================================================
//does nothing, since needs an ultimate destination
void RotateAroundBallSkill::initialize() 
{

}
//===============================================================================
//resets timer, sets ultimate destination
void RotateAroundBallSkill::initialize(Pair dest) 
{
  ultimateDestination.set(dest);
  timer->resetTimer();
  initialized = true;
  finished = false;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void RotateAroundBallSkill::execute()
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
    strategy->getCurrentFrame()->setMessage(robotID, "rotate done");
    return;
  }

  strategy->getCurrentFrame()->setMessage(robotID, "rotate rotating");

  //determine next angle robot should be at
  nextAngle = ultimateAngle;

  //determine angle diff 
  angleDiff = angleDifference(ultimateAngle, robotAngle);
  
  if( angleDiff > ROTATION_STEP )
  {
    nextAngle = robotAngle + ROTATION_STEP;
    angleDiff = ROTATION_STEP;
  }
  else if (angleDiff < -ROTATION_STEP)
  {
    nextAngle = robotAngle - ROTATION_STEP;
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
                   nextAngle - robotAngle, 
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
bool RotateAroundBallSkill::evaluate() 
{
  return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void RotateAroundBallSkill::update() 
{
  
}
//===============================================================================
/// determines if the robot is facing the right direction
bool RotateAroundBallSkill::isFinished(float tolerance)
{  
  return finished;
}
//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void RotateAroundBallSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/RotateAroundBallSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(ROTATION_STEP);
  ROTATION_STEP = ROTATION_STEP * PI;

  READ_FLOAT(FINISHED_ROTATION_THRESHOLD);

  READ_FLOAT(PUSH_FACTOR);

  READ_INT(USE_BALL_FOR_PIVOT);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================
