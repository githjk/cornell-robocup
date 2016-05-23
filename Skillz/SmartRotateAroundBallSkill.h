#ifndef SMART_ROTATE_SKILL_H
#define SMART_ROTATE_SKILL_H

#include "Skill.h"

class SmartRotateAroundBallSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  SmartRotateAroundBallSkill(SystemParameters *sp,
                        VisionModule *vm, 
                        RoboCupStrategyModule *strat,
                        RobotIndex id, 
                        SkillSet* set);

  /**returns true if the robot has possession of the ball*/
  bool isValid();

  //does nothing
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
  bool isFinished(float tolerance=-1.0f);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "SmartRotateAroundBallSkill"; 
  }
  //}@

protected:
  /**@name    protected members  */
  //@{

  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  //returns true if b is between a1 and a2
  bool angleIsBetween(float a1, float a2, float b);

  //pivot point
  Pair pivotPoint;

  // Point that the robot should try to face
  Pair ultimateDestination;

  //Next destination we'll give the robot
  Pair rotatedDestination;

  //ultimate angle
  float ultimateAngle;

  //next angle robot sent to
  float nextAngle;

  //wher robot is
  Pair robotLoc;
  float robotAngle;

  float destRot;

  float angleDiff;

  bool finished;

  //PARAMETERS:
  float ROTATION_STEP;
  float FINISHED_ROTATION_THRESHOLD;
  float PUSH_FACTOR;
  int USE_BALL_FOR_PIVOT;
  float SAFE_LINE_HALF_LINE_OFFSET;
  //@}

};

#endif //SMART_ROTATE_SKILL_H
