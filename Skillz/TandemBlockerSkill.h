#ifndef TANDEM_BLOCKER_SKILL_H
#define TANDEM_BLOCKER_SKILL_H



#include "skill.h"
#include "CutGoalSkill.h"
class TandemBlockerSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
TandemBlockerSkill(SystemParameters *sp,
                   VisionModule *vm, 
                   RoboCupStrategyModule *strat,
                   RobotIndex id, 
                   SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
 void initialize() ;
 void initialize(Pair ballPos) ;
  
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
	  return "TandemBlockerSkill"; 
  }
  //}@

protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();


  //The defender's ID
  RobotIndex defenderID;
  Pair ball;
  // The front line you do not want the robot to cross	
  float UPPER_X ;
  // the back line you do not want the robot to cross
  float LOWER_X ;

 // The threshold X;
 float LOW_X;

 //Diving:
 ///The offset line we should dive along
 float DIVE_OFFSET;
 ///speed the ball should be going.
 float DIVE_SPEED;
 float GOAL_POST_TOLERANCE;

 bool presetBall;
 CutGoalSkill *cutGoal;

};


#endif //TANDEM_BLOCKER_SKILL_H