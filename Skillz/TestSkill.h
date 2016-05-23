#ifndef TEST_SKILL_H
#define TEST_SKILL_H

#include "Skill.h"

class TestSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  TestSkill(SystemParameters *sp,
            VisionModule *vm, 
            RoboCupStrategyModule *strat,
	    	RobotIndex id, 
			SkillSet* set);

  /**Call this function to check the prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize();

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
	  return "TestSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  // Changing the sign of this variable can change spin direction
  float spinCoeff;

  int TEST_PARAM;

};

#endif //TEST_SKILL_H
