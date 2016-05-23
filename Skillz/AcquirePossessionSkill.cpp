#include "AcquirePossessionSkill.h"
#include "InterceptBallSkill.h"
#include "StripBallSkill.h"
#include "StiffarmOffenseSkill.h"
#include "PullBallOffWallSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"


int AcquirePossessionSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
AcquirePossessionSkill::AcquirePossessionSkill(SystemParameters *sp,
                                               VisionModule *vm, 
                                               RoboCupStrategyModule *strat,
                                               RobotIndex id, 
                                               SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  initialized = false;
}
//=====================================================
//Delete all pointers
AcquirePossessionSkill::~AcquirePossessionSkill() 
{
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool AcquirePossessionSkill::isValid() 
{ 
  return isRobocupBallFound(*currentVisionData);
}
//===============================================================================
//Perform initializations for the moveToBallSkill and interceptBallSkill
void AcquirePossessionSkill::initialize() 
{
	initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void AcquirePossessionSkill::execute() 
{    
	///If not active, dont do anything!
	if(!initialized)
  {
		return;  
  }
  else
  {
    StripBallSkill* strip = (StripBallSkill*)skillSet->getSkill(StripBallSkill::skillNum);
    StiffarmOffenseSkill* stiff = (StiffarmOffenseSkill*)skillSet->getSkill(StiffarmOffenseSkill::skillNum);
    InterceptBallSkill* interceptBallSkill = (InterceptBallSkill*)skillSet->getSkill(InterceptBallSkill::skillNum);
    PullBallOffWallSkill* pullBallOffWallSkill = (PullBallOffWallSkill*)skillSet->getSkill(PullBallOffWallSkill::skillNum);
    RobotIndex defenderID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER);
    RobotIndex blockerID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER);
    RobotIndex specopID=strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_DEFENDER);
    Pair ballPos = getBallLocation(*currentVisionData);

    Pair robotPos= getLocation(robotID,*currentVisionData,*sp);
    float sepDist= 2 * sp->general.PLAYER_RADIUS;

    
    //if the ball on the wall, pull it off the wall
    if(pullBallOffWallSkill->isValid())
    {
      if(!pullBallOffWallSkill->isInitialized())
        pullBallOffWallSkill->initialize();
      strategy->getCurrentFrame()->setMessage(robotID, "running pull off wall");
      pullBallOffWallSkill->run();
    }

    //if they have the ball, strip
    else if(
             strip->isValid() &&
             (
               defenderID==NO_ROBOT || 
               getLocation(defenderID,*currentVisionData,*sp).distanceTo(ballPos) >= sepDist
             ) &&
             (
               blockerID==NO_ROBOT || 
               getLocation(blockerID,*currentVisionData,*sp).distanceTo(ballPos) >= sepDist
             ) &&
             (
               specopID==NO_ROBOT || 
               getLocation(specopID,*currentVisionData,*sp).distanceTo(ballPos) >= sepDist
             )
           ) 
    {
      interceptBallSkill->unInitialize();
      if(!strip->isInitialized())
      {
        strip->initialize();
      }
      strategy->getCurrentFrame()->setMessage(robotID, "running strip");
      strip->run();
    }
      
      /*
      //decide whether it's safe to strip or not.
      float sepDist= 2 * sp->general.PLAYER_RADIUS;
      if(strip->isValid() &&
         (defenderID==NO_ROBOT || getLocation(defenderID,*currentVisionData,*sp).distanceTo(ballPos) >= sepDist) &&
         (blockerID==NO_ROBOT || getLocation(blockerID,*currentVisionData,*sp).distanceTo(ballPos) >= sepDist) &&
         (specopID==NO_ROBOT || getLocation(specopID,*currentVisionData,*sp).distanceTo(ballPos) >= sepDist)
        ){
        if(!strip->isInitialized())
        {
          strip->initialize();
        }
        strategy->getCurrentFrame()->setMessage(robotID, "running strip");
        strip->run();
      }else{
        if(!stiff->isInitialized()){
          stiff->initialize(DEFENDER);
        }
        stiff->run();
      }      
      */
    //else go after ball
    else
    {
      strip->unInitialize();
      if(!interceptBallSkill->isInitialized())
      {
        interceptBallSkill->initialize();
      }
      strategy->getCurrentFrame()->setMessage(robotID, "running intercept");
      interceptBallSkill->run();
    }
  }
}
//===============================================================================
bool AcquirePossessionSkill::evaluate() 
{
	return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void AcquirePossessionSkill::update() 
{

}
//===============================================================================
//Done if we have the ball
bool AcquirePossessionSkill::isFinished(float tolerance)
{
	return friendlyHasPossession(robotID,  
                               *sp, 
                               *currentVisionData, 
                               *(RobocupStrategyData*)strategy->getCurrentFrame(), 
                               true);
}
//===============================================================================
void AcquirePossessionSkill::loadValues() 
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/AcquirePossessionSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
}
//===============================================================================
