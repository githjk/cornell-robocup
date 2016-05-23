#include "CreatorCreateSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/pass/pass.h"
#include "common/util/math/dist.h"
#include "common/util/shoot/shoot.h"
#include "common/util/info/ball/possession.h"

int CreatorCreateSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
CreatorCreateSkill::CreatorCreateSkill(SystemParameters *sp,
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
bool CreatorCreateSkill::isValid() 
{ 
  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void CreatorCreateSkill::initialize()
{
  //reset timer
	timer->resetTimer();
	initialized = true;
  state = 0;

  startLoc = getLocation(robotID,*currentVisionData,*sp);
  calculatePassDest(startLoc,0);

  strategy->getCurrentRoboCupFrame()->setPassDest(robotID, passDest);

  if(dist(getLocation(robotID,*currentVisionData,*sp),passDest) < PASS_DISTANCE)
  {
    strategy->getCurrentRoboCupFrame()->setPassValue(robotID,1);
  }
  else
  {
    strategy->getCurrentRoboCupFrame()->setPassValue(robotID,0);
  }
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void CreatorCreateSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

  
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  float currentRot = angleToBall(robotID,*currentVisionData,*sp);
  
  //float left,right;

  /// Angle the creator should be facing
  float angle;

  // Is there a Pass
  bool isShot;

  // The position we will like the creator to be.
  Pair target;

  // The point of goal the creator will shoot at
  Pair goalPos;

  // Get ball Location
  Pair ballLoc(strategy->getCurrentRoboCupFrame()->getOffensiveBallLocation());

  // Get Special-Op creator location
	RobotIndex specCreatorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_CREATOR);
	Pair specCreatorLoc(getLocation(specCreatorID,*currentVisionData,*sp));

  // Get aggressor location
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);
	Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp));

  /// No point of skill without aggressor
  if(aggressorID == NO_ROBOT)
  {
     state = 2;
  }
  else
  {
    /// If no specialOp creator just go to split line and shoot
    if(specCreatorID == NO_ROBOT)
    {
      state = 1;
    }
  }

  /// Finding the split angle
  isShot = calcShot(robotID,sp->field.THEIR_GOAL_LINE,sp->strategy2002.SHOOT_LANE_THRESH,sp->field.RIGHT_GOAL_POST,sp->field.LEFT_GOAL_POST,
      NO_ROBOT,*currentVisionData,*sp,&goalPos);
    
  if(!isShot)
  {
        goalPos.setX(sp->field.THEIR_GOAL_LINE);
        goalPos.setY((sp->field.LEFT_GOAL_POST + sp->field.RIGHT_GOAL_POST)/2);
  }
  angle = findshootAngle(currentPos,goalPos,aggressorPos,ANGLE_BIAS);

  /// Finding the position for state = 0
  Pair destPos;
  if(currentPos.getY() > sp->field.SPLIT_LINE)
  {
     destPos.set(specCreatorLoc.getX() - (sp->general.PLAYER_RADIUS + 
                                        sp->general.OPPONENT_RADIUS + 
                                        1.5f*sp->general.OBSTACLE_CLEARANCE),
                                        specCreatorLoc.getY() + 
                                        (sp->general.PLAYER_RADIUS + 
                                        sp->general.OPPONENT_RADIUS + 
                                        1.5f*sp->general.OBSTACLE_CLEARANCE));
  }
  else
  {
     destPos.set(specCreatorLoc.getX() - (sp->general.PLAYER_RADIUS + 
                                        sp->general.OPPONENT_RADIUS + 
                                        1.5f*sp->general.OBSTACLE_CLEARANCE),
                                        specCreatorLoc.getY() - 
                                        (sp->general.PLAYER_RADIUS + 
                                        sp->general.OPPONENT_RADIUS + 
                                        1.5f*sp->general.OBSTACLE_CLEARANCE));
  }
               
  /// State = 0 (Try to move where the special Op aggressor is. Same Y, Different X)
  if(state <= 0)
  {
      //if(ABS(specCreatorLoc.getY()) <= ABS(currentPos.getY()))
      if(dist(destPos,currentPos) > GAP)
      {
         command->setPos(destPos);
         strategy->getCurrentFrame()->setMessage(robotID, "C: Going towards SpecialOP");
      }
      else
      {
         state = 1;
      }
      calculatePassDest(startLoc,angle);
  }
  
  /// State = 1 ( We reached behind the special Op Creator. Now time to be open for pass and take a shot.)

  if(state == 1)
  {
      strategy->getCurrentFrame()->setMessage(robotID, "C: Ready for Pass and shoot");
      command->setPos(robotDest);
  }
      
  /// Setting the pass destination
  strategy->getCurrentRoboCupFrame()->setPassDest(robotID, passDest);
  GUI_Record.debuggingInfo.setDebugPoint(robotID,1,passDest);

  if(dist(currentPos,passDest) < PASS_DISTANCE && state == 1)
  {
    strategy->getCurrentRoboCupFrame()->setPassValue(robotID,true);
  }
  else
  {
    strategy->getCurrentRoboCupFrame()->setPassValue(robotID,false);
  }
  
  command->setRotation(angle);
  if(ABS(command->getRotation()) < (PI/2)*KICK_RATIO)
  {command->setKick(KICK_SHOT);}

  if(state > 1)
  {  
     strategy->getCurrentFrame()->setMessage(robotID, "C: NO AGGRESSOR FOUND");
  }
}
//=====================================================
bool CreatorCreateSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void CreatorCreateSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool CreatorCreateSkill::isFinished(float tolerance)
{
  return strategy->getCurrentRoboCupFrame()->getPassValue(robotID);
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void CreatorCreateSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/CreatorCreateSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(ANGLE_BIAS);
  READ_FLOAT(GAP);
  READ_FLOAT(PASS_DISTANCE);
  READ_FLOAT(DISTANCE_FACTOR);
  READ_FLOAT(KICK_RATIO);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
/// This function finds the pass destination
void CreatorCreateSkill::calculatePassDest(Pair original,float rotAngle)
{

  /// Is there open place
  bool isOpen;

  /// The central point of their goal box
  Pair centralPoint(sp->field.THEIR_GOAL_LINE,(sp->field.RIGHT_GOAL_POST+sp->field.LEFT_GOAL_POST)/2);
 
  /// The Xline on which you want to receive pass
  float passLine = original.getX() - DISTANCE_FACTOR*sp->general.PLAYER_RADIUS ;

  if(original.getY() > sp->field.SPLIT_LINE)
  {
    isOpen = calcShot(centralPoint,passLine,sp->strategy2002.PASS_LANE_THRESH,sp->field.RIGHT_GOAL_POST,sp->field.LEFT_GOALIE_BOX,NO_ROBOT,*currentVisionData,*sp,&robotDest);
  }
  else
  {
    isOpen = calcShot(centralPoint,passLine,sp->strategy2002.PASS_LANE_THRESH,sp->field.LEFT_GOAL_POST,sp->field.RIGHT_GOALIE_BOX,NO_ROBOT,*currentVisionData,*sp,&robotDest);
  }

  if(!isOpen)
  {
    robotDest.setX(passLine);
    robotDest.setY(sp->field.SPLIT_LINE);
  }
  passDest.set(frontOfRobot(robotDest,rotAngle,*sp));
}

//=====================================================
/// This function returns the robot destination
Pair CreatorCreateSkill::getRobotDest()
{
  return robotDest;
}