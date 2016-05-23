#include "OppositeCoverSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/info/ball/ball.h"
#include "common/util/info/robot/robot.h"

int OppositeCoverSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
OppositeCoverSkill::OppositeCoverSkill(SystemParameters *sp,
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
bool OppositeCoverSkill::isValid() 
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
void OppositeCoverSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
}


//=====================================================
///The robot will drive forward with its kicker on.
void OppositeCoverSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

  Pair ballLoc = getBallLocation(*currentVisionData);
  Pair ourLoc(getLocation(robotID, *currentVisionData, *sp));


  RobotIndex DefenderID = 
    strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER);

  Pair dest;
  
  if(DefenderID != NO_ROBOT &&
     robotFound(DefenderID, *currentVisionData, *sp))
  {
    Pair defenderLoc = getLocation(DefenderID, *currentVisionData, *sp);
    if(ballLoc.getY() > sp->field.SPLIT_LINE)
    {
      dest.set(
                defenderLoc.getX() + X_OFFSET,
                (
                  sp->field.RIGHT_GOAL_POST +
                  sp->general.PLAYER_RADIUS + 
                  Y_OFFSET
                )
              );
    }
    else
    {
      dest.set(
                defenderLoc.getX() + X_OFFSET,
                (
                  sp->field.LEFT_GOAL_POST -
                  sp->general.PLAYER_RADIUS - 
                  Y_OFFSET
                )
              );
    }  
  }
  else
  {
    dest.set(
              sp->field.OUR_GOALIE_BOX + 4*sp->general.PLAYER_RADIUS,
              sp->field.SPLIT_LINE
            );
  }

  Pair originalDestination(dest);

  //find nearest opponent to this spot, if 
  //too close, push forward to avoid circling
  float closestDist = -1.0f;
  float tempDist;
  Pair closestOp;
  Pair tempOp;
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    tempOp.set(getLocation(sp->general.OTHER_TEAM, i, *currentVisionData));
    tempDist = tempOp.distanceTo(dest);
    if(
        closestDist < 0.0f ||
        tempDist < closestDist
      )
    {
      closestDist = tempDist;
      closestOp.set(tempOp);
    }
  }

  //if too close, push off
  if(closestDist < (sp->general.PLAYER_RADIUS + 
                    sp->general.OPPONENT_RADIUS))
  {
    //----------------------------------------------
    //push forwards
    if(closestOp.getX() < dest.getX())
    {
      //keep yDiff same, push x forward
      float yDiffSquared = dest.squareDistanceTo(closestOp);
      float seperationDistSquared = (
                                      (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS) *
                                      (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS)
                                    );
      float xDiffSquared = seperationDistSquared - yDiffSquared;
      
      //sanity check, impossible not to be true
      if(xDiffSquared > 0.0f)
      {
        dest.setX(dest.getX() + SQRT(xDiffSquared));
      }
    }
    //----------------------------------------------
    //push sideways toward the ball
    else
    {   
      float clearance = sp->general.PLAYER_RADIUS +
                        sp->general.OPPONENT_RADIUS;

      dest.set(closestOp.getX() + clearance,
               closestOp.getY());
    }
    //----------------------------------------------
  }

  if(dest.distanceTo(originalDestination) > ourLoc.distanceTo(originalDestination))
  {
    dest.set(ourLoc);
  }

  command->setPos(dest);
  command->setRotation(angleBetween(command->getPos(),
                                    ballLoc));

  strategy->getCurrentRoboCupFrame()->setMessage(robotID, "Opposite Cover");
}
//=====================================================
bool OppositeCoverSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void OppositeCoverSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool OppositeCoverSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void OppositeCoverSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/OppositeCoverSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(X_OFFSET);
  READ_FLOAT(Y_OFFSET);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
