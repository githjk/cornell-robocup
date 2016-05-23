#include "TandemDefenderSkill.h"
#include "TightDefenseDefenderSkill.h"
#include "DiveSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/pair/pair.h"
#include "common/util/defense/defending.h"
#include "common/util/info/ball/ball.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"


int TandemDefenderSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
TandemDefenderSkill::TandemDefenderSkill(SystemParameters *sp,
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
bool TandemDefenderSkill::isValid() 
{ 
  return true;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void TandemDefenderSkill::initialize()
{
	timer->resetTimer();
	presetBall=false;

	UPPER_X = sp->field.DEFENSE_ZONE_LINE;
	LOWER_X = sp->field.DEATH_ZONE_LINE;
  cutGoal=(CutGoalSkill *)skillSet->getSkill(CutGoalSkill::skillNum);
  sign=(strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation().getY()>0 ? 1 : -1);
  
  initialized = true;
}
void TandemDefenderSkill::initialize(Pair ballPos){
  initialize();
  presetBall=true;
  ball=ballPos;
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void TandemDefenderSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
	}
	else
	{
    //see if we should dive
    if(
        !presetBall &&
        towardGoal(getRobocupBall(*currentVisionData),
                   sp->field.OUR_GOAL_LINE,
                   sp->field.LEFT_GOAL_POST + GOAL_POST_TOLERANCE,
                   sp->field.RIGHT_GOAL_POST - GOAL_POST_TOLERANCE,
                   DIVE_SPEED)
      )
    {
      DiveSkill * dive= (DiveSkill *)skillSet->getSkill(DiveSkill::skillNum);
      if(!dive->isInitialized()){
        dive->initialize(getLocation(robotID,*currentVisionData,*sp).getX());
      }
      dive->run();
      return;
    }
    //always uninit dive when not used
    else
    {
      DiveSkill * dive= (DiveSkill *)skillSet->getSkill(DiveSkill::skillNum);
      dive->unInitialize();
    }

		//grab the ball location
    if(!presetBall){
      ball = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();
    }

    if(
       ball.getX() < sp->field.DEATH_ZONE_LINE+LOW_X ||
       (
        ball.getX() < sp->field.DEFENSE_ZONE_LINE &&
        (
         ball.getY() > sp->field.LEFT_SWEET_SECTOR ||
         ball.getY() < sp->field.RIGHT_SWEET_SECTOR
        )
       )
      )
    {
      //ball's too close!  call tight!
      skillSet->getSkill(TightDefenseDefenderSkill::skillNum)->initialize();
      skillSet->getSkill(TightDefenseDefenderSkill::skillNum)->run();
      return;
    }

    int newsign=(ball.getY() > sp->field.SPLIT_LINE - sign*CHANGE_BUFFER ? 1 : -1);
    if(newsign != sign){
      //Only switch if ball's not coming towards us fast
      if(ballXVel(*currentVisionData) > -BALL_SPEED){
        sign=newsign;
      }
    }
    Pair goalpost;
    goalpost.setX(sp->field.OUR_GOAL_LINE);
    if(sign > 0)
    {
      goalpost.setY(sp->field.LEFT_GOAL_POST - sp->general.PLAYER_RADIUS);
    }
    else
    {
      goalpost.setY(sp->field.RIGHT_GOAL_POST + sp->general.PLAYER_RADIUS);

    }

    //calculate goalie position
    Pair goaliePoint;
    extrapolateForY(goalpost,ball,LOWER_X - 2*sp->general.PLAYER_RADIUS,goaliePoint);
    goaliePoint.setY(goaliePoint.getY() - sign*sp->general.PLAYER_RADIUS);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,0,goaliePoint);

    //find goalie's shadow
    float y1, y2;
    findShadow(ball,goaliePoint, *sp, y1, y2);

    //call CutGoalSkill
    if(sign>0){
      if(presetBall) cutGoal->initialize(y1, sp->field.RIGHT_GOAL_POST,UPPER_X,LOWER_X,ball);
      else cutGoal->initialize(y1, sp->field.RIGHT_GOAL_POST,UPPER_X,LOWER_X);
    }else{
      if(presetBall) cutGoal->initialize(sp->field.LEFT_GOAL_POST, y2,UPPER_X,LOWER_X,ball);
      else cutGoal->initialize(sp->field.LEFT_GOAL_POST, y2,UPPER_X,LOWER_X);
    }
    cutGoal->run();
    
    blocked=false;
    //See if we're blocked
    Pair robot=getLocation(robotID,*currentVisionData,*sp);
    for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++){
      if(isBetween(robot, command->getPos(),sp->general.OTHER_TEAM,i,*currentVisionData,*sp,-32000.0f,sp->general.PLAYER_RADIUS)){
        GUI_Record.debuggingInfo.setDebugPoint(robotID,0,command->getPos());

        /********************************** BLOCKED ****************************************\
           If we can tell the position-swapper that we're stuck, here's the place to do it.
        \***********************************************************************************/
        blocked = true;

        //Don't do this.  Let Position switching switch us
        //stick self to the side of offending robot.
        /*
        strategy->getCurrentFrame()->setMessage(robotID,"Blocked! hugging opponent");
        command->setPos(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));
        if(robot.getY()<command->getYPos()){
          command->setYPos(command->getYPos() - (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS));
        }else{
          command->setYPos(command->getYPos() + (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS));
        }
        */
      }
  	}


	  //debugging only:
	  findShadow(ball,getLocation(robotID,*currentVisionData,*sp),*sp,y1,y2);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,1,sp->field.OUR_GOAL_LINE,y1);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,2,sp->field.OUR_GOAL_LINE,y2);

    //use normal control
    command->setControl(OMNI_NORMAL);
    
    //arm kicker
    if(
        ABS(getRotation(robotID,*currentVisionData,*sp)) <= KICK_ANGLE &&
        kickSafe(robotID, *currentVisionData, *sp, *strategy->getCurrentRoboCupFrame())
      )
    {
    //go kick ball if it's close enough
      command->setKick(KICK_SHOT);
    }
    
    if(
        (ball.getX() > robot.getX()) && 
        (command->getPos().distanceTo(ball) <= sp->general.PLAYER_RADIUS + KICK_DIST) &&
        (ABS(command->getPos().getX() - ball.getX()) < KICK_DIST/2.0f) &&
        kickSafe(robotID, *currentVisionData, *sp, *strategy->getCurrentRoboCupFrame())
      )
    {
      command->setRotation(AIM_ANGLE*sign);
      command->setPos(ball);
      strategy->getCurrentFrame()->setMessage(robotID,"Tandem: Kicking ball away");

    }
	}
}
//=====================================================
///If the timer goes past 3 secs, reverse rotation
bool TandemDefenderSkill::evaluate() 
{
	return true;
}



//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void TandemDefenderSkill::update() 
{
	///at this time simple screening does not "learn"
}


//Screen is never finished, it can always continue
bool TandemDefenderSkill::isFinished(float tolerance){
	return blocked;
}




///Reads in the values of the parameters from the skill's parameter text file.
void TandemDefenderSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/TandemDefenderSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
	
	//default X position if not specified when skill initialized or the X position is not within the range required
	READ_FLOAT(BALL_SPEED);		
	READ_FLOAT(LOW_X);		
	READ_FLOAT(CHANGE_BUFFER);		
	READ_FLOAT(KICK_ANGLE);
  READ_FLOAT(AIM_ANGLE);
  READ_FLOAT(KICK_DIST);
  READ_FLOAT(DIVE_OFFSET);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();

  //***Shared params for Diving***
  mainFile.open("Params/Skill_Params/TandemDiveParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
	
	//default X position if not specified when skill initialized or the X position is not within the range required
  READ_FLOAT(DIVE_SPEED);
  READ_FLOAT(GOAL_POST_TOLERANCE);


	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();

}