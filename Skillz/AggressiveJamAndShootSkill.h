#ifndef AGGRESSIVE_JAM_AND_SHOOT_SKILL_H
#define AGGRESSIVE_JAM_AND_SHOOT_SKILL_H

#include "skill.h"
class AggressiveJamAndShootSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  AggressiveJamAndShootSkill(SystemParameters *sp,   
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

  bool isFinished(float tolerance);

  ///has kicked
  bool hasKicked();

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "AggressiveJamAndShootSkill"; 
  }

  ///To be used by anyone who wants to know where I'm shooting at.
  Pair getTargetPoint();
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  Pair target;
  
  bool kicked;
  bool gaveup;
  bool aimed;
  int driftdir;

  int kickFrames;
  int PAUSE_FRAMES;
  int PAUSE_RANDOM;

  float WAIT_TIME;
  float AIM_ANGLE;
  ///drifting params
  float DRIFT_DIST;
  float RANDOM_ANGLE;
  float EDGE_ANGLE;
  float AIM_DISTANCE;
  float FINISHED_TIME;
  float BOUNCE_ANGLE;
  float SIDE_DIST;  
  float SIDE_LANE_FACTOR;
  float DRIFT_ADJUST_ANGLE;

  float IDEAL_RADIUS_LOW;
  float IDEAL_RADIUS_HIGH;
  float MAXIMUM_BACKUP;
  float SAMPLE_DIST;
  float DRIFT_LANE_FACTOR;

  float ideal_radius;



  ///angle needed to be valid
  float VALID_ANGLE;
  float ENTERBOX_CAUTION_DIST;

      float upperbound;
      float lowerbound;
      float testline;

  float shoot_threshold;

};

#endif //AGGRESSIVE_JAM_AND_SHOOT_SKILL_H