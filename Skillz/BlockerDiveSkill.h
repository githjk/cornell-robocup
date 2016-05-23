#ifndef BLOCKER_DIVE_SKILL_H
#define BLOCKER_DIVE_SKILL_H

#include "Skill.h"

class BlockerDiveSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  BlockerDiveSkill(SystemParameters *sp,
                   VisionModule *vm, 
                   RoboCupStrategyModule *strat,
                   RobotIndex id, 
                   SkillSet* set);

  /**returns true if the robot has possession of the ball*/
  bool isValid();

  ///resets timer
  void initialize(float lineX);
  ///resets timer
  void initialize();

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();

  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();

  ///Check if we've gotten to where we're supposed to be (location and rotation)
  bool isFinished(float tolerance=.01f);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "BlockerDiveSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  bool diveLeft;
};

#endif //BLOCKER_DIVE_SKILL_H
