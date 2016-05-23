#ifndef DOUBLE_PICK_BLOCK_SKILL_H
#define DOUBLE_PICK_BLOCK_SKILL_H

#include "skill.h"
#include "common/datatypes/robocup_strategy_data.h"
class DoublePickBlockSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  DoublePickBlockSkill(SystemParameters *sp,
                       VisionModule *vm, 
                       RoboCupStrategyModule *strat,
                       RobotIndex id, 
                       SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
 void initialize();

 ///initialize with the position
 void initialize(Position rp, float offset);
  
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
	  return "DoublePickBlockSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  Position posOfInterest;
  RobotIndex pickedOpponent;
  float GAP;
  float FIRST_GAP;
  float robotOffset;
  int side;

};

#endif //DOUBLE_PICK_BLOCK_SKILL_H