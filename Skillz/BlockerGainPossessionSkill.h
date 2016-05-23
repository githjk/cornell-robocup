#ifndef BLOCKER_GAIN_POSSESSION_SKILL_H
#define BLOCKER_GAIN_POSSESSION_SKILL_H

#include "skill.h"
class BlockerGainPossessionSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  BlockerGainPossessionSkill(SystemParameters *sp,
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

  //Checks if robot backing up
  bool isBackingUp();

  ///Returns the skill's name
  char* getName() const { 
	  return "BlockerGainPossessionSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();


  float BACKUP_DIST;
  float BACKUP_SPEED;
  float BALL_CUSHION;
  float DRIBBLE_DIST;
  float SPIN_ANGLE;
  float BEHIND_DIST;
  float FACING_BALL_ANGLE;
  float POS_FRAMES;
  float ball_cushion_dist;

  //some state
  bool backingUp;
  bool finished;
  bool ball_behind;
  int frames;

};

#endif //BLOCKER_GAIN_POSSESSION_SKILL_H