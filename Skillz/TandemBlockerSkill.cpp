#include "TandemBlockerSkill.h"
#include "TightDefenseBlockerSkill.h"
#include "DiveSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/pair/pair.h"
#include "common/util/shoot/shoot.h"
#include "common/util/math/angle.h"
#include "common/util/defense/defending.h"
#include "common/util/info/ball/ball.h"
#include "GUI/GUI.h"

int TandemBlockerSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
TandemBlockerSkill::TandemBlockerSkill(SystemParameters *sp,
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
bool TandemBlockerSkill::isValid() 
{ 
  return true;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void TandemBlockerSkill::initialize()
{
	//reset timer
	presetBall=false;
	timer->resetTimer();
	
	LOWER_X=UPPER_X = sp->field.DEATH_ZONE_LINE -2*sp->general.PLAYER_RADIUS;
	cutGoal=(CutGoalSkill *)skillSet->getSkill(CutGoalSkill::skillNum);
	initialized = true;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void TandemBlockerSkill::initialize(Pair ballPos)
{
	initialize();
  presetBall=true;
  ball=ballPos;
}
//=====================================================
void constrain(float &y, SystemParameters *sp){
  if(y>sp->field.LEFT_GOAL_POST){
    y=sp->field.LEFT_GOAL_POST;
  }else if(y<sp->field.RIGHT_GOAL_POST){
    y=sp->field.RIGHT_GOAL_POST;
  }
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void TandemBlockerSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
	}
  Pair currentPos=getLocation(robotID,*currentVisionData,*sp);

  //see if we should dive
  if(
      !presetBall && 
      towardGoal(getRobocupBall(*currentVisionData),
                 sp->field.OUR_GOAL_LINE,
                 sp->field.LEFT_GOAL_POST + GOAL_POST_TOLERANCE,
                 sp->field.RIGHT_GOAL_POST - GOAL_POST_TOLERANCE,
                 DIVE_SPEED))
  {
    DiveSkill * dive= (DiveSkill *)skillSet->getSkill(DiveSkill::skillNum);
    if(!dive->isInitialized()){
      dive->initialize(getLocation(robotID,*currentVisionData,*sp).getX());
    }
    dive->run();
    if(nearOurGoalieBox(currentPos,*sp,sp->general.PLAYER_RADIUS))command->setControl(OMNI_GOALIE);
    else command->setControl(OMNI_NORMAL_ENTERBOX);
    command->setSpeed(GOALIE_SPEED);

    /*
    //constrain to betwen goal posts
    if(command->getPos().getY() > sp->field.LEFT_GOAL_POST)
    {
      command->setPos(command->getPos().getX(),
                      sp->field.LEFT_GOAL_POST);
    }
    else if(command->getPos().getY() < sp->field.RIGHT_GOAL_POST)
    {
      command->setPos(command->getPos().getX(),
                      sp->field.RIGHT_GOAL_POST);
    }
*/

    return;
  }
  //always uninit dive when not used
  else
  {
    DiveSkill * dive= (DiveSkill *)skillSet->getSkill(DiveSkill::skillNum);
    dive->unInitialize();
  }

  // grab the ball location
  if(!presetBall){
    ball = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();
  }

  if(
      (
        ball.getX() < sp->field.DEATH_ZONE_LINE+LOW_X ||
        (
          ball.getX() < sp->field.DEFENSE_ZONE_LINE &&
          (
            ball.getY() > sp->field.LEFT_SWEET_SECTOR ||
            ball.getY() < sp->field.RIGHT_SWEET_SECTOR
          )
        )
      ) &&
      !badRobot(DEFENDER, 0.3f, *currentVisionData, *strategy->getRelativeRoboCupFrame(-1), *sp)
    )
  {
    //ball's too close!  call tight!
    skillSet->getSkill(TightDefenseBlockerSkill::skillNum)->initialize();
    skillSet->getSkill(TightDefenseBlockerSkill::skillNum)->run();
    return;
  }

   //find defender's shadow
  float y1, y2;
  float dy1,dy2;
  defenderID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER);
  RobotIndex specialOpID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_DEFENDER);

  if(defenderID == NO_ROBOT)
  {
  	LOWER_X = sp->field.OUR_GOAL_LINE + 1*sp->general.PLAYER_RADIUS;

    //defender does not exist, covers none of goal
    dy1 = -32000.0f;
    dy2 = -32000.0f;
  }
  else
  {
 	  Pair defender = getLocation(defenderID,*currentVisionData,*sp);
    findShadow(ball,defender, *sp, dy1, dy2);
  }
  
  float sy1,sy2;
  if(specialOpID == NO_ROBOT)
  {
    sy1 = -32000.0f;
    sy2 = -32000.0f;
  }
  else
  {
 	  Pair specop = getLocation(specialOpID,*currentVisionData,*sp);
    findShadow(ball,specop, *sp, sy1, sy2);
  }

  constrain(dy1,sp);
  constrain(dy2,sp);
  constrain(sy1,sp);
  constrain(sy2,sp);

  //find outer edges.
  y1=MIN(sy1,dy1);
  y2=MAX(sy2,dy2);

	Pair blocker;
	blocker = getLocation(robotID,*currentVisionData,*sp);
  
  //Check which post is more uncovered and cut the goal on that end
  if(ball.getX() < LOWER_X+LOW_X)
  {    
    command->setXPos(sp->field.OUR_GOAL_LINE + sp->general.PLAYER_RADIUS);
    command->setRotation(angleBetween(blocker, ball));
    if(ball.getY()> sp->field.SPLIT_LINE)
    {
      command->setYPos(sp->field.LEFT_GOAL_POST + sp->general.PLAYER_RADIUS);
    }
    else
    {
      command->setYPos(sp->field.RIGHT_GOAL_POST - sp->general.PLAYER_RADIUS);
    }
    strategy->getCurrentFrame()->setMessage(robotID,"TandemBlocker no good.  don't call it");
  }  
  else 
  {
    float guard1, guard2;

    if( y2 <= sp->field.RIGHT_GOAL_POST || y1 >= sp->field.LEFT_GOAL_POST)
    { 
        guard1=sp->field.LEFT_GOAL_POST;
        guard2=sp->field.RIGHT_GOAL_POST;
    }
    else
    {	
		    if(((sp->field.LEFT_GOAL_POST)-y2) > (y1 - (sp->field.RIGHT_GOAL_POST)))
        {
          guard1=sp->field.LEFT_GOAL_POST;
          guard2=y2;
        }
        else
        {
          guard1=y1;
          guard2=sp->field.RIGHT_GOAL_POST;
        }
    }

    //if shadows are overlapping, one large shadow is fine.
    //otherwise, see if the middle is what we want.
    if(dy2 > sy2 && dy1 > sy2){
      if(ABS(guard2-guard1) < (dy1-sy2)){
        guard1=sy2;
        guard2=dy1;
      }
    }else if(sy2 > dy2 && sy1 > dy2){
      if(ABS(guard2-guard1) < (sy1-dy2)){
        guard1=dy2;
        guard2=sy1;
      }
    }
		
    if(presetBall) cutGoal->initialize(guard1, guard2,UPPER_X,LOWER_X,ball);
    else cutGoal->initialize(guard1, guard2,UPPER_X,LOWER_X);
		cutGoal->run();
    if(inOurGoalieBox(currentPos,*sp))command->setControl(OMNI_GOALIE);
    else command->setControl(OMNI_NORMAL_ENTERBOX);
    command->setSpeed(GOALIE_SPEED);

    //Check for opponent in the way.
    for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
    {
        if(isBetween(blocker, 
                     command->getPos(),
                     sp->general.OTHER_TEAM,
                     i,
                     *currentVisionData,
                     *sp,
                     -32000.0f,
                     sp->general.PLAYER_RADIUS))
        {
          GUI_Record.debuggingInfo.setDebugPoint(robotID,0,command->getPos());
          float goalieBoxLine=sp->field.OUR_GOALIE_BOX - sp->general.PLAYER_RADIUS;
          if(presetBall) cutGoal->initialize(guard1, guard2,goalieBoxLine,goalieBoxLine);
          else cutGoal->initialize(guard1, guard2,goalieBoxLine,goalieBoxLine,ball);
		      cutGoal->run();
          if(inOurGoalieBox(currentPos,*sp))command->setControl(OMNI_GOALIE);
          else command->setControl(OMNI_NORMAL_ENTERBOX);
          command->setSpeed(GOALIE_SPEED);

          strategy->getCurrentFrame()->setMessage(robotID,"Blocked! dropping back");
          break;
       }
    }

    //debugging only:
	  findShadow(ball,getLocation(robotID,*currentVisionData,*sp),*sp,y1,y2);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,1,sp->field.OUR_GOAL_LINE,y1);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,2,sp->field.OUR_GOAL_LINE,y2);
  }
 
  if(inOurGoalieBox(currentPos,*sp))command->setControl(OMNI_GOALIE);
  else command->setControl(OMNI_NORMAL_ENTERBOX);
  command->setSpeed(GOALIE_SPEED);

}
//=====================================================
///If the timer goes past 3 secs, reverse rotation
bool TandemBlockerSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void TandemBlockerSkill::update() 
{
}
//=====================================================
//Screen is never finished, it can always continue
bool TandemBlockerSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
//Reads in the values of the parameters from the skill's parameter text file.
void TandemBlockerSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/TandemBlockerSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
  READ_FLOAT(LOW_X);
  READ_FLOAT(DIVE_OFFSET);
  //################	ADD PARAMETERS ABOVE ################
	mainFile.close();
  
  //***Shared params for Diving***
  mainFile.open("Params/Skill_Params/TandemDiveParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //################	ADD PARAMETERS BELOW ################
  READ_FLOAT(DIVE_SPEED);
  READ_FLOAT(GOAL_POST_TOLERANCE);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
