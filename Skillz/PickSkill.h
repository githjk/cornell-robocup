#ifndef PICK_SKILL_H
#define PICK_SKILL_H

#include "skill.h"
#include "common/datatypes/base_types.h"

class PickSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  PickSkill(SystemParameters *sp, 
            VisionModule *vm, 
            RoboCupStrategyModule *strat,	  
            RobotIndex id, 
            SkillSet* set);

  /**Call this function to check the prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///Perform initializations for the skill.
  void initialize();
  void initialize(RobotIndex handlerRobot, RobotIndex opponentRobot,
                  int handlerTeam, int opponentTeam, 
                  Pair handlerDest, bool goToCenter = false);

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();
  
  void update();
  
  void setOpponent(RobotIndex opponentRobot);
    
  Pair getPickLoc();

  ///determine if we're finished yet
  bool isFinished(float tolerance);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const {
	  return "PickSkill"; 
  }
  //}@

protected:
  /**Execute the skill.*/
  void execute();

  // Angle from robot we pick to the picking position.
  float pickAngle;
  //A temp variable
  float theta;
  // Dist to pick opponent from
  float pickDist;
  // Robot we need to pick
  RobotIndex opponent;
  int opponentTeam;
  // Friend who has the ball
  RobotIndex handler;
  int handlerTeam;
  
  //Where the handler is going
  Pair dest;
  //If we want to move towards the middle of the field...
  bool goCenter;
  
  //Their locations
  Pair handlerLoc;
  Pair opponentLoc;
  Pair pickerLoc;
  
  // To make sure we initialized correctly
  bool handlerInitialized;
  bool enemyInitialized;
  // Locations to pick (i,e. where we should "stand" while picking)
  Pair pickLoc, pickLoc1, pickLoc2;
  // Distances from pick locations 1 and 2 to the line between handler and dest
  float pickLocDist1, pickLocDist2;
  // Temporary variables
  float x,y;

  float BUFFER_DIST;
  float sidelineLaneWidth;

};

#endif //PICK_SKILL_H
