#include "BehindScreenOffenseSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/math/dist.h"


int BehindScreenOffenseSkill::skillNum = -1;


//===============================================================================
///Constructor.  Calls the base class Skill constructor
BehindScreenOffenseSkill::BehindScreenOffenseSkill(SystemParameters *sp,
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
BehindScreenOffenseSkill::~BehindScreenOffenseSkill() 
{
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool BehindScreenOffenseSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void BehindScreenOffenseSkill::initialize() 
{
  currentVisionData = visionModule->getCurrentFrame();
  initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void BehindScreenOffenseSkill::execute()
{    
  //----------------------------
	///ALWAYS call the following 2 functions to get the latest data
	currentVisionData = visionModule->getCurrentFrame();
	command = strategy->getCurrentFrame()->getDestination(robotID);
	///ALWAYS call the previous 2 functions to get the latest data
	//----------------------------  

	///If not active, dont do anything!
	if(!initialized) {
		strategy->getCurrentFrame()->setMessage(robotID, "BehindScreen not Initialized!");
    return;  
  }  
  

  command->setRotation(angleToBall(robotID,*currentVisionData,*sp));
  
  
  //0) Find aggressor.  If aggressor doesn't exist, find robot teammate to ball
  aggressor = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);
  if(aggressor == NO_ROBOT) {   
    aggressor = getTeammateClosestToBall();
    //Make sure that a teammate exists
    if(aggressor == NO_ROBOT) {
      strategy->getCurrentFrame()->setMessage(robotID, "NO Teammates found!");
      return;
    }
  }


  //1) Find opponent on same side as aggressor who is closest to aggressor but behind him.
  //If none exists, go to a location behind the aggressor, slightly offset in the y-dir.
  opponent = getOpponent();
  if(opponent == NO_ROBOT) 
  {
    //printf("thresh = %f", SWITCH_THRESH);
    strategy->getCurrentFrame()->setMessage(robotID, "Going to default loc");
    aggLoc = getLocation(aggressor,*currentVisionData,*sp);
    command->setXPos(aggLoc.getX() - 2*sp->general.PLAYER_RADIUS - DEF_X_OFFSET);
    
    //Historesis: Check to see which side we should be on, and if we should switch
    if(side == LEFT && aggLoc.getY() < sp->field.SPLIT_LINE - SWITCH_THRESH)
      side = RIGHT;
    else if(side == RIGHT && aggLoc.getY() > sp->field.SPLIT_LINE + SWITCH_THRESH)
      side = LEFT;
    
    //Go to default location behind aggressor, offset in the y-dir based on the 
    //which side we are on
    if(side == RIGHT)
    {
        command->setYPos(aggLoc.getY() + DEF_Y_OFFSET);
    }
    else
    {
        command->setYPos(aggLoc.getY() - DEF_Y_OFFSET);
    }
  }
  //2) Call SimpleScreenSkill on that opponent
  else
  {
    //Initialize the skill to:
    //-screen opponent robot
    //-from the ball
    //-and look toward the ball at all times
    //-with grace distance 0.05 meters
    simpleScreenSkill->initialize(SCREEN_OPPONENT_ROBOT, 
                                  opponent, 
                                  SCREEN_BALL, 
                                  0, 
                                  SCREEN_BALL, 
                                  0);
    strategy->getCurrentFrame()->setMessage(robotID, "Screening robot");
    simpleScreenSkill->run();
  }
}



//===============================================================================
//Find opponent on same side as aggressor who is closest to aggressor 
//(in x-direction) but behind him
RobotIndex BehindScreenOffenseSkill::getOpponent() 
{
  currentVisionData = visionModule->getCurrentFrame();

  RobotIndex opp = NO_ROBOT;
  Pair oppLoc;

  float distance = -1.0f;

  aggressor = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);
  aggLoc = getLocation(aggressor,*currentVisionData,*sp);

  //Look at each robot to see if it exists
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) 
  {   
    if( robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) ) 
    {
      oppLoc = getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
      if( 
          //opponent behind aggressor
          oppLoc.getX() < aggLoc.getX() - 2*sp->general.PLAYER_RADIUS && 
          //opponent on same side of split line as aggressor
          (
            (oppLoc.getY() > sp->field.SPLIT_LINE && aggLoc.getY() > sp->field.SPLIT_LINE) || 
            (oppLoc.getY() <= sp->field.SPLIT_LINE && aggLoc.getY() <= sp->field.SPLIT_LINE)
          ) &&
          //opponent not too close and not too far away from aggressor
          dist(oppLoc, aggLoc) < MAX_SCREEN_DISTANCE &&
          dist(oppLoc, aggLoc) > MIN_SCREEN_DISTANCE &&
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
RobotIndex BehindScreenOffenseSkill::getTeammateClosestToBall() {
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
bool BehindScreenOffenseSkill::evaluate() 
{
	return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void BehindScreenOffenseSkill::update() 
{
}




//===============================================================================
///test skill goes forever
bool BehindScreenOffenseSkill::isFinished(float tolerance)
{
  return false;
}




//===============================================================================
void BehindScreenOffenseSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/BehindScreenOffenseSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  

  //READ_FLOAT(GOAL_LINE_OFFSET);
  //READ_FLOAT(SIDELINE_OFFSET);

  READ_FLOAT(DEF_X_OFFSET);
  READ_FLOAT(DEF_Y_OFFSET);
  READ_FLOAT(SCREEN_DIST);
  READ_FLOAT(SWITCH_THRESH);
  READ_FLOAT(MAX_SCREEN_DISTANCE);
  READ_FLOAT(MIN_SCREEN_DISTANCE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
