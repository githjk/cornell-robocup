#ifndef CROSS_CREASE_PASS_SKILL_H
#define CROSS_CREASE_PASS_SKILL_H

#include "skill.h"
#include "robocup_strategy_data.h"// for Position enum

class CrossCreasePassSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  CrossCreasePassSkill(SystemParameters *sp,
                       VisionModule *vm, 
                       RoboCupStrategyModule *strat,
                       RobotIndex id, 
                       SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
 void initialize();
  ///always call the more specific init function below
 void initialize(Position receiverPosition);
  
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
	  return "CrossCreasePassSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  ///position offset from goalie box corner
  float POS_OFFSET_X;
  float POS_OFFSET_Y;

  ///maximum time we wait before kicking.
  float TIME_OUT;

  //angle that we're happy with
  float HAPPY_ANGLE;

  int DELAY_FRAMES;

  int delay;

  ///state about what we're doing
  RobotIndex creatorID;
  bool kicking; //for timing out or defaulting out of TurnAndKick
  Position receiver;

};

#endif //CROSS_CREASE_PASS_SKILL_H