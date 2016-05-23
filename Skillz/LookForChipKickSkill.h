#ifndef LOOK_FOR_CHIP_KICK_SKILL_H
#define LOOK_FOR_CHIP_KICK_SKILL_H

#include "skill.h"
class LookForChipKickSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  LookForChipKickSkill(SystemParameters *sp,
                       VisionModule *vm, 
                       RoboCupStrategyModule *strat,
                       RobotIndex id, 
                       SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
 void initialize(bool sameSide);
  
  //base intialize method cannot be used
 void initialize() { };
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
	  return "LookForChipKickSkill"; 
  }

  //overloaded execute method to take in side bias
  void execute();
  
  //}@
protected:
  bool sameSide;
};

#endif //LOOK_FOR_CHIP_KICK_SKILL_H