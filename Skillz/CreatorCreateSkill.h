#ifndef CREATOR_CREATE_SKILL_H
#define CREATOR_CREATE_SKILL_H

#include "skill.h"
class CreatorCreateSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  CreatorCreateSkill(SystemParameters *sp,
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

  ///Function to find pass destination
  void calculatePassDest(Pair startLoc,float rotAngle);
  
  /// Return the robot destination
  Pair CreatorCreateSkill::getRobotDest();

  ///Returns the skill's name
  char* getName() const { 
	  return "CreatorCreateSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  int state;
  float ANGLE_BIAS;
  float GAP;

  /// Place where we want to set the pass 
  Pair passDest;

  /// Place where we want the robot to be
  Pair robotDest;
  
  /// The location of creator at the beginning
  Pair startLoc;
  /// Distance away from the pass
  float PASS_DISTANCE;

  /// How many player radius away from goalie box
  float DISTANCE_FACTOR;

  float KICK_RATIO;


};

#endif //CREATOR_CREATE_SKILL_H