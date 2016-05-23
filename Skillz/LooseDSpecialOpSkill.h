#ifndef LOOSE_D_SPEC_OP_SKILL_H
#define LOOSE_D_SPEC_OP_SKILL_H

#include "skill.h"
#include "common/datatypes/base_types.h"

class LooseDSpecialOpSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  LooseDSpecialOpSkill(SystemParameters *sp,
                       VisionModule *vm, 
                       RoboCupStrategyModule *strat,
                       RobotIndex id, 
                       SkillSet* set);

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
	  return "LooseDSpecialOpSkill"; 
  }
  //}@

protected:
  /**Execute the skill.*/
  void execute();

  ///Finds the opponents that we must cover, returns the number (0-2) found
  int findOpponents(RobotIndex &op1, RobotIndex &op2);
  
  
  ///Opponents we must cover
  RobotIndex op1, op2;
  Pair op1Loc, op2Loc, ballLoc, op1Shot, op2Shot;
    
  ///The number of opponents we need to cover.  Ranges from 0 to 2
  int numOpps;
  ///if op2 has a shot
  bool op2HasShot;
  //The right and left y-values of op2's shot on goal.
  //If rightShot >= leftShot, then op2 doesn't have a good shot
  float rightShot, leftShot;

  //Arrays used to find opponents.  Used in findOpponents()
  bool ignore[NUM_ROBOTS];
  //Other data used in findOpponents()
  float ballX, ballY, oppX, oppY, minX, minBall, distToBall;
  //RobotIndex firstOpp;
  int oppsLeft;

  //Dist behind ball an opponent must be to be ignored (so it can't 
  //be op1 or op2)
  float GRACE;
  //Offsets from the goal line and sideline that define the default 
  //position to go to when not covering any opponent
  float GOAL_LINE_OFFSET, SIDELINE_OFFSET;
  float SWITCH_THRESH;
  
  //Side to go to.
  typedef enum {
    RIGHT,
    LEFT
  } Side;

  Side side;

};

#endif //LOOSE_D_SPEC_OP_SKILL_H
