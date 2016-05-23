#ifndef FACILATE_PASS_SKILL_H
#define FACILATE_PASS_SKILL_H

#include "Skill.h"

class FacilatePassSkill : public Skill  //okay, it should be spelled FACILITATE, but I don't want to change it everywhere in the code.
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  FacilatePassSkill(SystemParameters *sp,
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
	  return "FacilatePassSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  float OFFSET;
  float CONE_SIZE;
  float OFFSET_Y;



};

#endif //FACILATE_PASS_SKILL_H
