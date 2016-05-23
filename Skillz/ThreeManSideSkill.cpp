#include "ThreeManSideSkill.h"
#include "CutGoalSkill.h"
#include "common/util/defense/defending.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/info/ball/ball.h"
#include "common/util/math/angle.h"


int ThreeManSideSkill::skillNum = -1;



///Constructor.  Calls the base class Skill constructor
ThreeManSideSkill::ThreeManSideSkill(SystemParameters *sp,
                                     VisionModule *vm, 
                                     RoboCupStrategyModule *strat,
                                     RobotIndex id, 
                                     SkillSet* set) : Skill(sp,vm,strat,id, set)
{
  loadValues();
  initialized = false;
}

/// Check to see if we have the ball or not.
bool ThreeManSideSkill::isValid() 
{ 
  return true;
}

void ThreeManSideSkill::initialize() 
{
  timer->resetTimer();
  initialized=true;
  presetBall=false;
  chase=false;
}
///resets timer
void ThreeManSideSkill::initialize(Pair ballPos) 
{
  initialize();
  presetBall=true;
  ball=ballPos;
}


//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void ThreeManSideSkill::execute()
{    
  ///If not active, dont do anything!
  if(!initialized) return; 

    //find goalie's shadow
  // grab the ball location
  if(!presetBall){
    ball = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();
  }

   //find blocker's shadow
  float y1, y2;
  RobotIndex BlockerID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER);

  if(BlockerID == NO_ROBOT)
  {
    //blocker does not exist.  we're in trouble
    y1 = sp->field.LEFT_GOAL_POST;
    y2 = sp->field.LEFT_GOAL_POST;
  }
  else
  {
 	  Pair blocker = getLocation(BlockerID,*currentVisionData,*sp);
    findShadow(ball,blocker, *sp, y1, y2);
  }

  int side=1;
  //Figure out which side we should be on.
  if(ball.getY() < sp->field.HALF_LINE) side=-side;
  if(strategy->getCurrentRoboCupFrame()->getPositionOfRobot(robotID) == DEFENDER){
    side=-side;
    chase=true;
  }

  if(side == -1)
  {
    //y2 = y1;
    y1 = sp->field.RIGHT_GOAL_POST - sp->general.BALL_RADIUS;
    y2 = sp->field.SPLIT_LINE - sp->general.BALL_RADIUS;
//    if(y2<y1)y2=y1;
  }
  else
  {
//    y1=y2;
//    if(y2<y1)y1=y2;
    y1 = sp->field.SPLIT_LINE + sp->general.BALL_RADIUS;
    y2 = sp->field.LEFT_GOAL_POST + sp->general.BALL_RADIUS;
  }

  if(presetBall){ 
    ((CutGoalSkill *)skillSet->getSkill(CutGoalSkill::skillNum))->initialize(y1,y2,UPPER_X,LOWER_X,ball);
  }else{
    ((CutGoalSkill *)skillSet->getSkill(CutGoalSkill::skillNum))->initialize(y1,y2,UPPER_X,LOWER_X);
  }
  skillSet->getSkill(CutGoalSkill::skillNum)->run();

  Pair robotLoc = getLocation(robotID, *currentVisionData, *sp);

  //make him look at the ball, but never rotate too far back
  float destAngle = angleBetween(robotLoc, getBallLocation(*currentVisionData));
  float maxAngle = 3.0f*PI/8.0f;

  if(destAngle > maxAngle)
    destAngle = maxAngle;
  if(destAngle < -maxAngle)
    destAngle = -maxAngle;
  
  command->setRotation(destAngle);

  /*
  //if robot moving past other side, make him run to the close side of the other robot, forcing 
  //position switching with obstacle avoidance getting in the way
  int friendID;
  if(strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER) == robotID)
    friendID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_DEFENDER);
  else
    friendID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER);

  if(friendID != NO_ROBOT)
  {
    Pair friendLoc = getLocation((RobotIndex)friendID, *currentVisionData, *sp);
    Pair robotLoc = getLocation(robotID, *currentVisionData, *sp);
    //other guy closer, go to his side
    if(friendLoc.squareDistanceTo(command->getPos()) < robotLoc.squareDistanceTo(command->getPos()))
    {
      command->setXPos(friendLoc.getX());
      if(friendLoc.getY() < robotLoc.getY())
      {
        command->setYPos(friendLoc.getY() + 2.0f*sp->general.PLAYER_RADIUS);
      }
      else
      {
        command->setYPos(friendLoc.getY() - 2.0f*sp->general.PLAYER_RADIUS);
      }
    }
  }
  */

  //see if we should run out and kick the ball
  if(
      ball.distanceTo(command->getPos()) <= sp->general.PLAYER_RADIUS + KICK_DIST &&
      kickSafe(robotID, *currentVisionData, *sp, *strategy->getCurrentRoboCupFrame())
    )
  {
    strategy->getCurrentFrame()->setMessage(robotID,"3Man: Kicking ball away");
    command->setPos(robotPositionGivenFront(ball,0.0f,*sp));
  }

  if(
      ABS(getRotation(robotID,*currentVisionData,*sp)) <= KICK_ANGLE &&
      kickSafe(robotID, *currentVisionData, *sp, *strategy->getCurrentRoboCupFrame())
    )
  {
    command->setKick(KICK_SHOT);
  }

    //debugging only:
	  findShadow(ball,getLocation(robotID,*currentVisionData,*sp),*sp,y1,y2);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,1,sp->field.OUR_GOAL_LINE,y1);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,2,sp->field.OUR_GOAL_LINE,y2);

}

///always true for now.
bool ThreeManSideSkill::evaluate() {
  return true;
}

//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void ThreeManSideSkill::update() {
  
}

///check if we've reached our target destination and rotation
bool ThreeManSideSkill::isFinished(float tolerance)
{
  return false;
}




///Reads in the values of the parameters from the skill's parameter text file.
void ThreeManSideSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/ThreeManSideSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
	READ_FLOAT(LINE_X_OFFSET);
	UPPER_X = sp->field.DEFENSE_ZONE_LINE + LINE_X_OFFSET;
	LOWER_X = sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS + LINE_X_OFFSET;
  READ_FLOAT(KICK_ANGLE);
  READ_FLOAT(KICK_DIST);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}