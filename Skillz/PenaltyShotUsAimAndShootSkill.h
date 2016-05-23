#ifndef PENALTY_SHOT_US_AIM_AND_SHOOT_SKILL_H
#define PENALTY_SHOT_US_AIM_AND_SHOOT_SKILL_H

#include "Skill.h"

class PenaltyShotUsAimAndShootSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  PenaltyShotUsAimAndShootSkill(SystemParameters *sp,
                                VisionModule *vm, 
                                RoboCupStrategyModule *strat,
                                RobotIndex id, 
                                SkillSet* set);

  /**returns true if the robot has possession of the ball*/
  bool isValid();

  ///resets timer
  void initialize();

  ///tells the skill which Point to aim for.
  void initialize(Pair face,KickSpeed k,bool precise=true);

  ///tells the skill to aim for a robot.
  void initialize(RobotIndex robot,KickSpeed k,bool precise=true);

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();
  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();

  ///Check if we've kicked
  bool isFinished(float tolerance=.01f);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "PenaltyShotUsAimAndShootSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  bool kicked;

  Pair robotLoc;
  Pair opponentGoalie;
  bool goalieFound;

  float yLeft, yRight;
  float yTarget;
  bool aimLeft;

  float TIMEOUT_AND_STOP_CHOOSING_SIDES;
  float TIMEOUT_AND_KICK;
  float POST_OFFSET;
  float ANGLE_TOLERANCE;
  float KICK_TIME;
};

#endif //PENALTY_SHOT_US_AIM_AND_SHOOT_SKILL_H
