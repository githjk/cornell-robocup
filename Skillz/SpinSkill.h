#ifndef SPIN_SKILL_H
#define SPIN_SKILL_H

#include "Skill.h"

class SpinAroundBallSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  SpinAroundBallSkill(SystemParameters *sp,                  
                      VisionModule *vm, 
                      RoboCupStrategyModule *strat,
                      RobotIndex id, 
                      SkillSet* set);

  /**returns true if the robot has possession of the ball*/
  bool isValid();

  ///resets timer
  void initialize();

  ///tells the skill which direction it should face, and resets the timer.
  void initialize(Pair destPoint);

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();

  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();
  
  ///checks if we're facing the right direction
  bool isFinished(float tolerance=.01f);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "SpinSkill"; 
  }
  //}@

protected:
  /**@name    protected members  */
  //@{

  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  /// Point that the robot should try to face
  Pair destPoint;
  /// Where the ball is
  Pair ballLoc;
  /// Point we pivot around
  Pair pivot;
  /// Where we're rotating to
  Pair rotDest;
  /// Our current angle
  float curAngle;

  //PARAMETERS:

  ///adjustment to radius we spin around the ball
  float RADIUS_OFFSET;
  ///rotation rate multiplier
  float ROTATION_GAIN;
  float ROTATION_BIAS;
  ///threshold at which we swith to traj gen
  float SPIN_AROUND_BALL_THRESHHOLD;
  ///angle offset from facing the pivot point
  float ANGLE_OFFSET;
  ///amount to adjust radius for location of ball on dribbler
  float RADIUS_ADJUST_FACTOR;
  ///how much to push on the ball
  float PUSH_FACTOR;

  ///Whether to use RotateAroundBall instead
  int USE_ROTATE;

  //@}

};

#endif //SPIN_SKILL_H
