#ifndef REMOVE_BALL_SETUP_SKILL_H
#define REMOVE_BALL_SETUP_SKILL_H

#include "skill.h"



class RemoveBallSetupSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  RemoveBallSetupSkill(SystemParameters *sp,
	                   VisionModule *vm, 
					   RoboCupStrategyModule *strat,
					   RobotIndex id, 
					   SkillSet* set);

  /**Call this function to check the prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize();

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();

  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();

  ///tell whether the skill is finished yet.
  bool isFinished(float tolerance);

  ///Loads parameter values from a text file
  void loadValues();
  
  ///Returns the skill's name
  char* getName() const { 
	  return "RemoveBallSetupSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();


  //Side to go to.
  typedef enum {
    RIGHT,
    LEFT
  } Side;

  Side side;
  
  float OFFSET_GOAL_LINE;
  float OFFSET_GOAL_WALL;
  float FACE_ANGLE;
  float ANGLE_DIFF;
  float SWITCH_THRESH;
  float BIAS;
  float CLOSE_DIST;
  float KICK_RATIO;

  

};

#endif //REMOVE_BALL_SETUP_SKILL_H
