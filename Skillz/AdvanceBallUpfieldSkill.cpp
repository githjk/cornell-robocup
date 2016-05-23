#include "AdvanceBallUpfieldSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/math/dist.h"
#include "common/util/shoot/shoot.h"
#include "JukeAroundOpponentSkill.h"
#include "TurnAndKickSkill.h"
#include "DribblePointSkill.h"
#include "SpinSkill.h"
#include "PullBallOffWallSkill.h"
#include "math/angle.h"

int AdvanceBallUpfieldSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
AdvanceBallUpfieldSkill::AdvanceBallUpfieldSkill(SystemParameters *sp,
                                                 VisionModule *vm, 
                                                 RoboCupStrategyModule *strat,
                                                 RobotIndex id, 
                                                 SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();
  initialized = false;
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool AdvanceBallUpfieldSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void AdvanceBallUpfieldSkill::initialize() 
{
  timer->resetTimer();
  initialized = true;
  pick=false;
  juking=false;
  passing=false;
  starting=true;
  pulling=false;
  spinning=false;

  destination.set(sp->field.HALF_LINE,sp->field.SPLIT_LINE);
}

float AdvanceBallUpfieldSkill::threatDist(){
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  float minDist=32000.0f;
  Pair opLoc;
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS;i++){
    if(robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp)){
      opLoc=getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
      //check if they're a threat
      if(dist(opLoc,currentPos) < minDist &&
         opLoc != currentPos &&
         ABS(angleBetween(currentPos,opLoc)) < THREAT_ANGLE){
        minDist=dist(opLoc,currentPos);
      }
    }
  }
  return minDist;

}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void AdvanceBallUpfieldSkill::execute()
{    
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));

  float wbuf=WALL_BUFFER+sp->general.PLAYER_RADIUS;
  
  //Choose a destination downfield
  //if open shot on net, drive towards it, else pick default 
  //point on same side of the field
  if(!calcShot(robotID, 
               sp->field.THEIR_GOAL_LINE,
               sp->strategy2002.SHOOT_LANE_THRESH,
               sp->field.RIGHT_GOAL_POST,
               sp->field.LEFT_GOAL_POST,
               NO_ROBOT,
               *currentVisionData,
               *sp,
               &destination))
  {
    //make destination center of their goal line
    destination.set(sp->field.THEIR_GOAL_LINE,
                    sp->field.SPLIT_LINE);
  }

  PullBallOffWallSkill * pull = (PullBallOffWallSkill *)skillSet->getSkill(PullBallOffWallSkill::skillNum);
  SpinAroundBallSkill * spin = (SpinAroundBallSkill *)skillSet->getSkill(SpinAroundBallSkill::skillNum);
  spin->initialize(destination);

  if(starting)
  {
    if(pull->isValid())
    {
      pull->initialize();
      pulling=true;
    }
    else
    {
      spin->initialize(destination);
      if(threatDist() < THREAT_DIST + sp->general.PLAYER_RADIUS +sp->general.OPPONENT_RADIUS)
        faceAngle=BACKWARDS_ANGLE_THREATENED;
      else
        faceAngle=BACKWARDS_ANGLE;
      if(!spin->isFinished(faceAngle))
      {
        spinning = true;
      }
    }
    starting = false;
  }

  if(pulling)
  { 
    // we're pulling the ball off the wall
    strategy->getCurrentFrame()->setMessage(robotID,"Pulling ball from wall");
    pull->run();
    if(pull->isFinished())
    {
      pulling=false;
    }
    else
    {
      return;
    }
  }

  if(spinning)
  {
    // we're facing upfield before we get going.
    strategy->getCurrentFrame()->setMessage(robotID,"Turning Around");
    spin->run();
    if(spin->isFinished(faceAngle))
    {
      spinning=false;
    }
    else
    {
      return;
    }
  }

  if(juking)
  {
    JukeAroundOpponentSkill * juke = (JukeAroundOpponentSkill *)skillSet->getSkill(JukeAroundOpponentSkill::skillNum);
    if(juke->isFinished(0.0f)){
      juking=false;
      juke->unInitialize();
    }else{
      juke->run();
      return;
    }
  }
  /*
  if(passing)
  {
    //once we've decided to kick, kick forever.
    TurnAndKickSkill * kick = (TurnAndKickSkill *)skillSet->getSkill(TurnAndKickSkill::skillNum);
    kick->run();
    strategy->getCurrentFrame()->setMessage(robotID,"Passing to Creator");
  }
  */
  Pair opLoc;
  //find threatening/blocking robots
  float tdist=threatDist();
  if( tdist< THREAT_DIST + sp->general.PLAYER_RADIUS +sp->general.OPPONENT_RADIUS){
    JukeAroundOpponentSkill * juke = (JukeAroundOpponentSkill *)skillSet->getSkill(JukeAroundOpponentSkill::skillNum);
    if(tdist < BLOCKING_DIST + sp->general.PLAYER_RADIUS +sp->general.OPPONENT_RADIUS &&
      juke->isValid()){
      //*currently, just juke*
      juke->initialize(destination);
      juke->run();
      juking=true;
      return;
    }else{
      //we have a threat, try to pass, pick, etc.
      
      //*currently, just try to pass*
      //look for pass to creator (but don't kick!)
      RobotIndex creatorID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(CREATOR);
      Pair creatorPos=getLocation(creatorID,*currentVisionData,*sp);
      if(robotFound(creatorID,*currentVisionData,*sp) &&
        strategy->getCurrentRoboCupFrame()->getPassValue(creatorID))
      {
        TurnAndKickSkill * kick = (TurnAndKickSkill *)skillSet->getSkill(TurnAndKickSkill::skillNum);
        kick->initialize(strategy->getCurrentRoboCupFrame()->getPassDest(creatorID),NO_KICK,true);
        kick->run();
        strategy->getCurrentFrame()->setMessage(robotID,"Beginning Pass to Creator");
        passing=true;
        return;
      }
    }
  }

  RobotIndex specialID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  int ignoreNum=0;
  if(robotFound(specialID,*currentVisionData,*sp)) ignoreNum=1; //see if there's an SOA to ignore

  //no threats, use dribblePoint
  DribblePointSkill * dribble = (DribblePointSkill *)skillSet->getSkill(DribblePointSkill::skillNum);
  if(!dribble->isInitialized()) dribble->initialize(destination,&specialID,ignoreNum);
  dribble->run();


}
//===============================================================================
bool AdvanceBallUpfieldSkill::evaluate() 
{
	return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void AdvanceBallUpfieldSkill::update() 
{
}

///returns if we're passing or not
bool AdvanceBallUpfieldSkill::isPassing() {return passing;}

//===============================================================================
///either we have a shot or we've gotten where we were going.
bool AdvanceBallUpfieldSkill::isFinished(float tolerance)
{
  currentVisionData = visionModule->getCurrentFrame();
  return (dist(robotID,destination,*currentVisionData,*sp) <= THERE_DIST) ||
          goodCurrentShot(getLocation(robotID,*currentVisionData,*sp),
                        getRotation(robotID,*currentVisionData,*sp),
                        *currentVisionData,
                        *sp,
                         sp->strategy2002.SHOOT_LANE_THRESH);
}
//===============================================================================
void AdvanceBallUpfieldSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/AdvanceBallUpfieldSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(WALL_BUFFER);
  READ_FLOAT(THREAT_DIST);
  READ_FLOAT(THREAT_ANGLE);
  READ_FLOAT(BLOCKING_DIST);
  READ_FLOAT(DEST_X);
  READ_FLOAT(DEST_Y);
  READ_FLOAT(HYST_DIST);
  READ_FLOAT(BACKWARDS_ANGLE);
  READ_FLOAT(BACKWARDS_ANGLE_THREATENED);
  READ_FLOAT(THERE_DIST);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
