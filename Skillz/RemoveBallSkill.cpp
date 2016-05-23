#include "RemoveBallSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"

int RemoveBallSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
RemoveBallSkill::RemoveBallSkill(SystemParameters *sp,
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
bool RemoveBallSkill::isValid() 
{ 
  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void RemoveBallSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
  finished=false;
  dest.setX(sp->field.OUR_GOALIE_BOX-sp->general.PLAYER_RADIUS);
  if(getLocation(robotID,*currentVisionData,*sp).getY()>sp->field.SPLIT_LINE) {
    side=1;
    dest.setY(sp->field.LEFT_GOAL_POST - EDGE_DIST);
  }else{
    side=-1;
    dest.setY(sp->field.RIGHT_GOAL_POST + EDGE_DIST);
  }

}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void RemoveBallSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }
  command->setControl(OMNI_GOALIE);

  if(finished){
    command->setDribble(NO_DRIBBLE);
    command->setVerticalDribble(NO_V_DRIBBLE);
    command->setKick(KICK_SLOW);
    strategy->getCurrentFrame()->setMessage(robotID,"Kicking ball to the side");
    return;
  }
	RobotIndex aggID;
  aggID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);
  Pair currentPos=getLocation(robotID,*currentVisionData,*sp);
  Pair aggressorLoc;
  Pair alternateLoc;
  //find the aggressor's location
  aggressorLoc=getLocation(aggID,*currentVisionData,*sp);
  aggressorLoc.setX(aggressorLoc.getX() + KICK_X_OFFSET);

  //create a default location to kick to.
  alternateLoc.setX(currentPos.getX());
  strategy->getCurrentFrame()->setMessage(robotID,"No Aggressor to pass to.  HELP!");
  if(side > 0){
    alternateLoc.setY(sp->field.LEFT_GOALIE_BOX+sp->general.PLAYER_RADIUS);
  }else{
    alternateLoc.setY(sp->field.RIGHT_GOALIE_BOX-sp->general.PLAYER_RADIUS);
  }

  //slow robot down for careful maneuvering.
  command->setSpeed(CAREFUL_SPEED);
  command->setDribble(SLOW_DRIBBLE);
  command->setVerticalDribble(SLOW_V_DRIBBLE);
  
  strategy->getCurrentFrame()->setMessage(robotID,"Turning to the side");
  //go to and face the closest side
  command->setRotation(side*FACE_ANGLE);
  if(ABS(angleDifference(getRotation(robotID,*currentVisionData,*sp),side*FACE_ANGLE)) <= SET_ANGLE){
    strategy->getCurrentFrame()->setMessage(robotID,"Heading to the side");
    //angle=side*FACE_ANGLE;
    command->setPos(dest);
  }

  if(dest.distanceTo(getLocation(robotID,*currentVisionData,*sp))<THERE_DIST){
    //now face the aggressor if he's there.
    float aggAngle=angleBetween(currentPos,aggressorLoc);
    GUI_Record.debuggingInfo.setDebugPoint(robotID,0,aggressorLoc);
    if(aggID == NO_ROBOT || ABS(angleDifference(PI/2*side,aggAngle)) < SIDE_ANGLE){
      aggAngle = angleBetween(currentPos,alternateLoc);
      GUI_Record.debuggingInfo.setDebugPoint(robotID,0,alternateLoc);
    }
    command->setRotation(aggAngle);// face aggressor
    strategy->getCurrentFrame()->setMessage(robotID,"Facing the aggressor");
    if(ABS(angleDifference(aggAngle,getRotation(robotID,*currentVisionData,*sp))) <= THERE_ANGLE){
      strategy->getCurrentFrame()->setMessage(robotID,"Kicking to the side");
      command->setDribble(NO_DRIBBLE);
      command->setVerticalDribble(NO_V_DRIBBLE);
      command->setKick(KICK_SLOW);
      finished=true;
    }
  }
}
//=====================================================
bool RemoveBallSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void RemoveBallSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool RemoveBallSkill::isFinished(float tolerance)
{
	return finished;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void RemoveBallSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/RemoveBallSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(EDGE_DIST);
  READ_FLOAT(FACE_ANGLE);
  READ_FLOAT(SET_ANGLE);
  READ_FLOAT(THERE_DIST);
  READ_FLOAT(THERE_ANGLE);
  READ_FLOAT(KICK_X_OFFSET);
  READ_FLOAT(SIDE_ANGLE);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
