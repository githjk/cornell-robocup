#include "PassBlockSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "SimpleScreenSkill.h"

int PassBlockSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
PassBlockSkill::PassBlockSkill(SystemParameters *sp,
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
bool PassBlockSkill::isValid() { return true; }




//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void PassBlockSkill::initialize() {
  GUI_Record.debuggingInfo.addDebugMessage("Please initialize PassBlock with an opponent's ID");
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void PassBlockSkill::initialize(RobotIndex opponent) {
  initialized = true;
  opponentID=opponent;
}




//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void PassBlockSkill::execute()
{    
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data

  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("PassBlock not initialized!");    
    return;  
  }
  ((SimpleScreenSkill *)(skillSet->getSkill(SimpleScreenSkill::skillNum)))->
      initialize(SCREEN_OPPONENT_ROBOT,
                 opponentID,
                 SCREEN_BALL,
                 0,
                 SCREEN_BALL,
                 0);
  ((SimpleScreenSkill *)(skillSet->getSkill(SimpleScreenSkill::skillNum)))->run();
  strategy->getCurrentFrame()->setMessage(robotID,"Performing Pass Block");
}

///If the timer goes past 3 secs, reverse rotation
bool PassBlockSkill::evaluate() {
  return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void PassBlockSkill::update() {
  ///If not initialized, dont do anything!
  if(!initialized) return;    
}



//===============================================================================
///test skill goes forever
bool PassBlockSkill::isFinished(float tolerance){
  return false;
}




//===============================================================================
void PassBlockSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/PassBlockSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}