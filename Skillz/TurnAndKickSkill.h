#ifndef TURN_AND_KICK_H
#define TURN_AND_KICK_H

#include "Skill.h"
#include "SpinSkill.h"
#include "ManeuverSkill.h"

class TurnAndKickSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  TurnAndKickSkill(SystemParameters *sp,
                   VisionModule *vm, 
                   RoboCupStrategyModule *strat,
                   RobotIndex id, 
                   SkillSet* set);

  /**returns true if the robot has possession of the ball*/
  bool isValid();

  ///resets timer
  void initialize();

  ///tells the skill which Point to aim for.
  void initialize(Pair face,KickSpeed k,bool precise=true);

  ///tells the skill to aim for a robot.
  void initialize(RobotIndex robot,KickSpeed k,bool precise=true);

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();
  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();

  ///Check if we've kicked
  bool isFinished(float tolerance=.01f);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "TurnAndKickSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  /**Execute the kick functionality when pointed in the right direction*/
  void doKick(Pair targetPoint);


  /**Conatins common initializations*/
  void initialize(KickSpeed k,bool precise);

  /**@name    protected members  */
  //@{
  ///Skills to use
  SpinAroundBallSkill* spinSkill;
  ManeuverWithBallSkill* maneuverSkill;

  ///Kicking speed
  KickSpeed kickSpeed;

  ///whether we should be precise or fast
  bool preciseKick;

  ///whether we've kicked or not
  bool kicked;
  bool wasMoving;

  /// Where the robot should try to face
  RobotIndex faceRobot;
  bool isRobot;
  Pair targetPoint;


  //Timer timer;

  ///Updating variables
  float updateVal;
  bool updateComputed;
  Pair kickLoc;
  
  /// Parameters
  float UPDATE_DISTANCE;
  float STILL_KICK_THRESHHOLD;
  float MOVING_KICK_THRESHHOLD;
  float SPEED_THRESHHOLD;
  float UPDATE_TIMEOUT;
  float IGNORE_ANGLE;
  float PRECISION_ANGLE;
  float ENTERBOX_CAUTION_DIST;



  //@}

};

#endif //TURN_AND_KICK_H
