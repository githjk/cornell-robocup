#include "CrossCreaseCreatorSkill.h"
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


int CrossCreaseCreatorSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
CrossCreaseCreatorSkill::CrossCreaseCreatorSkill(SystemParameters *sp,
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
bool CrossCreaseCreatorSkill::isValid() 
{ 
  
 
  //Get the aggressor location
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR); 
  Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp));
  Pair creator(getLocation(robotID,*currentVisionData,*sp));
  
  bool isPass;

  //Chooisng the passLine
  if(aggressorPos.getY() > sp->field.SPLIT_LINE)
  {
       passLine = sp->field.RIGHT_GOAL_POST - OFFSET;
       boundaryLine = sp->field.RIGHT_GOALIE_BOX-sp->general.PLAYER_RADIUS - OFFSET;
  }
  else
  {
       passLine = sp->field.LEFT_GOAL_POST + OFFSET;
       boundaryLine = sp->field.LEFT_GOALIE_BOX + sp->general.PLAYER_RADIUS + OFFSET;
  }


  /** Picking bounds for the passLine*/

  /// left boundary
  topBound = sp->field.THEIR_GOAL_LINE;

  /// The right boundary will be a function of aggressor's position
  bottomBound = sp->field.THEIR_GOAL_LINE - 2*sp->general.PLAYER_RADIUS - MIN_WIDTH;
  float diff = aggressorPos.getX()- sp->field.HALF_LINE;

  if(diff > 0)
  { bottomBound = bottomBound - BOUND_FACTOR*diff; }

  /// Check if there is a pass
  float halfWidth;
  isPass = (calcYShot(aggressorID,passLine,FACTOR*sp->strategy2002.PASS_LANE_THRESH,bottomBound,topBound,robotID,*currentVisionData,*sp,&creatorPos,&halfWidth)
           && 
           ABS( angleDifference(getRotation(aggressorID,*currentVisionData,*sp),angleBetween(aggressorPos,creatorPos))) < PI/MAXIMUM_ANGLE &&
           creatorPos.distanceTo(creator) < PASS_DIST);
  
    //show the bounds we're using
  GUI_Record.debuggingInfo.setDebugPoint(robotID,7,topBound,passLine);
  GUI_Record.debuggingInfo.setDebugPoint(robotID,8,bottomBound,passLine);

    //adjust the position to the top edge of the open area.
  creatorPos.setX(creatorPos.getX()+halfWidth/2.0f);
  
  /// Set the pass destination
  if(isPass) 
  {
    if(!initialized){
      strategy->getCurrentRoboCupFrame()->setPassDest(robotID,creatorPos);
      strategy->getCurrentRoboCupFrame()->setPassValue(robotID,true);
    }
  }


  /// Caluclate the boundary point
  Line AC(aggressorPos,creatorPos);
  Line boundary(0.0f,boundaryLine,1.0f,boundaryLine);
  findLineIntersection(AC,boundary,&boundaryPoint);

  ///If rotation of aggressor is correct and if pass can be received skill and creator is close enough, is valid
  return(isPass);

 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void CrossCreaseCreatorSkill::initialize() 
{
  timer->resetTimer();
  initialized=false;
  if(!isValid()){//set pass destination
    GUI_Record.debuggingInfo.addDebugMessage("No Cross found for Cross Crease Pass.");
  }
  initialized = true;

    
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void CrossCreaseCreatorSkill::execute()
{    
  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("CrossCreaseCreatorSkill not initialized!");    
    return;  
  }
  //show the bounds we're using
  GUI_Record.debuggingInfo.setDebugPoint(robotID,7,topBound,passLine);
  GUI_Record.debuggingInfo.setDebugPoint(robotID,8,bottomBound,passLine);

  /** The process of finding the rotation */

  //Get the aggressor location
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR); 
  Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp));

  // try to find where our shot is
  bool isShot;
  Pair goalPos;
  isShot = calcShot(robotID,sp->field.THEIR_GOAL_LINE,sp->strategy2002.SHOOT_LANE_THRESH,sp->field.RIGHT_GOAL_POST,sp->field.LEFT_GOAL_POST,
  NO_ROBOT,*currentVisionData,*sp,&goalPos);

  if(!isShot)
  {
    goalPos.setX(sp->field.THEIR_GOAL_LINE);
    goalPos.setY((sp->field.LEFT_GOAL_POST + sp->field.RIGHT_GOAL_POST)/2);
  }



  /// Rotation will be split angle and the kicker will be ON
  float angle;
  angle = findshootAngle(creatorPos,goalPos,aggressorPos,0.5f);

  
  /// Make sure creator does not bang into wall
  if(creatorPos.getX() > (sp->field.THEIR_GOAL_LINE - sp->general.PLAYER_RADIUS - sp->general.BALL_RADIUS))
  {
    creatorPos.setX(sp->field.THEIR_GOAL_LINE - sp->general.PLAYER_RADIUS - sp->general.BALL_RADIUS);
  }
  

  /// Check if we are too close to goalie and make necessary changes
 
  float moveback = calculateGoalieEffect();
  Pair possiblePoint;
  extendPoint(aggressorPos,creatorPos,moveback,possiblePoint);

  if(dist(aggressorPos,possiblePoint) > dist(aggressorPos, boundaryPoint))
  {
    creatorPos = boundaryPoint;
  }
  else
  {
    creatorPos = possiblePoint;
  }
  /// Set the passing value 
  
  strategy->getCurrentRoboCupFrame()->setPassDest(robotID,creatorPos);
  GUI_Record.debuggingInfo.setDebugPoint(robotID,0,creatorPos);
  strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Getting ready for cross crease pass.");    

  /// If the chosen point is bad then go to some safe deafult point
  if(creatorPos.getY() == passLine)
  {
    
     command->setPos(robotPositionGivenFront(creatorPos,angle,*sp));
  }
  else
  {
    command->setXPos(aggressorPos.getX());
    command->setYPos(-(aggressorPos.getY()));
  }
  
  /// Turn the kicker on and orient yourself to shoot

  command->setRotation(angle);
  if(ABS(command->getRotation()) < (PI/2)*KICK_RATIO)
  {command->setKick(KICK_SHOT);}
}

///If the timer goes past 3 secs, reverse rotation
bool CrossCreaseCreatorSkill::evaluate() 
{
  return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void CrossCreaseCreatorSkill::update() 
{
  ///If not initialized, dont do anything!
  if(!initialized) return;    
}



//===============================================================================
///test skill goes forever
bool CrossCreaseCreatorSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void CrossCreaseCreatorSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/CrossCreaseCreatorSkillParams.txt", ios::nocreate);
  
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
  READ_FLOAT(KICK_RATIO);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}

///=============================================================================

float CrossCreaseCreatorSkill::calculateGoalieEffect()
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