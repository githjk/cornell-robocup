#include "ThreeManBlockerSkill.h"
#include "DiveSkill.h"
#include "CutGoalSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/defense/defending.h"
#include "common/util/info/ball/ball.h"
#include "GUI/GUI.h"



int ThreeManBlockerSkill::skillNum = -1;



///Constructor.  Calls the base class Skill constructor
ThreeManBlockerSkill::ThreeManBlockerSkill(SystemParameters *sp,
                                           VisionModule *vm, 
                                           RoboCupStrategyModule *strat,
                                           RobotIndex id, 
                                           SkillSet* set) : Skill(sp,vm,strat,id, set)
{
  loadValues();
  initialized = false;
}

/// Check to see if we have the ball or not.
bool ThreeManBlockerSkill::isValid() 
{ 
  return true;
}

//Initializations
void ThreeManBlockerSkill::initialize() {
  initialize(line_x);
}
void ThreeManBlockerSkill::initialize(float lineX,Pair ballPos) {
  initialize(lineX);
  presetBall=true;
  ball=ballPos;
}
void ThreeManBlockerSkill::initialize(float lineX) 
{
  presetBall=false;
  timer->resetTimer();
  initialized=true;
  line_x=lineX;

}
void ThreeManBlockerSkill::initialize(Pair ballPos) 
{
  initialize(line_x,ballPos);
}


//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void ThreeManBlockerSkill::execute()
{    

  ///If not active, dont do anything!
  if(!initialized) return; 
    //see if we should dive
  Pair currentPos=getLocation(robotID,*currentVisionData,*sp);
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
    if(nearOurGoalieBox(currentPos,*sp,sp->general.PLAYER_RADIUS))command->setControl(OMNI_GOALIE);
    else command->setControl(OMNI_NORMAL_ENTERBOX);

/*    //constrain to betwen goal posts
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
    
    command->setSpeed(GOALIE_SPEED);
    return;
  }
  //always uninit dive when not used
  else
  {
    DiveSkill * dive= (DiveSkill *)skillSet->getSkill(DiveSkill::skillNum);
    dive->unInitialize();
  }

  float up=line_x;
  if(badRobot(DEFENDER,DEFENDER_DIST,*currentVisionData,*strategy->getRelativeRoboCupFrame(-1),*sp) ||
     badRobot(SPECIAL_OP_DEFENDER,DEFENDER_DIST,*currentVisionData,*strategy->getRelativeRoboCupFrame(-1),*sp) ){
    up=upper_x;
  }else{
    up=line_x;
  }
  if(presetBall){
    ((CutGoalSkill *)skillSet->getSkill(CutGoalSkill::skillNum))->initialize(sp->field.LEFT_GOAL_POST,
                                                                             sp->field.RIGHT_GOAL_POST,
                                                                             up,line_x,ball);
  }else{
    ((CutGoalSkill *)skillSet->getSkill(CutGoalSkill::skillNum))->initialize(sp->field.LEFT_GOAL_POST,
                                                                             sp->field.RIGHT_GOAL_POST,
                                                                             up,line_x);
  }

  skillSet->getSkill(CutGoalSkill::skillNum)->run();
  if(inOurGoalieBox(currentPos,*sp))command->setControl(OMNI_GOALIE);
  else command->setControl(OMNI_NORMAL_ENTERBOX);
  command->setSpeed(GOALIE_SPEED);
    //debugging only:
    float y1,y2;
	  findShadow(getBallLocation(*currentVisionData),getLocation(robotID,*currentVisionData,*sp),*sp,y1,y2);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,1,sp->field.OUR_GOAL_LINE,y1);
	  GUI_Record.debuggingInfo.setDebugPoint(robotID,2,sp->field.OUR_GOAL_LINE,y2);

}

///always true for now.
bool ThreeManBlockerSkill::evaluate() {
  return true;
}

//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void ThreeManBlockerSkill::update() {
  
}

///check if we've reached our target destination and rotation
bool ThreeManBlockerSkill::isFinished(float tolerance)
{
  return false;
}




///Reads in the values of the parameters from the skill's parameter text file.
void ThreeManBlockerSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/ThreeManBlockerSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
	READ_FLOAT(LINE_X_OFFSET);
  line_x=sp->field.OUR_GOALIE_BOX -1*sp->general.PLAYER_RADIUS + LINE_X_OFFSET;
  READ_FLOAT(UPPER_X_OFFSET);
  upper_x=sp->field.DEATH_ZONE_LINE -2*sp->general.PLAYER_RADIUS+UPPER_X_OFFSET;
  READ_FLOAT(DEFENDER_DIST);
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