#ifndef PASS_TO_CREATOR_SKILL_H
#define PASS_TO_CREATOR_SKILL_H

#include "skill.h"
class PassToCreatorSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  PassToCreatorSkill(SystemParameters *sp,
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
	  return "PassToCreatorSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  ///ID we're passing to
  RobotIndex creatorID;
  ///whether we're kicking or not.
  bool kicking;
  ///whether or not we're still spinning.
  bool spinning;

  ///Params:

  ///lane width required for pass
  float PASS_LANE_FACTOR;

  ///angle to stop using spin and just face
  float HAPPY_ANGLE;



};

#endif //PASS_TO_CREATOR_SKILL_H