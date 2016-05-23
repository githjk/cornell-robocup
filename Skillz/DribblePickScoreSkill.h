#ifndef DRIBBLE_PICK_SCORE_SKILL_H
#define DRIBBLE_PICK_SCORE_SKILL_H

#include "skill.h"
class DribblePickScoreSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  DribblePickScoreSkill(SystemParameters *sp,
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
	  return "DribblePickScoreSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  float PICK_BUFFER;
  float TURN_HAPPY_ANGLE;
  float PAST_DIST;
  float FORWARD_DIST;
  float CLOSE_DIST;

  float state;
  int direction;
  Pair destPos;


};

#endif //DRIBBLE_PICK_SCORE_SKILL_H