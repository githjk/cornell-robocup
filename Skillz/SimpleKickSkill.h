#ifndef SIMPLE_KICK_SKILL_H
#define SIMPLE_KICK_SKILL_H

#include "skill.h"
class SimpleKickSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  SimpleKickSkill(SystemParameters *sp,
                  VisionModule *vm, 
                  RoboCupStrategyModule *strat,
                  RobotIndex id, 
                  SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
  void initialize();
  ///always call this one:
  void initialize(KickSpeed k);
  
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
	  return "SimpleKickSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  KickSpeed kickSpeed;

  float rotation;

};

#endif //SIMPLE_KICK_SKILL_H