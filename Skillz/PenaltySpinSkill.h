#ifndef PENALTY_SPIN_SKILL_H
#define PENALTY_SPIN_SKILL_H

#include "skill.h"
class PenaltySpinSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  PenaltySpinSkill(SystemParameters *sp,
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
	  return "PenaltySpinSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  int direction;

  float theta;
  float thetaVel;

  float ACCEL;
  float THETA_THRESH;
  float MAX_ROT_VEL;
  int PENALTY_SPIN_DIRECTION;

  bool finished;

};

#endif //PENALTY_SPIN_SKILL_H