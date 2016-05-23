#include "SpecialOpCageDefenseSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/math/dist.h"

int SpecialOpCageDefenseSkill::skillNum = -1;




//===============================================================================
///Constructor.  Calls the base class Skill constructor
SpecialOpCageDefenseSkill::SpecialOpCageDefenseSkill(SystemParameters *sp,
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
//Skill is valid if the ball is found
bool SpecialOpCageDefenseSkill::isValid() { 
  //ALWAYS call the following function to get the latest data
	currentVisionData = visionModule->getCurrentFrame();
	
  return isRobocupBallFound(*currentVisionData);
}




//===============================================================================
//Perform any initializations for the skill
void SpecialOpCageDefenseSkill::initialize() {  
  initialized = true;
}




//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void SpecialOpCageDefenseSkill::execute() {    
  //----------------------------
	///ALWAYS call the following 2 functions to get the latest data
	currentVisionData = visionModule->getCurrentFrame();
	command = strategy->getCurrentFrame()->getDestination(robotID);
	///ALWAYS call the previous 2 functions to get the latest data
	//---------------------------

	///If not active, dont do anything!
	if(!initialized) {
		return;  
  }

	//1) Find opponent closest to the ball who does not have possession outside of 
  //the death zone. 
  opponent = getOpponentClosestToBall();
  if(opponent == NO_ROBOT) {
    strategy->getCurrentFrame()->setMessage(robotID, "Going to default loc");
    command->setXPos(sp->field.OUR_GOAL_LINE + GOAL_LINE_OFFSET);
    if( strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation().getY() < 0.0f)
      command->setYPos(sp->field.LEFT_SIDE_LINE - SIDELINE_OFFSET);
    else 
      command->setYPos(sp->field.RIGHT_SIDE_LINE + SIDELINE_OFFSET);
    return;
  }


  //2) Call SimpleScreenSkill on that opponent

  //Initialize the skill to:
  //-screen opponent robot 
  //-from ball
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




//===============================================================================
//Finds opponent closest to the ball who does not have possession outside of 
//the death zone. Returns NO_ROBOT if none are found
RobotIndex SpecialOpCageDefenseSkill::getOpponentClosestToBall() {
  RobotIndex opp = NO_ROBOT;
  float dist = 1000;

  currentVisionData = visionModule->getCurrentFrame();
  //Look at each robot to see if it exists and has possession
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) {    
    if( robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) 
      && getTheirXLocation(i,*currentVisionData,*sp) > sp->field.DEATH_ZONE_LINE) {
      //Update closest opponent to ball if he is closer
      if(distToRoboCupBall(sp->general.OTHER_TEAM,i,*currentVisionData) < dist
        && !opponentHasPossession(i, *sp) ) {
        opp = i;
        dist = distToRoboCupBall(sp->general.OTHER_TEAM,i,*currentVisionData);
      }      
    }
  }
  
  return opp;
}




//===============================================================================
bool SpecialOpCageDefenseSkill::evaluate() {
	return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void SpecialOpCageDefenseSkill::update() {
}




//===============================================================================
///test skill goes forever
bool SpecialOpCageDefenseSkill::isFinished(float tolerance) {
  return false;
}




//===============================================================================
void SpecialOpCageDefenseSkill::loadValues() {
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/SpecialOpCageDefenseSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  
  READ_FLOAT(GOAL_LINE_OFFSET);
  READ_FLOAT(SIDELINE_OFFSET);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
