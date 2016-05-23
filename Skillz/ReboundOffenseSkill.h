#ifndef REBOUND_OFFENSE_SKILL_H
#define REBOUND_OFFENSE_SKILL_H

#include "skill.h"
class ReboundOffenseSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  ReboundOffenseSkill(SystemParameters *sp,
                      VisionModule *vm, 
                      RoboCupStrategyModule *strat,
                      RobotIndex id, 
                      SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
 void initialize();
  
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
	  return "ReboundOffenseSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();
  
  float KICK_RATIO;

};

#endif //REBOUND_OFFENSE_SKILL_H