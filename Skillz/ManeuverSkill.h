#ifndef MANEUVER_SKILL_H
#define MANEUVER_SKILL_H

#include "Skill.h"

class ManeuverWithBallSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  ManeuverWithBallSkill(SystemParameters *sp,                
                        VisionModule *vm, 
                        RoboCupStrategyModule *strat,
                        RobotIndex id, 
                        SkillSet* set);

  /**returns true if the robot has possession of the ball*/
  bool isValid();

  ///resets timer
  void initialize();

  ///tells the skill which direction it should face, and resets the timer.
  void initialize(Pair dest, Pair face);

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
	  return "ManeuverWithBallSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  /**@name    protected members  */
  //@{

  /// Point that the robot should try to go
  Pair destPoint;
  /// Where the robot should try to face
  Pair facePoint;
  
  //Timer timer;
  
  /// Parameters
  float BALL_DRIBBLE_ANGLE;
  float ROT_RATE;
  float OFFSET_RATIO;
  float CLOSENESS_THRESHOLD;



  //@}

};

#endif //MANEUVER_SKILL_H
