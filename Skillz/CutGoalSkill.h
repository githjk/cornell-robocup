#ifndef CUT_GOAL_SKILL_H
#define CUT_GOAL_SKILL_H

#include "skill.h"

class CutGoalSkill : public Skill
{
public:
 static int skillNum;

/**@name    public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  CutGoalSkill(SystemParameters *sp,
               VisionModule *vm, 
               RoboCupStrategyModule *strat,
               RobotIndex id, 
               SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
  void initialize() { }

  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize(float firstPoint, float secondPoint, float UPPER_LIMIT, float LOWER_LIMIT,Pair ballPos);
  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize(float firstPoint, float secondPoint, float UPPER_LIMIT, float LOWER_LIMIT);

  
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
	  return "CutGoalSkill"; 
  }
  //}@

protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();


  //y1 point in our algorithm
  //Pair OneGoalPoint ;
  float point1;
  //y2 point in our algorithm
  //Pair SecondGoalPoint;
  float point2;
  // The front line you do not want the robot to cross	
  float UPPER_X ;
  // the back line you do not want the robot to cross
  float LOWER_X ;

  bool presetBall;
  Pair ball;

};


#endif //CUT_GOAL_SKILL_H