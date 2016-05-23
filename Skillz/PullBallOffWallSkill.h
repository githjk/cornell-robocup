#ifndef PULL_BALL_OFF_WALL_SKILL_H
#define PULL_BALL_OFF_WALL_SKILL_H

#include "skill.h"
#include "PullBallOffCornerSkill.h"



class PullBallOffWallSkill : public Skill
{
public:

	static int skillNum;

	/**@name	public functions  */
	//@{

	///Constructor.	Calls the base class's (Skill) constructor.
	PullBallOffWallSkill(SystemParameters *sp,
                       VisionModule *vm,
                       RoboCupStrategyModule *strat,
                       RobotIndex id, 
                       SkillSet* set);

					  
	///Destructor: do nothing
	~PullBallOffWallSkill() {}

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
	bool ballOnSideWall(const VisionData& field);

	/**This checks to see if I have possession of the ball and it is > successWallThresh away
	from a wall, and the ball is > successOurCornerThresh away from our goal line, and
	is > successTheirCornerThresh away from their goal line.*/
	bool ballPulledOffWall(const VisionData& field);

  ///determine if we're finished yet
  bool isFinished(float tolerance=0.0f);

  ///Loads the parameter values for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const 
  { 
	  return "PullBallOffWallSkill"; 
  }

  void unInitialize();
  
	
protected:
	
	///Execute the skill - get possession, back and turn away from wall.
	void execute();
	
	///store some state
	bool haveBall;
	///Counter of how many times we successfully get possession
	int successes;
	///Counter of failures (bumping the ball away)
	int failures;

  //Skill handle to pull ball off the corner
  PullBallOffCornerSkill *pullCorner;
	
  ///Parameters
	float WALL_THRESH;
	float SUCCESS_WALL_THRESH;
	float BALL_SPEED_THRESH;
	float DRIBBLE_DIST;

  float CORNER_SPEED_DIST;
  float FAR_DIST;
  float NEAR_DIST ;
  float FACING_ANGLE;
  float PUSH_THROUGH_DIST;
  float SIDE_OFFSET;
  int POS_FRAMES;
  float MAX_KICK_ANGLE;

  float frames;

  bool goingBehind;
  Pair behindLoc;
  bool wallsInWay;

};

#endif //PULL_BALL_OFF_WALL_SKILL_H
