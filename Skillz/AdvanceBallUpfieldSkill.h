#ifndef ADVANCE_BALL_UPFIELD_SKILL_H
#define ADVANCE_BALL_UPFIELD_SKILL_H

#include "skill.h"
class AdvanceBallUpfieldSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  AdvanceBallUpfieldSkill(SystemParameters *sp,
                          VisionModule *vm, 
                          RoboCupStrategyModule *strat,
                          RobotIndex id, 
                          SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
 void initialize();
  
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
	  return "AdvanceBallUpfieldSkill"; 
  }
  ///Returns whether or not we're passing
  bool isPassing();

  //}@
protected:
  float AdvanceBallUpfieldSkill::threatDist();

/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  ///params:
  
  ///How far to stay away from the wall when choosing destinations.
  float WALL_BUFFER;
  ///How close an opponent can get before they are considered a threat (and within what angle)
  float THREAT_DIST;
  float THREAT_ANGLE;
  ///How close an opponent must be to be considered blocking us
  float BLOCKING_DIST;
  ///Destination point:
  float DEST_X;
  float DEST_Y;
  float HYST_DIST;
  ///angle for turning with spin
  float BACKWARDS_ANGLE;
  float BACKWARDS_ANGLE_THREATENED;
  float faceAngle;
  ///Distance when we consider ourselves "there"
  float THERE_DIST;

  //hysteresis:
  Pair destination;


  //booleans to keep track of what we are currently doing
  bool juking;
  bool passing;
  bool pick;

  bool starting;
  bool pulling;
  bool spinning;


};

#endif //ADVANCE_BALL_UPFIELD_SKILL_H