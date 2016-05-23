#include "KickUpfieldSkill.h"
#include "SimpleKickSkill.h"
#include "SpinSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "common/util/pair/pair.h"
#include "common/util/math/angle.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "PullBallOffWallSkill.h"

int KickUpfieldSkill::skillNum = -1;

//===============================================================================
///Constructor.  Calls the base class Skill constructor
KickUpfieldSkill::KickUpfieldSkill(SystemParameters *sp,
                                   VisionModule *vm, 
                                   RoboCupStrategyModule *strat,
                                   RobotIndex id, 
                                   SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool KickUpfieldSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void KickUpfieldSkill::initialize() 
{
  mode = 1;
  timer->resetTimer();
  initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void KickUpfieldSkill::execute()
{
  ///If not initialized, dont do anything!
  if(!initialized) 
  {
    GUI_Record.debuggingInfo.addDebugMessage("KickUpfieldSkill not initialized!");    
    return;  
  }

  if(ABS(angleDifference(
          getRotation(robotID, *currentVisionData, *sp),
          angleBetween(getLocation(robotID,*currentVisionData,*sp),Pair(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE))
          )) < ANGLE_THRESHOLD &&
    mode == 1)
  {
    mode = 2;
  }
  
  //if we need to pull it off the wall or the corner
  PullBallOffWallSkill * pull = (PullBallOffWallSkill *)skillSet->getSkill(PullBallOffWallSkill::skillNum);

  //pull off wall
  if(pull->isValid())
  {
    strategy->getCurrentRoboCupFrame()->setMessage(robotID, "KUFS pull off wall");
    if(!pull->isInitialized()){
      pull->initialize();
    }
	  pull->run();
  }
  //rotate
  else if(mode == 1)
  {
    strategy->getCurrentRoboCupFrame()->setMessage(robotID, "KUFS rotating");
    //somehwere upfield, so dest rot will be 0
    Pair dest(getLocation(robotID,*currentVisionData,*sp));
    dest.setX(dest.getX() + 10.0f);

    if(!skillSet->getSkill(SpinAroundBallSkill::skillNum)->isInitialized())
      ((SpinAroundBallSkill*)skillSet->getSkill(SpinAroundBallSkill::skillNum))->initialize(dest);
    skillSet->getSkill(SpinAroundBallSkill::skillNum)->run();

    //keep resetting timer so it doesn't transition out until kick has occured
    timer->resetTimer();
  }
  //kick
  else
  {
    strategy->getCurrentRoboCupFrame()->setMessage(robotID, "KUFS kicking");
    if(!skillSet->getSkill(SimpleKickSkill::skillNum)->isInitialized())
    ((SimpleKickSkill*)skillSet->getSkill(SimpleKickSkill::skillNum))->initialize(KICK_SHOT);
    skillSet->getSkill(SimpleKickSkill::skillNum)->run();
  }
}
//===============================================================================
bool KickUpfieldSkill::evaluate() 
{
  return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void KickUpfieldSkill::update() 
{
  return;
}
//===============================================================================
//===============================================================================
///test skill goes forever
bool KickUpfieldSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
//===============================================================================
void KickUpfieldSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/KickUpfieldSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(ANGLE_THRESHOLD);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
//===============================================================================
