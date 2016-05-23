#ifndef TANDEM_DEFENDER_SKILL_H
#define TANDEM_DEFENDER_SKILL_H

#include "skill.h"
#include "CutGoalSkill.h"

class TandemDefenderSkill : public Skill
{
public:
 static int skillNum;

/**@name    public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  TandemDefenderSkill(SystemParameters *sp,
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
  void initialize(Pair ballPos);
  
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
	  return "TandemDefenderSkill"; 
  }
  //}@

protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();



  ///which side of the field we're on
  int sign;
  ///The front line you do not want the robot to cross	
  float UPPER_X ;
  ///the back line you do not want the robot to cross
  float LOWER_X ;
  ///ball speed threshold
  float BALL_SPEED;
  ///abandon skill if ball gets too close
  float LOW_X;
  ///hysteresis amount
  float CHANGE_BUFFER;
  ///Angle to arm kicker at
  float KICK_ANGLE;
  ///Angle to aim the kick at
  float AIM_ANGLE;
  ///distance to chase at
  float KICK_DIST;
  ///line offset to dive along
  float DIVE_OFFSET;
  ///minimum ball speed required to dive
  float DIVE_SPEED;
  float GOAL_POST_TOLERANCE;

  bool presetBall;
  Pair ball;
  bool blocked;

  //skill to actually use
 CutGoalSkill *cutGoal;

};


#endif //TANDEM_DEFENDER_SKILL_H