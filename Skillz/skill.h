#ifndef SKILL_H
#define SKILL_H

#include "ai.h"
#include "VisionModule.h"
#include "SkillSet.h"
#include "common/datatypes/ParameterOperations.h"
#include "common/datatypes/SystemParameters.h"
#include "common/vision/visionTimer.h"

//#include "GUI/GUI.h"

//begin hack
class SkillSet;
//end hack

/**
The class Skill, used as an interface for all other skills since a number
of its functions are pure virtual (must be overridden, class cannot be
instanciated).  Each time a robot must perform a skill, the robot runs the 
execute() method of its instance of that skill.  Calling execute() makes the
robot perform the skill.
NOTE: we never create an instance of this class, only classes that 
inherit from it. */
class Skill : public ParameterOperations
{

protected:
  /**@name    protected variables  */
  //@{            

  ///pointer to RoboCup parameters
  SystemParameters *sp;

  ///pointer to for controlling the robot
  RoboCupStrategyModule *strategy;

  ///vision module
  VisionModule *visionModule;

  //at beginning of execute, always call write down local copy of vision data
  VisionData* currentVisionData;

  ///SkillSet pointer
  SkillSet *skillSet;

  //at beginnign of execute, always get this frames command
  Destination* command;

  ///ID of the robot using the skill
  RobotIndex robotID;

  ///a timer
  VisionTimer *timer;

  ///indicates if skill is active or not
  bool initialized;
  
  ///Indicates if the skill has been run this frame
  bool hasBeenRun;
  
    
  //}@

public:  
  /**@name    public functions  */
  //@{

  //======================================
  ///The constructor takes in strategy data, vision data, and the index of
  ///the robot performing the skill, and initializes all protected variables
  ///with that data. 
  Skill(SystemParameters *sp,
        VisionModule *vm, 
        RoboCupStrategyModule *strat,
		RobotIndex id,
		SkillSet* set);
  //======================================
  ///The destructor
  virtual ~Skill()
  {
    //destroy the timer
    delete timer;
  }

  //======================================
  ///The public function called from outside the skill that calls the skill's
  ///execute function.  It prevents execute() from being called more than once,
  ///preventing infinite loops.
  bool run();
  
  //======================================
  ///Call this function to check the prerequisites of the skill.  This will return 
  ///a bool indicating whether or not the skill is viable in the present situation.
  virtual bool isValid() = 0;
  //======================================
  ///Perform any initializations for the skill, such as resetting the timer.
  virtual void initialize() = 0;  
  //======================================
  ///Sets initialized value to false, preformed when positions swtich, or plays transition
  void unInitialize();
  //======================================
  ///Evaluates the skill's performance, and sees if it can improve its behavior.
  ///Also indicates if the skill is still viable (whether it is frustrated or not).
  ///True = skill is okay.  False = skill is frustrated.
  virtual bool evaluate() = 0;
  //======================================
  ///For tweaking the skill.  You may want to change local parameters or behave 
  ///differently to adapt to any situation that is frustrating the skill. 
  virtual void update() = 0;
  //======================================
  ///indicates if the skill is active or not
  virtual bool isInitialized()
  {
    return initialized;
  }
  //======================================
  ///indicates if the skill is finished or not, within the given tolerance
  virtual bool isFinished(float tolerance=0.0f)
  {
    return false;
  }
  //======================================
  /// Loads the parameter values from a text file
  virtual void loadValues() = 0;
  //======================================  
  ///Sets hasBeenRun to false so the skill can run next frame
  void resetSkill()
  { 
    hasBeenRun = false; 
  }
  //======================================
  ///Returns the skill's name
  virtual char* getName() const = 0;
  //======================================
  ///Updates vision data and strategy data pointers
  void updatePointers();
  //======================================
  //}@
  

protected:
  ///Execute the skill.  This is the main part of the skill, where you tell the
  ///robot how to perform the skill.
  virtual void execute() = 0;
};

#endif //SKILL_H