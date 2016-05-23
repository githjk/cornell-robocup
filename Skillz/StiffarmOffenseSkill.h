#ifndef STIFFARM_OFFENSE_SKILL_H
#define STIFFARM_OFFENSE_SKILL_H

#include "skill.h"
#include "SimpleScreenSkill.h"
#include "common/util/math/dist.h"

class StiffarmOffenseSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  StiffarmOffenseSkill(SystemParameters *sp,
                       VisionModule *vm, 
                       RoboCupStrategyModule *strat,
                       RobotIndex id, 
                       SkillSet* set);

  //Destructor
  ~StiffarmOffenseSkill();

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
  void initialize();
  ///use this one to call stiffarm on a different position
  void initialize(Position position);
  
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
	  return "StiffarmOffenseSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  Position friendPosition;

  //Gets opponent ID to screen
  RobotIndex getOpponent();
  //Gets teammate closest to ball
  RobotIndex getTeammateClosestToBall();

  RobotIndex opponent, aggressor;

  //We will use the simpleScreen skill to screen the opponent
  SimpleScreenSkill *simpleScreenSkill;
  
  Pair aggLoc;

  //As a default position, we set out position based on the aggressor position.
  //These are offsets we use
  float DEF_X_OFFSET, DEF_Y_OFFSET;
  //Used for switching the y-offset of default position in historesis
  float SWITCH_THRESH;
  float MAX_SCREEN_DISTANCE, MIN_SCREEN_DISTANCE, MIN_MAX_THRESH;

  float PUSH_FOWARD_DISTANCE;
  //Side to go to.
  typedef enum {
    RIGHT,
    LEFT
  } Side;

  Side side;

  //Opponent was screening last frame, used to
  //have historisis on opponent robots
  RobotIndex lastOpponent;
};

#endif //STIFFARM_OFFENSE_SKILL_H