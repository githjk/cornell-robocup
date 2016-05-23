#include "ManeuverSkill.h"
#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "math/dist.h"
#include "math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"



int ManeuverWithBallSkill::skillNum = -1;



///Constructor.  Calls the base class Skill constructor
ManeuverWithBallSkill::ManeuverWithBallSkill(SystemParameters *sp, 
                                             VisionModule *vm, 
                                             RoboCupStrategyModule *strat,
                                             RobotIndex id, 
                                             SkillSet* set) : Skill(sp,vm,strat,id, set)
{
   loadValues();
  initialized = false;
}

/// Check to see if we have the ball or not.
bool ManeuverWithBallSkill::isValid() 
{ 
  //check for possession
  return friendlyHasPossession(robotID, *sp, *visionModule->getCurrentFrame(), *(RobocupStrategyData*)strategy->getCurrentFrame(), false);
  
  //testing only:
  //return true;
}

///resets timer
void ManeuverWithBallSkill::initialize() 
{
  timer->resetTimer();
}

///Perform any initializations for the skill, such as reseting the timer.
void ManeuverWithBallSkill::initialize(Pair dest, Pair face) 
{
  timer->resetTimer();
  destPoint=dest;
  facePoint=face;
  initialized=true;
}

//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void ManeuverWithBallSkill::execute()
{    
  Pair robotPoint= getLocation(robotID,*currentVisionData,*sp);
  ///TESTING-----
  command->setPos(destPoint);
  command->setRotation(angleBetween(robotPoint,facePoint));
  strategy->getCurrentFrame()->setMessage(robotID,"going directly there");
  command->setDribble(DRIBBLE_DEFAULT);
  command->setVerticalDribble(V_DRIBBLE_DEFAULT);
  return;
  //--------------
  
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 0 , destPoint);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 1 , facePoint);

  Pair ballPoint;

  ///If not active, dont do anything!
  if(!initialized) return; 
  
  //always dribble.
  command->setDribble(DRIBBLE_DEFAULT);
  command->setVerticalDribble(V_DRIBBLE_DEFAULT);

  //go as fast as possible
  command->setControl(OMNI_FAST);

  float distToGo = dist(robotID,destPoint, *currentVisionData, *sp);
  float angleCurrent = getRotation(robotID, *currentVisionData, *sp);
  float angleFinal= angleBetween(destPoint,facePoint);
  float offsetDist=distToGo * (OFFSET_RATIO);
  float angleToFinal  = angleBetween(robotPoint,destPoint);
  

  float angleDiff=angleDifference(angleFinal,angleToFinal);
  Pair targetPoint;
  
  //if we're on the wrong side, put the angle to the side.
  float tAngle;
  if(fabs(angleDiff) <= PI/2){
    tAngle=angleFinal;
  }else{
    if(angleDiff>0)tAngle=angleToFinal+PI/2;
    else tAngle=angleToFinal-PI/2;
  }

  //calculate a target point to head to.
  targetPoint=Pair(destPoint.getX() - COS(tAngle)*offsetDist, destPoint.getY() - SIN(tAngle)*offsetDist);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 3 , targetPoint);

  float angleToTarget = angleBetween(robotPoint,targetPoint);
  angleDiff=angleDifference(angleCurrent,angleToTarget);

  // if we're right there, go straight to final destination
  if(dist(robotID,destPoint,*currentVisionData,*sp) <= CLOSENESS_THRESHOLD){
    command->setPos(targetPoint);
    command->setSpeed(BALL_POSSESSION_SPEED);
    angleDiff=angleDifference(angleCurrent,angleFinal);
    if(ABS(angleDiff) < ROT_RATE){
      command->setRotation(angleBetween(robotPoint,facePoint));
    }else{
      if (angleDiff>0){
        command->setRotation(angleCurrent-ROT_RATE);
      }else{
        command->setRotation(angleCurrent+ROT_RATE);
      }
    }
  }else if(ABS(angleDiff) < BALL_DRIBBLE_ANGLE){
    //go straight to target point
    command->setPos(targetPoint);
    command->setRotation(angleToFinal);
    //if (angleCurrent>angleFinal)command->setRotation(angleCurrent-ROT_RATE);
    //else if( angleCurrent<angleFinal)command->setRotation(angleCurrent+ROT_RATE);
    //else command->setRotation(angleCurrent);
  }else{
    //can't go directly, loop there
    float angleTravel; //angle to travel at.
    if (angleDiff>0){
      command->setRotation(angleCurrent-ROT_RATE);
      angleTravel=angleCurrent-BALL_DRIBBLE_ANGLE;
    }else{
      command->setRotation(angleCurrent+ROT_RATE);
      angleTravel=angleCurrent+BALL_DRIBBLE_ANGLE;
    }
    command->setPos(robotPoint.getX()+COS(angleTravel)*offsetDist,robotPoint.getY()+SIN(angleTravel)*offsetDist);
  }

}

///always true for now.
bool ManeuverWithBallSkill::evaluate() 
{
  return true;
}

//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void ManeuverWithBallSkill::update() 
{
  
}

///check if we've reached our target destination and rotation
bool ManeuverWithBallSkill::isFinished(float tolerance)
{
  return (angleDifference(angleBetween(getLocation(robotID,* visionModule->getCurrentFrame(),*sp),destPoint),
          getRotation(robotID,* visionModule->getCurrentFrame(),*sp))<=tolerance) 
          && dist(robotID,destPoint,* visionModule->getCurrentFrame(),*sp)<=tolerance;
}




///Reads in the values of the parameters from the skill's parameter text file.
void ManeuverWithBallSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/ManeuverSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  
  ///Read in a test param
  READ_FLOAT(BALL_DRIBBLE_ANGLE);
  READ_FLOAT(ROT_RATE);
  READ_FLOAT(OFFSET_RATIO);
  READ_FLOAT(CLOSENESS_THRESHOLD);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}