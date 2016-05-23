#ifndef THREE_MAN_BLOCKER_SKILL_H
#define THREE_MAN_BLOCKER_SKILL_H

#include "Skill.h"

class ThreeManBlockerSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  ThreeManBlockerSkill(SystemParameters *sp,
                       VisionModule *vm, 
                       RoboCupStrategyModule *strat,
                       RobotIndex id, 
                       SkillSet* set);

  /**returns true if the robot has possession of the ball*/
  bool isValid();

  ///resets timer
  void initialize(float lineX);
  ///resets timer
  void initialize(float lineX,Pair ballPos);
  ///resets timer
  void initialize();
  ///resets timer
  void initialize(Pair ballPos);

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
	  return "ThreeManBlockerSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  /**@name    protected members  */
  //@{

  ///line we move along (default is goalie box - 1 radius) 
  float line_x;
  float upper_x;
  float LINE_X_OFFSET;
  float DIVE_SPEED;
  float GOAL_POST_TOLERANCE;
  float UPPER_X_OFFSET;
  float DEFENDER_DIST;
  bool presetBall;
  Pair ball;


  //@}

};

#endif //THREE_MAN_BLOCKER_SKILL_H