#ifndef SIMPLE_DEFENSE_SKILL_H
#define SIMPLE_DEFENSE_SKILL_H
#include "skill.h"

class SimpleDefenseSkill : public Skill
{
public:
  static int skillNum;
  /**@name    public functions  */
  ///Constructor.  Calls the base class Skill constructor
  SimpleDefenseSkill(SystemParameters *sp,
                     VisionModule *vm, 
                     RoboCupStrategyModule *strat,
                     RobotIndex id, 
                     SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();
  
  ///always call the more specific init function below
  void initialize() { }
 
  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize(float DefenceX);
  
  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();
  
  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();
 
  ///always false
  bool isFinished(float tolerance);
  
  ///Reads in parameters for this skill
  void loadValues();
  
  ///Returns the skill's name
  char* getName() const { 
	  return "SimpleDefenseSkill"; 
 }
  //}@
protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  float constX;
  Pair pointA, pointB, pointC;
  
  //these are the point "types"  
  #define SCREEN_FRIENDLY_ROBOT 0
  #define SCREEN_OPPONENT_ROBOT 1
  #define SCREEN_BALL 2
 
  // default robot location
  float DEFAULT_DEFENSE_X;
};

#endif //SIMPLE_DEFENSE_SKILL_H



