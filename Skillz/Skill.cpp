#include "Skill.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"

  //======================================
  ///The constructor takes in strategy data, vision data, and the index of
  ///the robot performing the skill, and initializes all protected variables
  ///with that data. 
  Skill::Skill(SystemParameters *sp,
               VisionModule *vm, 
               RoboCupStrategyModule *strat,
               RobotIndex id,
               SkillSet* set) 
  {    
    //--
    //set internal pointers
    //--
    //global params
    this->sp = sp;

    //vision module
    visionModule = vm;

    //destination
    strategy = strat;

	  skillSet = set;

    //robot index
    robotID = id;

    initialized = false;

    timer = new VisionTimer(vm);
  }
  //======================================
  ///The public function called from outside the skill that calls the skill's
  ///execute function.  It prevents execute() from being called more than once,
  ///preventing infinite loops.
  bool Skill::run() 
  {
    //Dont run the execute() function if it already ran this frame
    if(!hasBeenRun) {
      //Make sure to set hasBeenRun to true BEFORE calling execute()
      hasBeenRun = true;
      execute();
      hasBeenRun = false;
      return true;
    }
    else 
    {
      GUI_Record.debuggingInfo.addDebugMessage("ERROR - skill trying to run again this frame.");
      printf("ERROR - ");
      printf(getName());
      printf(" trying to run again this frame.\n");
      return false;
    }
  }
  
  //======================================
  ///Sets initialized value to false, preformed when positions swtich, or plays transition
  void Skill::unInitialize()
  { 
    initialized = false; 
  }
  //======================================
  ///Updates vision data and strategy data pointers
  void Skill::updatePointers()
  {
    currentVisionData = visionModule->getCurrentFrame();
    command = strategy->getCurrentFrame()->getDestination(robotID);
  }
  //======================================
