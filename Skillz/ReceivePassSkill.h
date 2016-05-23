#ifndef RECEIVE_CREASE_PASS_SKILL_H
#define RECEIVE_CREASE_PASS_SKILL_H

#include "skill.h"

class ReceivePassSkill : public Skill
{
public:
  
  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  ReceivePassSkill(SystemParameters *sp,
                   VisionModule *vm, 
                   RoboCupStrategyModule *strat,
                   RobotIndex id, 
                   SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
  void initialize();

  ///Other initialize function
  void initialize(float MIN_DISTANCE, Pair position, bool kicker_on,bool to_extend);

  
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
	  return "ReceivePassSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();


  /// The id of the aggressor
  RobotIndex aggressorID;

  /// The Line on which the creator will be moving
  Line LineOfInterest;

  /// The default minimum distance
  float DEFAULT_MINIMUM_DISTANCE;
  float minDist;

  /// Angle offset for the rotation to be adjusted
  float BIAS;

  float THRESHOLD_SPEED;

  /// The two boundary points
  Pair boundary1,boundary2,extensionBoundary,originalBoundary,intendedPosition;

  /// If an extension is possible
  bool isExtension;

  /// If the kicker should be ON
  bool kicker_on;

  //Whether we were charging last frame.
  bool charging;

  /// The distance from goalie and creator
  float GAP;

  /// Distance from the boundary you will like the robot to be placed
  float OFFSET_BOUNDARY;

  ///allowable angle for splitangle
  float VALID_KICK_ANGLE;

  float KICK_RATIO;

  //when we want to charge the ball
  //(ball's velocity must pass within this much of the player's radius for us to charge it)
  float CHARGE_RADIUS;
  float MIN_CHARGE_DIST;


  //point to go to
  Pair destPos;

};

#endif //RECEIVE_CREASE_PASS_SKILL_H
