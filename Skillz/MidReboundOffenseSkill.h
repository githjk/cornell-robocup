#ifndef MID_REBOUND_OFFENSIVE_SKILL_H
#define MID_REBOUND_OFFENSIVE_SKILL_H

#include "skill.h"
#include "SimpleScreenSkill.h"

class MidReboundOffenseSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  MidReboundOffenseSkill(SystemParameters *sp,
                         VisionModule *vm, 
                         RoboCupStrategyModule *strat,
                         RobotIndex id,                    
                         SkillSet* set);


  //Destructor deletes simpleScreenSkill
  ~MidReboundOffenseSkill();


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
	  return "MidReboundOffenseSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();
  
  RobotIndex getOpponent();

  RobotIndex aggressor, creator, opponent;

  Pair aggLoc, creatorLoc;

  float distance;

  float UPPER_LIMIT, LOWER_LIMIT, RIGHT_LIMIT, LEFT_LIMIT, KICK_RATIO;

  SimpleScreenSkill *simpleScreenSkill;
};

#endif //MID_REBOUND_OFFENSIVE_SKILL_H