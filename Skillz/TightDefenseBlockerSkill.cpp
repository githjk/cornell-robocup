#include "TightDefenseBlockerSkill.h"
#include "TandemBlockerSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/info/ball/ball.h"
#include "common/util/defense/defending.h"

int TightDefenseBlockerSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
TightDefenseBlockerSkill::TightDefenseBlockerSkill(SystemParameters *sp,
                                                   VisionModule *vm, 
                                                   RoboCupStrategyModule *strat,
                                                   RobotIndex id, 
                                                   SkillSet* set) :
                                                   Skill(sp,vm,strat,id,set) 
{
  initialized = false;
}

//=====================================================
///Call this function to check the prerequisites of the skill.	This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool TightDefenseBlockerSkill::isValid() 
{ 
  return true;
}

//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void TightDefenseBlockerSkill::initialize()
{
  gainPossessionSkill = 
    (BlockerGainPossessionSkill*)skillSet->getSkill(BlockerGainPossessionSkill::skillNum);

	gainPossessionSkill->initialize();
  //moveSkill->initialize(false);
	initialized = true;
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void TightDefenseBlockerSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized) return; 

  gainPossessionSkill = 
    (BlockerGainPossessionSkill*)skillSet->getSkill(BlockerGainPossessionSkill::skillNum);

  command->setSpeed(GOALIE_SPEED);

  Pair currentPos=getLocation(robotID,*currentVisionData,*sp);
  float currentRot=getRotation(robotID,*currentVisionData,*sp);

  if(nearOurGoalieBox(currentPos,*sp,sp->general.PLAYER_RADIUS))command->setControl(OMNI_GOALIE);
  else command->setControl(OMNI_NORMAL_ENTERBOX);

  ballLoc.set(strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation());
  getRobotWithBall();

  //See if defender is doing the right thing
  if(badRobot(DEFENDER,DEFENDER_DIST,*currentVisionData,*strategy->getRelativeRoboCupFrame(-1),*sp) && ballLoc.getX() > sp->field.OUR_GOALIE_BOX){
    if(!skillSet->getSkill(TandemBlockerSkill::skillNum)->isInitialized()){
      skillSet->getSkill(TandemBlockerSkill::skillNum)->initialize();
    }
    skillSet->getSkill(TandemBlockerSkill::skillNum)->run();
  }else{
    strategy->getCurrentFrame()->setMessage(robotID, "Hugging Post");
    command->setXPos(sp->field.OUR_GOAL_LINE + sp->general.PLAYER_RADIUS + GOAL_LINE_OFFSET);
    if(ballLoc.getY() > sp->field.SPLIT_LINE){
      command->setYPos(sp->field.LEFT_GOAL_POST);
      command->setRotation(FACE_ANGLE);
    }else{ 
      command->setYPos(sp->field.RIGHT_GOAL_POST);
      command->setRotation(-FACE_ANGLE);
    }
  }
  
  //see if we should go kick the ball
  if(ballLoc.getX() < sp->field.OUR_GOALIE_BOX + sp->general.BALL_RADIUS &&
     ballLoc.getY() < sp->field.LEFT_GOALIE_BOX &&
     ballLoc.getY() > sp->field.RIGHT_GOALIE_BOX  &&
     getBallSpeed(*currentVisionData) < MAX_BALL_SPEED)
  {
    //check for dangerous case:
    if(
        ABS(ballLoc.getY() - sp->field.SPLIT_LINE) < ABS(currentPos.getY() - sp->field.SPLIT_LINE) &&
        ballLoc.getX() < sp->field.OUR_GOAL_LINE + sp->general.PLAYER_RADIUS
      )
    {
      //don't do anything!
      strategy->getCurrentFrame()->setMessage(robotID,"Tight: Scared stiff");
      command->setPos(currentPos);
      command->setRotation(currentRot);
    }
    else if(
              (
                (
                 ballLoc.getY() > sp->field.LEFT_GOAL_POST && 
                 ballLoc.getY() < sp->field.LEFT_GOAL_POST + sp->general.PLAYER_RADIUS
                ) ||
                (
                 ballLoc.getY() < sp->field.RIGHT_GOAL_POST && 
                 ballLoc.getY() > sp->field.RIGHT_GOAL_POST - sp->general.PLAYER_RADIUS
                ) 
              ) &&
              ballLoc.getX() < sp->field.OUR_GOAL_LINE + sp->general.PLAYER_RADIUS
            )
    {
      //butt the ball out.
      command->setRotation(0.0f);
      if(ballLoc.getY() > sp->field.SPLIT_LINE){
        command->setYPos(sp->field.LEFT_GOAL_POST);
      }else{
        command->setYPos(sp->field.RIGHT_GOAL_POST);
      }
      command->setXPos(sp->field.OUR_GOAL_LINE + sp->general.DIST_TO_DRIBBLER);
      command->setKick(KICK_SLOW);
      strategy->getCurrentFrame()->setMessage(robotID,"Tight: Butting ball away");
    }
    else
    {
      //run forward to hit ball away
      if(ballLoc.getY() > sp->field.SPLIT_LINE)
      {
        command->setYPos(sp->field.LEFT_GOALIE_BOX - sp->general.DIST_TO_DRIBBLER);
      }
      else
      {
        command->setYPos(sp->field.RIGHT_GOALIE_BOX + sp->general.DIST_TO_DRIBBLER);
      }

      if(ballLoc.getX() > sp->field.OUR_GOALIE_BOX - sp->general.PLAYER_RADIUS)
      {
        command->setXPos(ballLoc.getX() - sp->general.PLAYER_RADIUS + 2.0f*sp->general.BALL_RADIUS);
      }
      else if(ballLoc.getX() < sp->field.OUR_GOAL_LINE + sp->general.PLAYER_RADIUS)
      {
        command->setXPos(sp->field.OUR_GOAL_LINE + sp->general.DIST_TO_DRIBBLER);
      }
      else
      {
        command->setXPos(ballLoc.getX());
      }
      //ignore walls if we're past the post.
      if(currentPos.getY() > sp->field.LEFT_GOAL_POST ||
        currentPos.getY() < sp->field.RIGHT_GOAL_POST){
        command->setControl(OMNI_NO_AVOID);
      }
      strategy->getCurrentFrame()->setMessage(robotID,"Tight: Kicking ball away");
    }
  }

  //arm the kicker:
  if(currentPos.distanceTo(command->getPos()) < KICK_DIST &&
        ABS(angleDifference(currentRot,command->getRotation())) < KICK_ANGLE){
    command->setKick(KICK_SLOW);
  }

}


//=====================================================
//Finds if the ball is in the goal box
bool TightDefenseBlockerSkill::ballInGoalBox() {
  currentVisionData = visionModule->getCurrentFrame();
		
	ballLoc.set(strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation());
  return ballLoc.getX() <= sp->field.OUR_GOALIE_BOX 
    && ballLoc.getY() < sp->field.LEFT_GOALIE_BOX
    && ballLoc.getY() > sp->field.RIGHT_GOALIE_BOX;
}


//=====================================================
//Finds which robot has the ball, and overwrites robotWithBall
//and teamWithBall variables. Sets robotWithBall = NO_ROBOT if 
//ball is free
void TightDefenseBlockerSkill::getRobotWithBall() {
  //Look at each robot to see if it exists and has possession
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) 
  {
    if(robotFound(i,*currentVisionData,*sp) &&
       friendlyHasPossession(i,
                     *sp, 
                     *currentVisionData,
                     *(RobocupStrategyData*)strategy->getCurrentFrame(), 
                     false)) 
    {
      robotWithBall = i;
      teamWithBall = sp->general.TEAM;
      return;
    }
    if(robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) 
      && opponentHasPossession(i, *sp)) {
      robotWithBall = i;
      teamWithBall = sp->general.OTHER_TEAM;
      return;
    }
  }
  //No one has possession
  robotWithBall = NO_ROBOT;
  teamWithBall = sp->general.OTHER_TEAM;
}

//=====================================================
bool TightDefenseBlockerSkill::evaluate() 
{
	return true;
}


//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void TightDefenseBlockerSkill::update() 
{
  //I am primitive, I do not learn. :(
}

//=====================================================
bool TightDefenseBlockerSkill::isFinished(float tolerance)
{
	return false;
}

//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void TightDefenseBlockerSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/TightDefenseBlockerSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	

  READ_FLOAT(SCREEN_DIST);
  READ_FLOAT(GOAL_LINE_OFFSET);
  READ_FLOAT(FACE_ANGLE);
  READ_FLOAT(KICK_DIST);
  READ_FLOAT(KICK_ANGLE);
  READ_FLOAT(CLOSE_DIST);
  READ_FLOAT(DEFENDER_DIST);
  READ_FLOAT(MAX_BALL_SPEED);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
