#include "SupplementThreeManSkill.h"
#include "AcquirePossessionSkill.h"
#include "info/robot/robot.h"
#include "info/ball/ball.h"
#include "math/angle.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"


int SupplementThreeManSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
SupplementThreeManSkill::SupplementThreeManSkill(SystemParameters *sp,
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
bool SupplementThreeManSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void SupplementThreeManSkill::initialize() 
{
  timer->resetTimer();
  initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void SupplementThreeManSkill::execute()
{    
  ///If not initialized, dont do anything!
  if(!initialized) 
  {
    return;  
  }

  //if the ball is close enough to defenisve players, move next to 
  //the defensive player that is closer to the ball
  //since he'll become the aggresor and move upfield. 
  //this way we can slide in and form the defensive wall immediately.
  Pair ballLoc = getBallLocation(*currentVisionData);
  Pair robotLoc = getLocation(robotID, *currentVisionData, *sp);
  RobotIndex closeDefensiveID = NO_ROBOT;
  float closestDistance;

  RobotIndex tempID;
  float tempDistance;

  tempID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER);
  if(tempID != NO_ROBOT)
  {
    closestDistance = getLocation(tempID, *currentVisionData, *sp).distanceTo(ballLoc);
    closeDefensiveID = tempID;
  }

  tempID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER);
  if(tempID != NO_ROBOT)
  {
    tempDistance = getLocation(tempID, *currentVisionData, *sp).distanceTo(ballLoc);
    if(tempDistance < closestDistance)
    {
      closestDistance = tempDistance;
      closeDefensiveID = tempID;
    }
    if(closeDefensiveID == strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER))
      closeDefensiveID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER);
  }

  tempID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_DEFENDER);
  if(tempID != NO_ROBOT)
  {
    tempDistance = getLocation(tempID, *currentVisionData, *sp).distanceTo(ballLoc);
    if(tempDistance < closestDistance)
    {
      closestDistance = tempDistance;
      closeDefensiveID = tempID;
    }
    if(closeDefensiveID == strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER))
      closeDefensiveID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_DEFENDER);
  }
  
  //if closest defensive player 
  //a.) exists
  //b.) within tolerance, then go to side of him
  if(closestDistance < 3.0f*sp->general.PLAYER_RADIUS)
  {
    Pair defensivePlayer = getLocation(closeDefensiveID, *currentVisionData, *sp);
    if(defensivePlayer.getY() > sp->field.SPLIT_LINE)
    {
      command->setYPos(defensivePlayer.getY() + sp->general.PLAYER_RADIUS + 0.02f);
    }
    else
    {
      command->setYPos(defensivePlayer.getY() - sp->general.PLAYER_RADIUS - 0.02f);
    }
    command->setXPos(defensivePlayer.getX());
    command->setRotation(angleBetween(robotLoc, ballLoc));

  }
  //else acquire the ball
  else
  {
    Skill* skillHandle = skillSet->getSkill(AcquirePossessionSkill::skillNum);
    if(!skillHandle->isInitialized())
      skillHandle->initialize();
    skillHandle->run();
  }
}
//===============================================================================
///If the timer goes past 3 secs, reverse rotation
bool SupplementThreeManSkill::evaluate() 
{
  return true;
}
//===============================================================================
void SupplementThreeManSkill::update() 
{
}
//===============================================================================
///test skill goes forever
bool SupplementThreeManSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void SupplementThreeManSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/SupplementThreeManSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================
