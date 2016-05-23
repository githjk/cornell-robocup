#ifndef INTERCEPT_BALL_SKILL_H
#define INTERCEPT_BALL_SKILL_H

#include "Skill.h"

class InterceptBallSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  InterceptBallSkill(SystemParameters *sp,    
                     VisionModule *vm, 
                     RoboCupStrategyModule *strat,
                     RobotIndex id, 
                     SkillSet* set);

  /**returns true if the robot has possession of the ball*/
  bool isValid();

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
	  return "InterceptBallSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  /**@name    protected members  */
  //@{

  /// Point that the robot should try to go
  Destination destPoint;

  /// flag of whether or not we're holding position for the ball to reach us.
  bool hold;
  
  //Timer timer;
  
  /// Parameters
  float BALL_SPEED_THRESHOLD;
  float BALL_SPEED_THRESHOLD_HISTORISIS_FACTOR;
  int PRECISION;
  float LEAD_TIME;
  float DRIBBLE_DISTANCE;
  float CATCH_RADIUS;
  float INNER_CATCH;
  float FLEEING_ANGLE;
  float BIAS_ANGLE;

  bool ballFast;
  //@}

};

#endif //INTERCEPT_BALL_SKILL_H
