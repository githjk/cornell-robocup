#include "SpecialOpBlockSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/info/robot/robot.h"
#include "common/util/shoot/shoot.h"
#include "common/util/math/dist.h"

int SpecialOpBlockSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
SpecialOpBlockSkill::SpecialOpBlockSkill(SystemParameters *sp,
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
bool SpecialOpBlockSkill::isValid() 
{ 
	//----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
	currentVisionData = visionModule->getCurrentFrame();
	///ALWAYS call the previous 2 functions to get the latest data
	//----------------------------

  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void SpecialOpBlockSkill::initialize()
{
  currentVisionData = visionModule->getCurrentFrame();
  
  //find opponent
  opponent = getOpponent();
  if(opponent == NO_ROBOT) {
    strategy->getCurrentFrame()->setMessage(robotID, "SpecOpBlock beginning execution");
    return;
  }
  
	initialized = true;
}
//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void SpecialOpBlockSkill::execute()
{		 
  //----------------------------
	///ALWAYS call the following 2 functions to get the latest data
	currentVisionData = visionModule->getCurrentFrame();
	command = strategy->getCurrentFrame()->getDestination(robotID);
	///ALWAYS call the previous 2 functions to get the latest data
	//----------------------------
  	
  ///If not active, dont do anything!
  if(!initialized) 
  {
		return;  
  }

  //get pick angle
  oppLoc = getTheirRobotLocation(opponent,*currentVisionData,*sp);    
  if(oppLoc.getY() > sp->field.SPLIT_LINE) pickAngle = -2*PI/3;
  else pickAngle = 2*PI/3;

  //Get creator
  creator = strategy->getCurrentRoboCupFrame()->getRobotByPosition(CREATOR);
  if(creator == NO_ROBOT) 
  {
    strategy->getCurrentFrame()->setMessage(robotID, "SpecOpBlock: No Creator found!");
    return;
  }

  creatorLoc = getLocation(creator,*currentVisionData,*sp);
  
  //Find a new opponent to pick if: 
  //1) the old one doesn't exist or the old one is too far away from the creator
  //2) a new opponent exists within the constraints of getOpponent()
  //Otherwise we continue to use the old one
  //if( (opponent == NO_ROBOT || dist(creatorLoc, oppLoc) > MAX_OPP_DIST) && getOpponent() != NO_ROBOT ) {
  //  opponent = getOpponent();
  //}
  if(opponent == NO_ROBOT) 
  {
    strategy->getCurrentFrame()->setMessage(robotID, "SpecOpBlock: No opponent found!");
    return;
  }

  oppLoc = getTheirRobotLocation(opponent,*currentVisionData,*sp);
  
  //get pick angle and pick location
  if(oppLoc.getY() > sp->field.SPLIT_LINE) 
  {
    pickAngle = -2*PI/3;
  }
  else 
  {
    pickAngle = 2*PI/3;
  }

  pickLoc.setX( oppLoc.getX() + (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS + PICK_DIST) * COS(pickAngle) );
  pickLoc.setY( oppLoc.getY() + (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS + PICK_DIST) * SIN(pickAngle) );

  //go to pick loc if we are too far away or opponent is too far away from us
  myLoc = getLocation(robotID, *currentVisionData,*sp);
  if(
      dist(myLoc, pickLoc) > sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS + MOVE_THRESH || 
      dist(myLoc, oppLoc) > sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS + MOVE_THRESH
    ) 
  {
    command->setPos(pickLoc);
    strategy->getCurrentFrame()->setMessage(robotID, "SpecOpBlock: Moving to pick");
  }
  else 
  {
    command->setPos(myLoc);
    strategy->getCurrentFrame()->setMessage(robotID, "SpecOpBlock: Picking!");
  }

  strategy->getCurrentRoboCupFrame()->setPassDest(robotID, command->getPos());


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
    command->setKick(KICK_SHOT);
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=
}
//=====================================================
bool SpecialOpBlockSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void SpecialOpBlockSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool SpecialOpBlockSkill::isFinished(float tolerance) 
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void SpecialOpBlockSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/SpecialOpBlockSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(PICK_DIST);
  READ_FLOAT(MOVE_THRESH);
  READ_FLOAT(MAX_OPP_DIST);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//===============================================================================
//Find opponent on same side as aggressor who is closest to aggressor 
//(in x-direction) but in front of him
RobotIndex SpecialOpBlockSkill::getOpponent() {
  currentVisionData = visionModule->getCurrentFrame();

  RobotIndex opp = NO_ROBOT;
  Pair oppLoc;

  float distance = -1.0f;
  opp = NO_ROBOT;
  creator = strategy->getCurrentRoboCupFrame()->getRobotByPosition(CREATOR);
  aggressor = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);

  if(creator == NO_ROBOT || aggressor == NO_ROBOT)
    return opp;
  
  creatorLoc = getLocation(creator,*currentVisionData,*sp);
  aggressorLoc = getLocation(aggressor,*currentVisionData,*sp);

  //always check if there is a better opponent
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) 
  {   
	  if( robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) ) 
	  {
		  oppLoc = getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
		  if( 
			  //opponent in front of creator or opponent closer than creator to split line
			  (oppLoc.getX() < creatorLoc.getX()  || 
			   ABS(oppLoc.getY()-sp->field.SPLIT_LINE) < ABS(creatorLoc.getY()-sp->field.SPLIT_LINE) ) &&
         //opponent closer to creator than aggressor //people behind
         dist(aggressorLoc, oppLoc) > dist(oppLoc, creatorLoc) &&
         //opponent not in goalie box
         !inTheirGoalieBox(oppLoc, *sp) &&
			  //this opponent is closer than last opponent chosen
			  (distance == -1.0f || dist(oppLoc, creatorLoc) < distance)
			  )
		  {         
			  //Update closest opponent to aggressor if he is closer
			  opp = i;
			  distance = dist(oppLoc, creatorLoc);
		  }
	  }
  }
  return opp;
}