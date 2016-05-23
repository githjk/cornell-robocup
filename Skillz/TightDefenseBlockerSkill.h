#ifndef TIGHT_DEFENSE_BLOCKER_SKILL_H
#define TIGHT_DEFENSE_BLOCKER_SKILL_H

#include "skill.h"
//#include "MoveToBallSkill.h"
#include "BlockerGainPossessionSkill.h"

class TightDefenseBlockerSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  TightDefenseBlockerSkill(SystemParameters *sp,
                           VisionModule *vm, 
                           RoboCupStrategyModule *strat,
                           RobotIndex id, 
                           SkillSet* set);

  ~TightDefenseBlockerSkill() {
    //delete moveSkill;
    //moveSkill = NULL;
    delete gainPossessionSkill;
    gainPossessionSkill = NULL;    
  }

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
	  return "TightDefenseBlockerSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();
  
  //Finds the robot that has possession, if any.  
  //Sets the variables robotWithBall and teamWithBall
  void getRobotWithBall();
  
  //Tests to see if ball is in goal box
  bool ballInGoalBox();



  Pair currentLoc, ballLoc, opponentLoc;
  RobotIndex robotWithBall;
  int teamWithBall;

  //MoveToBallSkill *moveSkill;
  BlockerGainPossessionSkill *gainPossessionSkill;

  float SCREEN_DIST, GOAL_LINE_OFFSET;
  float FACE_ANGLE;
  float KICK_DIST;
  float KICK_ANGLE;
  //distance at which we will go after the ball and kick it
  float CLOSE_DIST;
  //threshold for deciding if defender is out of position
  float DEFENDER_DIST;

  float MAX_BALL_SPEED;


};

#endif //TIGHT_DEFENSE_BLOCKER_SKILL_H