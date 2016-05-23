#ifndef MOVE_TO_BALL_SKILL_H
#define MOVE_TO_BALL_SKILL_H

#include "skill.h"
#include "common/vision/visionTimer.h"



class MoveToBallSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  MoveToBallSkill(SystemParameters *sp,
                  VisionModule *vm, 
                  RoboCupStrategyModule *strat,
                  RobotIndex id,
                  SkillSet* set);
  
  ///Destructor: deletes SkillTimer object
  ~MoveToBallSkill() {delete possessionTimer;}

  /**The prereqs of this skill are that no one has possession and that the ball 
  speed is less than some threshold. */
  bool isValid();

  ///Perform any initializations for the skill. Also takes in a bool to see if 
  ///we want to rush the ball (e.g. if the ball is between us and the goal).
  void initialize(bool rushTheBall);
  void initialize();

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();

  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();
  
  ///determine if we're finished yet
  bool isFinished(float tolerance);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "MoveToBallSkill"; 
  }
  //}@

protected:
  ///Execute the skill.  Go get the ball!
  void execute();

  //Threshold angle for approaching ball.  If angle to ball is larger than this, slow down.
  float APPROACH_ANGLE;  

  ///Threshold on how long we should have possession of the ball to count as a success
  float SUCCESS_TIME; 
    
  ///The distance we need to be from ball to start slowing down
  float APPROACH_DIST; 
  
  ///Allows us to change the dist bn picker and ball
  float RADIUS_OFFSET; 

  ///distance from us that the goalie has to be for us to enter the goalie box.
  float ENTERBOX_CAUTION_DIST;

  //distance within we force robot to first rotate
  float FORCE_ROTATE;

  ///amount we are headed to the side of the goal that it's judged safe to swoop
  float GOAL_BUFFER;

  ///swooping angle
  float ADJUST_ANGLE;

  ///angle to creator's pass dest needed to arm kicker
  float PASS_ANGLE;

  ///middle of goal
  float MIDDLE_DIST;
  
  //min distance goalie must be from ball before we can get it
  float MIN_GOALIE_DISTANCE;

  ///Indicates if we are rushing the ball
  bool rushBall;  

  ///Timer to determine if we have possession.
  VisionTimer *possessionTimer;

};

#endif //MOVE_TO_BALL_SKILL_H
