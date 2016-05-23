#ifndef CAREFUL_STRIP_SKILL_H
#define CAREFUL_STRIP_SKILL_H

#include "skill.h"
#include "common/datatypes/base_types.h"
#include "MoveToBallSkill.h"

class CarefulStripBallSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  CarefulStripBallSkill(SystemParameters *sp,
                 VisionModule *vm, 
                 RoboCupStrategyModule *strat,
                 RobotIndex id, 
                 SkillSet* set);

  ~CarefulStripBallSkill() {}

  /**Call this function to check the prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///Perform initializations for the skill.
  void initialize();
  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();
  
  void update();
  
  ///determine if we're finished yet
  bool isFinished(float tolerance);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const {
	  return "CarefulStripBallSkill"; 
  }
  //}@

  bool ballIsStripped();

protected:
  /**Execute the skill.*/
  void execute();

  typedef enum
  {
    APPROACH = 0,
  	ATTACK,
    TURN_AND_SCOOP,
  };

  bool stripLeft;
  int stripping_step;
  
  float incomingAngle;
  Pair stripPoint;
  Pair stripAvoidPoint;

  float outcomingAngle;
  Pair scoopPoint;
  Pair lastScoopPoint;

  Pair robotLoc;
  float robotAngle;

  RobotIndex opponentID;
  int numRobots;
  int numFriendly;
  Pair opponentLoc;
  Pair tRobotLoc;

  Pair ballLoc;

  bool dangerousRobot;

  bool ballStripped;

  //params
  float LEFT_VS_RIGHT_HISTORISIS;
  float INCOMING_ANGLE_OFFSET;
  float ANGLE_THRESHOLD;
  float APPROACH_TO_ATTACK_THRESHOLD;
  float START_TURNING_DISTANCE;
  float STOP_AVOIDANCE_DISTANCE;
  float STRIPPING_DISTANCE;
  float STOP_SCOOP_DISTANCE;
  float SUCCESS_DISTANCE;
  float SIDE_SHIFT;
  float HEAD_ON_THRESHOLD;
  float HEAD_ON_THRESHOLD_HISTORISIS;
  float WORST_KICK_ANGLE;
};


#endif //CAREFUL_STRIP_SKILL_H
