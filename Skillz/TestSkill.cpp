#include "TestSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"


int TestSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
TestSkill::TestSkill(SystemParameters *sp,
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
bool TestSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void TestSkill::initialize() 
{
  timer->resetTimer();
  spinCoeff = 1.0f;
  initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void TestSkill::execute()
{    
  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("TestSkill not initialized!");    
    return;  
  }
  

  float destRot = getRotation(robotID, (*currentVisionData), (*sp)) + spinCoeff;
  if(destRot < PI)
    destRot = -2*PI + destRot;

  command->setRotation( destRot );
  
  printf("TEST_PARAM = %d\n", TEST_PARAM);
  SkillSet* skills = strategy->getSkillSet(robotID);
  skills->getSkill(TestSkill::skillNum)->run();
}

///If the timer goes past 3 secs, reverse rotation
bool TestSkill::evaluate() 
{
  ///If not initialized, dont do anything!
  if(!initialized) return true;  

  double oldTime = timer->currentTime();
  if(oldTime < 3.0 && timer->currentTime() >= 3.0) spinCoeff *= -1.0f;

  return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void TestSkill::update() 
{
  ///If not initialized, dont do anything!
  if(!initialized) return;    
}
//===============================================================================
///test skill goes forever
bool TestSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void TestSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/TestSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  ///Read in a test param
  READ_INT(TEST_PARAM);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================
