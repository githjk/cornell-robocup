#ifndef MOVE_AROUND_PICK_SKILL_H
#define MOVE_AROUND_PICK_SKILL_H

#include "skill.h"
#include "common/datatypes/base_types.h"
#include "Skillz/ManeuverSkill.h"

class MoveAroundPickSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  MoveAroundPickSkill(SystemParameters *sp,
                      VisionModule *vm, 
                      RoboCupStrategyModule *strat,
                      RobotIndex id, 
                      SkillSet* set);

  ~MoveAroundPickSkill();

  /**Call this function to check the prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///Perform initializations for the skill.
  void initialize();
  void initialize(RobotIndex pickerRobot, RobotIndex opponentRobot,
                  int pickerTeamNum, int opponentTeamNum, Pair handlerDest);

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
	  return "MoveAroundPickSkill"; 
  }
  //}@

protected:
  /**Execute the skill.*/
  void execute();

  //
  float sideLineLaneWidth;
  ManeuverWithBallSkill* maneuverSkill;
  
  // Opponent robot being picked
  RobotIndex opponent;
  int opponentTeam;
  // Friend who is picking opponent
  RobotIndex picker;
  int pickerTeam;
  
  Pair handlerLoc;      //Our location
  Pair pickerLoc;       //Picker Robot's location
  Pair opponentLoc;     //Opponent's location
  Pair dest;            //Where we want to go

  //Temporary variables
  Pair tempDest;
  float theta;
};

#endif //MOVE_AROUND_PICK_SKILL_H
