#include "AggressivePullBallOffCornerSkill.h"
#include "common/util/info/ball/ball.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"
#include "common/util/pair/pair.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"

int AggressivePullBallOffCornerSkill::skillNum = -1;
//===============================================================================
///Constructor.	Calls the base class's (Skill) constructor.
AggressivePullBallOffCornerSkill::AggressivePullBallOffCornerSkill(SystemParameters *sp,
                                               VisionModule *vm, 
                                               RoboCupStrategyModule *strat,
                                               RobotIndex id, 
                                               SkillSet* set) : Skill(sp,vm,strat,id,set) 
{
	///Read in any parameters stored in the parameter file.
	loadValues();
}
//===============================================================================
///The prereqs of this skill are that I have not successfully pulled the ball off of the Corner
///the ball the ball is less than Corner_THRESH from the side Corner,
///more than OUR_CORNER_THRESH from our goal line Corner
///more than THEIR_CORNER_THRESH from their goal line Corner, moving slower than
///BALL_SPEED_THRESH, opponents do not have the ball, teammates do not have the ball.
bool AggressivePullBallOffCornerSkill::isValid() 
{
  ///False if the ball is not in the valid zone along the side Corners, not in corners
	if( !ballOnCorner(*currentVisionData))
		return false;

	///False if my teammate (not me) has the ball
	for(RobotIndex i = ROBOT0; i<NUM_ROBOTS; i++)
	{
		if( i != robotID && friendlyHasPossession(i, *sp) &&
        !friendlyHasPossession(robotID, *sp))
    {
      return false;
    }
	}

  return true;
}
//===============================================================================
///Perform any initializations for the skill.
void AggressivePullBallOffCornerSkill::initialize() 
{
  ///Reset timer which tracks the duration of activity for this skill
  timer->resetTimer();
  initialized = true;
  spinning = false;
}
//===============================================================================
///Execute the skill - get possession, back and turn away from Corner.
void AggressivePullBallOffCornerSkill::execute()
{   
  ///If not active, dont do anything!
  if(!initialized) return;  

  ballLoc.set(getBallLocation(*currentVisionData));
  robotLoc = getLocation(robotID,*currentVisionData,*sp);
  robotAngle = getRotation(robotID, *currentVisionData, *sp);

  //determine two destinations: 
  //a.) run to point
  //b.) insert and spin point 
  if(ballLoc.getX()>sp->field.HALF_LINE){
    end=1;
    endWall=sp->field.THEIR_GOAL_LINE;
  }else{
    end=-1;
    endWall=sp->field.OUR_GOAL_LINE;
  }
  if(ballLoc.getY()>sp->field.SPLIT_LINE){
    side=1;
    sideWall=sp->field.LEFT_SIDE_LINE;
  }else{
    side=-1;
    sideWall=sp->field.RIGHT_SIDE_LINE;
  }

  nearLoc.set(endWall - end*NEAR_DIST*sp->general.PLAYER_RADIUS,
              sideWall - side*NEAR_DIST*sp->general.PLAYER_RADIUS);

  farLoc.set(endWall - end*FAR_DIST*sp->general.PLAYER_RADIUS,
              sideWall - side*FAR_DIST*sp->general.PLAYER_RADIUS);

  
  //using hysteresis determine if we should start or stop spinning
  if(
      robotLoc.distanceTo(farLoc) < FAR_TO_NEAR_LOC_TOL && 
      ABS(angleDifference(robotAngle, angleBetween(robotLoc, nearLoc))) < FAR_TO_NEAR_ROT_TOL &&
      !spinning
    ) 
  {
    spinning = true;
  }

  if(robotLoc.distanceTo(ballLoc) > NEAR_TO_FAR_TOL && spinning) spinning = false;

  //pick mode and run
  if(spinning)
  {
    strategy->getCurrentFrame()->setMessage(robotID, "PullBallOffCorner: Rotating");
    //command->setSpeed(CAREFUL_SPEED);
    command->setPos(nearLoc);
    command->setRotation(normalizeAngle(robotAngle - side*ROTATION_SPEED));
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
    command->setControl(OMNI_NO_AVOID);
  }
  else
  {
    strategy->getCurrentFrame()->setMessage(robotID, "PullBallOffCorner: Approaching Corner");
    command->setPos(farLoc);
    command->setRotation(angleBetween(robotLoc, nearLoc));
  }
}
//===============================================================================
bool AggressivePullBallOffCornerSkill::evaluate() 
{
  return true;
}
//===============================================================================
void AggressivePullBallOffCornerSkill::update() { }
//===============================================================================
//returns true if ball in any corner
bool AggressivePullBallOffCornerSkill::ballOnCorner(const VisionData& field)
{
  int Walls = 0;
  ballLoc.set(getBallLocation(*currentVisionData));

  if( ballLoc.getX() > sp->field.THEIR_GOAL_LINE - CORNER_THRESH ) Walls++;
  if( ballLoc.getX() < sp->field.OUR_GOAL_LINE + CORNER_THRESH ) Walls++;
  if( ballLoc.getY() > sp->field.LEFT_SIDE_LINE - CORNER_THRESH ) Walls++;
  if( ballLoc.getY() < sp->field.RIGHT_SIDE_LINE + CORNER_THRESH ) Walls++;

  return Walls > 1;
}
//===============================================================================
///Not yet implemented.  never finished
bool AggressivePullBallOffCornerSkill::isFinished(float tolerance)
{
  return !isValid();
}
//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void AggressivePullBallOffCornerSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/PullOffCornerSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
  
  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //Read in a param
	READ_FLOAT(CORNER_THRESH);
  READ_FLOAT(NEAR_DIST);
  READ_FLOAT(FAR_DIST);
  READ_FLOAT(ROTATION_SPEED);
  READ_FLOAT(FAR_TO_NEAR_LOC_TOL);
  READ_FLOAT(FAR_TO_NEAR_ROT_TOL);
  READ_FLOAT(NEAR_TO_FAR_TOL);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================




