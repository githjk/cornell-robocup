#include "CutGoalSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/math/cmath.h"
#include "common/util/pair/pair.h"
#include "GUI/GUI.h"


int CutGoalSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
CutGoalSkill::CutGoalSkill(SystemParameters *sp,
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
bool CutGoalSkill::isValid() 
{ 
  return true;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void CutGoalSkill::initialize(float firstPoint, float secondPoint, float UPPER_LIMIT, float LOWER_LIMIT, Pair ballPos){
  initialize(firstPoint,secondPoint,UPPER_LIMIT,LOWER_LIMIT);
  presetBall=true;
  ball=ballPos;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void CutGoalSkill::initialize(float firstPoint, float secondPoint, float UPPER_LIMIT, float LOWER_LIMIT)
{
	//reset timer
	timer->resetTimer();

  presetBall=false;
	
	point1=firstPoint;
	point2=secondPoint;
  if(UPPER_LIMIT < LOWER_LIMIT)
  {
	  LOWER_X = UPPER_LIMIT;
	  UPPER_X = LOWER_LIMIT;
  }else{
    UPPER_X = UPPER_LIMIT;
    LOWER_X = LOWER_LIMIT;
  }
  initialized = true;
}
//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void CutGoalSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
	}
	else
	{
		//grab the ball location
		
    if(!presetBall){
		  ball = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();
    }
		

    ///Calculate the angle bisector of the area we want to cover
    float theta;
	  float theta1=angleBetween(sp->field.OUR_GOAL_LINE,point1,ball.getX(),ball.getY());
    float theta2=angleBetween(sp->field.OUR_GOAL_LINE,point2,ball.getX(),ball.getY());
    theta=(theta1+theta2)/2.0f;
    theta=normalizeAngle(theta+PI);
    float halfAngle=ABS(angleDifference(theta1,theta2)/2.0f);

    //calculate midpoint to extend from
		Pair midpoint;
		midpoint.setY((sp->field.OUR_GOAL_LINE-ball.getX()) * TAN(theta) + ball.getY());
	  midpoint.setX(sp->field.OUR_GOAL_LINE);

    /*debugging helpful stuff
    GUI_Record.debuggingInfo.setDebugPoint(robotID,6,midpoint);   
    GUI_Record.debuggingInfo.setDebugPoint(robotID,7,sp->field.OUR_GOAL_LINE,point1);   
    GUI_Record.debuggingInfo.setDebugPoint(robotID,8,sp->field.OUR_GOAL_LINE,point2);   
    Pair ang1(ball.getX()+.2f,ball.getY());
    Pair ang2(ball.getX()+.2f,ball.getY());
    rotateAboutPoint(ang1,ball,theta1,ang1);
    rotateAboutPoint(ang2,ball,theta2,ang2);
    GUI_Record.debuggingInfo.setDebugPoint(robotID,3,ang1);   
    GUI_Record.debuggingInfo.setDebugPoint(robotID,4,ang2);  
    
    Pair t(ball.getX()+.2f,ball.getY());
    rotateAboutPoint(t,ball,theta,t);
    GUI_Record.debuggingInfo.setDebugPoint(robotID,5,t);  
    */

	  // The ideal point we want the robot to be in this circumstances
	  Pair dest;
	  float distance = sp->general.PLAYER_RADIUS / SIN(ABS(halfAngle)) ;
	  //char msg[80]; sprintf(msg,"dist: %5.2f",distance);GUI_Record.debuggingInfo.addDebugMessage(msg);


	  extendPoint(midpoint,ball,-distance,dest);

	  // We have to check if the destination point is between the Upper and lower limit
	  //	  float slope =  (midpoint.getY() - ball.getY()) / (midpoint.getX() - ball.getY()) ;
	  // If it is above the limit
	  if(dest.getX() > UPPER_X){
	    extrapolateForY(midpoint,ball,UPPER_X,dest);
	  }
	  // If it is below the limit
	  if(dest.getX() < LOWER_X){
	    extrapolateForY(midpoint,ball,LOWER_X,dest);
	  }
	  command->setControl(OMNI_NORMAL);
	  command->setPos(dest);

	  command->setRotation(angleBetween(getLocation(robotID, *currentVisionData, *sp),
                                      ball));
      
    strategy->getCurrentFrame()->setMessage(robotID,"Covering Goal");

	}
}
//=====================================================
///If the timer goes past 3 secs, reverse rotation
bool CutGoalSkill::evaluate() 
{
	return true;
}



//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void CutGoalSkill::update() 
{
	///at this time simple screening does not "learn"
}


//Screen is never finished, it can always continue
bool CutGoalSkill::isFinished(float tolerance){
	return false;
}




///Reads in the values of the parameters from the skill's parameter text file.
void CutGoalSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/CutGoalSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
	
	//default X position if not specified when skill initialized or the X position is not within the range required
	//READ_FLOAT(DEFAULT_DEFENSE_X);		
	
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}