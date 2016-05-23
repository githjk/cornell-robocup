#ifndef Shot_BLOCK_SKILL_H
#define Shot_BLOCK_SKILL_H

#include "skill.h"



class ShotBlockSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  ShotBlockSkill(SystemParameters *sp,
                 VisionModule *vm, 
                 RoboCupStrategyModule *strat,
                 RobotIndex id, 
                 SkillSet* set);

  /**Call this function to check the prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize();
  void initialize(RobotIndex opponent);

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();

  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();

  ///tell whether the skill is finished yet.
  bool isFinished(float tolerance);

  ///Loads parameter values from a text file
  void loadValues();
  
  ///Returns the skill's name
  char* getName() const { 
	  return "ShotBlockSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  RobotIndex opponentID;

  

  float GRACE_FACTOR;
  
};

#endif //Shot_BLOCK_SKILL_H
