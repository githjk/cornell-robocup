#include "StiffarmOffenseSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/pair/pair.h"
int StiffarmOffenseSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
StiffarmOffenseSkill::StiffarmOffenseSkill(SystemParameters *sp,
                                           VisionModule *vm, 
                                           RoboCupStrategyModule *strat,
                                           RobotIndex id, 
                                           SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();
  initialized = false;
  side = LEFT;
  simpleScreenSkill = (SimpleScreenSkill*)set->getSkill(SimpleScreenSkill::skillNum);
}
//===============================================================================
StiffarmOffenseSkill::~StiffarmOffenseSkill() 
{
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool StiffarmOffenseSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void StiffarmOffenseSkill::initialize() 
{
  initialize(AGGRESSOR);
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void StiffarmOffenseSkill::initialize(Position position) 
{
  initialized = true;
  lastOpponent = NO_ROBOT;
  friendPosition=position;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void StiffarmOffenseSkill::execute()
{    

	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

	  
  
  //0) Find aggressor.  If aggressor doesn't exist, find robot teammate to ball
  aggressor = strategy->getCurrentRoboCupFrame()->getRobotByPosition(friendPosition);
  if(aggressor == NO_ROBOT) {   
    aggressor = getTeammateClosestToBall();
    //Make sure that a teammate exists
    if(aggressor == NO_ROBOT) {
      strategy->getCurrentFrame()->setMessage(robotID, "SA: NO Teammates found!");
      return;
    }
  }


  //1) Find opponent on same side as aggressor who is closest to aggressor but in front him.
  //If none exists, go to a location behind the aggressor, slightly offset in the y-dir.
  opponent = getOpponent();
  if(opponent == NO_ROBOT) 
  {
    strategy->getCurrentFrame()->setMessage(robotID, "SA: Going to default loc");
    aggLoc = getLocation(aggressor,*currentVisionData,*sp);
    
    
    // The goal point
    Pair goalPos(sp->field.THEIR_GOAL_LINE,(sp->field.LEFT_GOAL_POST+sp->field.RIGHT_GOAL_POST)/2.0f);
    
    // Extension Point
    Pair extensionPoint;
    //Line between the aggressor and creator
    extendPoint(goalPos,aggLoc,-(2*sp->general.PLAYER_RADIUS + DEF_X_OFFSET),extensionPoint);

    GUI_Record.debuggingInfo.setDebugPoint(robotID,1,extensionPoint);
    // Perpendicular point
    Pair perpendicularPoint ;
    // Intended defaultPos
    Pair defaultPos ;
    
    //Historesis: Check to see which side we should be on, and if we should switch
    if(side == LEFT && aggLoc.getY() < sp->field.SPLIT_LINE - SWITCH_THRESH)
      side = RIGHT;
    else if(side == RIGHT && aggLoc.getY() > sp->field.SPLIT_LINE + SWITCH_THRESH)
      side = LEFT;

    //Go to default location in front of aggressor, offset in the y-dir based on the 
    //which side we are on
    if( side == RIGHT )
    {  rotateAboutPoint(extensionPoint,goalPos,-PI/2,perpendicularPoint);}
    else 
    {  rotateAboutPoint(extensionPoint,goalPos,PI/2,perpendicularPoint);}

    GUI_Record.debuggingInfo.setDebugPoint(robotID,2,perpendicularPoint);
    extendPoint(perpendicularPoint,extensionPoint,-DEF_Y_OFFSET,defaultPos);
    
    command->setPos(defaultPos);
    command->setRotation(angleToBall(robotID,*currentVisionData,*sp));
    


    return;
  }


  //2) Call SimpleScreenSkill on that opponent
  lastOpponent = opponent;
  //Initialize the skill to:
  //-screen opponent robot
  //-from the ball
  //-and look toward the ball at all times
  //-with grace distance 0.05 meters
  simpleScreenSkill->initialize(SCREEN_OPPONENT_ROBOT, 
                                opponent, 
                                SCREEN_FRIENDLY_ROBOT, 
                                aggressor, 
                                SCREEN_BALL, 
                                0);
  strategy->getCurrentFrame()->setMessage(robotID, "SA: Screening robot");
  simpleScreenSkill->run();

  //Look at ball for a quick pass
  command->setRotation(angleToBall(robotID,*currentVisionData,*sp));
}



//===============================================================================
//Find opponent on same side as aggressor who is closest to aggressor 
//(in x-direction) but in front of him
RobotIndex StiffarmOffenseSkill::getOpponent() {
  currentVisionData = visionModule->getCurrentFrame();

  RobotIndex opp = NO_ROBOT;
  Pair oppLoc;

  float distance = -1.0f;

  aggressor = strategy->getCurrentRoboCupFrame()->getRobotByPosition(friendPosition);
  aggLoc = getLocation(aggressor,*currentVisionData,*sp);
  oppLoc = getLocation(sp->general.OTHER_TEAM,lastOpponent,*currentVisionData);

  //if last oppent screened 1.) exists and 2.) is within tolerance, keep using him
  if(
	  lastOpponent != NO_ROBOT &&
	  robotFound(sp->general.OTHER_TEAM, lastOpponent, *currentVisionData, *sp) &&
	  dist(oppLoc, aggLoc) < MAX_SCREEN_DISTANCE + MIN_MAX_THRESH &&
	  dist(oppLoc, aggLoc) > MIN_SCREEN_DISTANCE - MIN_MAX_THRESH
	 )
  {	
	opp = lastOpponent;
  }

  //always check if there is a better opponent
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) 
  {   
	  if( robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) ) 
	  {
		  oppLoc = getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
		  if( 
			  //opponent in front of aggressor
			  oppLoc.getX() > aggLoc.getX() + 2*sp->general.PLAYER_RADIUS && 
			  //opponent on same side of split line as aggressor
			  (
			  (oppLoc.getY() > sp->field.SPLIT_LINE && aggLoc.getY() > sp->field.SPLIT_LINE) || 
			  (oppLoc.getY() <= sp->field.SPLIT_LINE && aggLoc.getY() <= sp->field.SPLIT_LINE)
			  ) &&
			  //opponent not too close and not too far away from aggressor
			  dist(oppLoc, aggLoc) < MAX_SCREEN_DISTANCE - MIN_MAX_THRESH &&
			  dist(oppLoc, aggLoc) > MIN_SCREEN_DISTANCE + MIN_MAX_THRESH &&
			  //this opponent is closer than last opponent chosen
			  (distance == -1.0f || dist(oppLoc, aggLoc) < distance)
			  )
		  {         
			  //Update closest opponent to aggressor if he is closer
			  opp = i;
			  distance = dist(oppLoc, aggLoc);
		  }
	  }
  }
  return opp;
}




//===============================================================================
//Finds teammate closest to ball!
RobotIndex StiffarmOffenseSkill::getTeammateClosestToBall() {
  currentVisionData = visionModule->getCurrentFrame();

  RobotIndex mate = NO_ROBOT;
  float distance = 1000;

  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) { 
    if( robotFound(i,*currentVisionData,*sp) && i != robotID) {
      if(distToRoboCupBall(i,*currentVisionData,*sp) < distance) {
        distance = distToRoboCupBall(i,*currentVisionData,*sp);
        mate = i;
      }
    }
  }
  return mate;
}



//===============================================================================
bool StiffarmOffenseSkill::evaluate() 
{
	return true;
}



//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void StiffarmOffenseSkill::update() 
{
}



//===============================================================================
///test skill goes forever
bool StiffarmOffenseSkill::isFinished(float tolerance)
{
  return false;
}



//===============================================================================
void StiffarmOffenseSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/StiffarmOffenseSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  
  READ_FLOAT(DEF_X_OFFSET);
  READ_FLOAT(DEF_Y_OFFSET);
  READ_FLOAT(SWITCH_THRESH);
  READ_FLOAT(MAX_SCREEN_DISTANCE);
  READ_FLOAT(MIN_SCREEN_DISTANCE);
  READ_FLOAT(MIN_MAX_THRESH);
  READ_FLOAT(PUSH_FOWARD_DISTANCE);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
