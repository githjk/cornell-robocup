#ifndef PULL_BALL_OFF_CORNER_SKILL_H
#define PULL_BALL_OFF_CORNER_SKILL_H

#include "skill.h"



class PullBallOffCornerSkill : public Skill
{
public:

	static int skillNum;

	/**@name	public functions  */
	//@{

	///Constructor.	Calls the base class's (Skill) constructor.
	PullBallOffCornerSkill(SystemParameters *sp,
                         VisionModule *vm,
                         RoboCupStrategyModule *strat,
                         RobotIndex id, 
                         SkillSet* set);

					  
	///Destructor: do nothing
	~PullBallOffCornerSkill() {}

	/**The prereqs of this skill are that no one has possession, the ball 
	speed is less than some small threshold, and the ball is very close to
	the wall. */
	bool isValid();

	///Perform any initializations for the skill. 
	void initialize();

	/**Indicates if the skill is still viable (whether it is frustrated or not).
	True = skill is okay.  False = skill is frustrated.*/
	bool evaluate();
	
	/**For tweaking the skill.  You may want to change local parameters or behave 
	differently to adapt to any situation that is frustrating the skill.*/
	void update();

	/**For checking if ball is on easily accessible portion of side wall.
	This returns true if the ball is not in our corner (> ourCornerThresh away from OUR_GOAL_LINE),
	not in their corner (> theirCornerThresh away from THEIR_GOAL_LINE)
	and ball is adjacent to a side wall (< wallThresh away).*/
	bool ballOnCorner(const VisionData& field);

  ///determine if we're finished yet
  bool isFinished(float tolerance=0.0f);

  ///Loads the parameter values for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "PullBallOffCornerSkill"; 
  }
	//}@
	
protected:
	
	///Execute the skill - get possession, back and turn away from wall.
	void execute();
	
  ///Parameters
	float CORNER_THRESH;

  float NEAR_DIST;
  float FAR_DIST;
  
  float ROTATION_SPEED;

  float FAR_TO_NEAR_LOC_TOL;
  float FAR_TO_NEAR_ROT_TOL;

  float NEAR_TO_FAR_TOL;
  
  Pair ballLoc, robotLoc;
  float robotAngle;

  float endWall,sideWall;
  int end, side;

  Pair nearLoc, farLoc;

  bool spinning;

};

#endif //PULL_BALL_OFF_WALL_SKILL_H
