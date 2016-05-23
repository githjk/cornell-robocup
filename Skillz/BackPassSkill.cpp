#include "BackPassSkill.h"
//#include "CrossCreasCreatorSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/pair/pair.h"
#include "common/util/math/cmath.h"
#include "common/util/math/dist.h"
#include "common/util/intersect/intersect.h"
#include "common/util/shoot/shoot.h"
#include "common/util/math/angle.h"


int BackPassSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
BackPassSkill::BackPassSkill(SystemParameters *sp,
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
bool BackPassSkill::isValid() 
{ 
  
 
  //Get the aggressor location
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR); 
  Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp));
  Pair creator(getLocation(robotID,*currentVisionData,*sp));
  
  bool isPass=false;

  
  //In no pass, check out where the creator currently is
  if(!isPass ){
    isPass = calcYShot(aggressorID,sp->field.HALF_LINE,FACTOR*sp->strategy2002.PASS_LANE_THRESH,
      sp->field.THEIR_GOALIE_BOX,sp->field.OFFENSE_ZONE_LINE,robotID,*currentVisionData,*sp,&creatorPos);
  }
  //In no pass, check out where the creator currently is
  /*if(!isPass ){
    isPass = calcShot(aggressorID,creator.getX(),FACTOR*sp->strategy2002.PASS_LANE_THRESH,
      creator.getY()-CLOSE_BOUND,creator.getY()+CLOSE_BOUND,robotID,*currentVisionData,*sp,&creatorPos);
  }*/

  isPass = isPass && creator.distanceTo(creatorPos) <= PASS_DIST;
  /// Set the pass destination
  if(isPass)
  {
    if(!initialized){
      strategy->getCurrentRoboCupFrame()->setPassDest(robotID,creatorPos);
      strategy->getCurrentRoboCupFrame()->setPassValue(robotID,true);
    }
  }


  ///If rotation of aggressor is correct and if pass can be received skill and creator is close enough, is valid
  return(isPass );

 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void BackPassSkill::initialize() 
{
  timer->resetTimer();
  initialized=false;
  if(!isValid()){//set pass destination
    GUI_Record.debuggingInfo.addDebugMessage("No Pass found for Back Pass.");
  }
  initialized = true;

    
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void BackPassSkill::execute()
{    
  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("BackPassSkill not initialized!");    
    return;  
  }
  //This skill should do the same thing as Cross Crease, namely, head to the pass dest.
  strategy->getCurrentRoboCupFrame()->setPassDest(robotID,creatorPos);
  strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Getting ready for pass.");  
  
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR); 
  Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp));

  float angle=angleBetween(creatorPos,aggressorPos);
  command->setPos(robotPositionGivenFront(creatorPos,angle,*sp));
  command->setRotation(angle);
  
}

///If the timer goes past 3 secs, reverse rotation
bool BackPassSkill::evaluate() 
{
  return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void BackPassSkill::update() 
{
  ///If not initialized, dont do anything!
  if(!initialized) return;    
}



//===============================================================================
///test skill goes forever
bool BackPassSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void BackPassSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/BackPassSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(OFFSET);
  READ_FLOAT(MIN_WIDTH);
  READ_FLOAT(MAXIMUM_ANGLE);
  READ_FLOAT(FACTOR);
  READ_FLOAT(GOALIE_DISTANCE_THRESHOLD);
  READ_FLOAT(BOUND_FACTOR);
  READ_FLOAT(PASS_DIST);
  READ_FLOAT(CLOSE_BOUND);
  READ_FLOAT(MIN_DIST);
  READ_FLOAT(ANGLE_LIMIT);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}

///=============================================================================

float BackPassSkill::calculateGoalieEffect()
{


  /// How much we will like the creator to backoff
  float moveBack = 0.0f;
  
  
  /// Find the opponent which is closest to the goal point   


     Pair goalPoint(sp->field.THEIR_GOAL_LINE,(sp->field.LEFT_GOAL_POST+sp->field.RIGHT_GOAL_POST+2)/2);
       float closest = 32000.0f;
	   RobotIndex closeRobot = NO_ROBOT;
       
	   Pair robot=getLocation(robotID,*currentVisionData,*sp);
       for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++){
          Pair C(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));
		  if(dist(C,goalPoint) < closest)
		  {
			  closest = dist(C,goalPoint);
			  closeRobot = i;
		  }
	   }
  
	  
	   /// If the distance of our robot from the closest robot 
	   /// is greater than certain Threshold
	  
	   if(closeRobot != NO_ROBOT)
	   {
		   Pair closeRobotLocation(getLocation(sp->general.OTHER_TEAM,closeRobot,*currentVisionData));
		   if(dist(robot,closeRobotLocation) < GOALIE_DISTANCE_THRESHOLD)
		   {
          moveBack = GOALIE_DISTANCE_THRESHOLD - dist(robot,closeRobotLocation);
			   
       }

     }
     return moveBack;

}