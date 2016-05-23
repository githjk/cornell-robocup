#include "MidReboundOffenseSkill.h"

#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/math/dist.h"
#include "common/util/shoot/shoot.h"

int MidReboundOffenseSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
MidReboundOffenseSkill::MidReboundOffenseSkill(SystemParameters *sp,
                                               VisionModule *vm, 
                                               RoboCupStrategyModule *strat,
                                               RobotIndex id, 
                                               SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();
  initialized = false;
  simpleScreenSkill = (SimpleScreenSkill*)set->getSkill(SimpleScreenSkill::skillNum);
}



//===============================================================================
//Destructor deletes nothing 
MidReboundOffenseSkill::~MidReboundOffenseSkill() {
}



//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool MidReboundOffenseSkill::isValid() { 
  return true; 
}




//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void MidReboundOffenseSkill::initialize() {
  initialized = true;
}




//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void MidReboundOffenseSkill::execute() {    
  //----------------------------
	///ALWAYS call the following 2 functions to get the latest data
	currentVisionData = visionModule->getCurrentFrame();
	command = strategy->getCurrentFrame()->getDestination(robotID);
	///ALWAYS call the previous 2 functions to get the latest data
	//----------------------------

	///If not active, dont do anything!
  if(!initialized){
		return;  
  }
  
  //Get opponent to screen
	opponent = getOpponent();
  if(opponent == NO_ROBOT) {
    strategy->getCurrentFrame()->setMessage(robotID, "MidRebound: No opponent found!");
    return;
  }

  //Screen opponent
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
  simpleScreenSkill->run();
  strategy->getCurrentFrame()->setMessage(robotID, "MidRebound: Screening robot");

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=
  Pair ballLoc(strategy->getCurrentRoboCupFrame()->getOffensiveBallLocation());
  Pair centerGoal(sp->field.THEIR_GOAL_LINE, sp->field.SPLIT_LINE);
  command->setRotation(findshootAngle(command->getPos(),
                                      ballLoc,
                                      centerGoal,
                                      0.5f));

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=
  //if the angle is between 3PI/8 and -3PI/8 arm the kicker
  if(
      getRotation(robotID, *currentVisionData, *sp) < 3.0f*PI/8.0f &&
      getRotation(robotID, *currentVisionData, *sp) > -3.0f*PI/8.0f
    )
  {
      if(ABS(command->getRotation()) < (PI/2)*KICK_RATIO)
      {command->setKick(KICK_SHOT);}
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=
}




//===============================================================================
bool MidReboundOffenseSkill::evaluate() {
	return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void MidReboundOffenseSkill::update() {
}




//===============================================================================
///test skill goes forever
bool MidReboundOffenseSkill::isFinished(float tolerance){
  return false;
}




//===============================================================================
void MidReboundOffenseSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/MidReboundOffenseSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  

  READ_FLOAT(UPPER_LIMIT);
  READ_FLOAT(LOWER_LIMIT);
  READ_FLOAT(RIGHT_LIMIT);
  READ_FLOAT(LEFT_LIMIT);
  READ_FLOAT(KICK_RATIO);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
//Find the opponent farthest upfield who is below the aggressor and creator
RobotIndex MidReboundOffenseSkill::getOpponent() {
  currentVisionData = visionModule->getCurrentFrame();

  RobotIndex opp = NO_ROBOT;
  Pair oppLoc;
  float left, right, upper;

  float maxX = -1.0f;
  opp = NO_ROBOT;
  creator = strategy->getCurrentRoboCupFrame()->getRobotByPosition(CREATOR);
  aggressor = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);

  //initialize the upper, right, and left boundaries
  //Both robots exist
  if(creator != NO_ROBOT && aggressor != NO_ROBOT) {
    creatorLoc = getLocation(creator,*currentVisionData,*sp);
    aggLoc = getLocation(aggressor,*currentVisionData,*sp);
    upper = MIN(creatorLoc.getX(), aggLoc.getX());
    if(creatorLoc.getY() > aggLoc.getY()) {
      right = aggLoc.getY();
      left = creatorLoc.getY();
    }
    else {
      left = aggLoc.getY();
      right = creatorLoc.getY();
    }
  }
  //Only aggressor exists
  else if(creator == NO_ROBOT && aggressor != NO_ROBOT) {
    aggLoc = getLocation(aggressor,*currentVisionData,*sp);
    upper = aggLoc.getX();
    if(aggLoc.getY() > sp->field.SPLIT_LINE) {
      left = aggLoc.getY();
      right = sp->field.RIGHT_SIDE_LINE + RIGHT_LIMIT;
    }
    else {
      right = aggLoc.getY();
      left = sp->field.LEFT_SIDE_LINE - LEFT_LIMIT;
    }
  }   
  //Only creator exists
  else if(creator != NO_ROBOT && aggressor == NO_ROBOT) {
    creatorLoc = getLocation(creator,*currentVisionData,*sp);
    upper = creatorLoc.getX();
    if(creatorLoc.getY() > sp->field.SPLIT_LINE) {
      left = creatorLoc.getY();
      right = sp->field.RIGHT_SIDE_LINE + RIGHT_LIMIT;
    }
    else {
      right = creatorLoc.getY();
      left = sp->field.LEFT_SIDE_LINE - LEFT_LIMIT;
    }
  }
  //neither exist
  else if(creator == NO_ROBOT && aggressor == NO_ROBOT) {
    upper = sp->field.THEIR_GOAL_LINE - UPPER_LIMIT;
    right = sp->field.RIGHT_SIDE_LINE + RIGHT_LIMIT;
    left = sp->field.LEFT_SIDE_LINE - LEFT_LIMIT;
  }


  creatorLoc = getLocation(creator,*currentVisionData,*sp);
  aggLoc = getLocation(aggressor,*currentVisionData,*sp);

  //always check if there is a better opponent
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) 
  {   
	  if( robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) ) 
	  {
		  oppLoc = getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
		  if( 
			  //opponent below upper limit
			  oppLoc.getX() < upper
        //robot above lower limit
        && oppLoc.getX() > sp->field.HALF_LINE + LOWER_LIMIT
        //between the right and left limits
        && oppLoc.getY() < left
        && oppLoc.getY() > right
			  //this opponent is closer than last opponent chosen
			  &&(maxX == -1.0f || oppLoc.getX() > maxX)
			  )
		  {         
			  //Update farthest upfield opponent
			  opp = i;
			  distance = oppLoc.getX();
		  }
	  }
  }
  return opp;
}