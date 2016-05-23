#ifndef TIGHT_DEFENSE_DEFENDER_SKILL_H
#define TIGHT_DEFENSE_DEFENDER_SKILL_H

#include "skill.h"
#include "ShotBlockSkill.h"

class TightDefenseDefenderSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  TightDefenseDefenderSkill(SystemParameters *sp,
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
	  return "TightDefenseDefenderSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();


  //The defender's ID

  RobotIndex specialOpDefenderID;

  ShotBlockSkill *shotBlock;

  float FACE_ANGLE;
  float KICK_DIST;
  float KICK_ANGLE;
  float MORE_OFFSET;

};

#endif //TIGHT_DEFENSE_DEFENDER_SKILL_H