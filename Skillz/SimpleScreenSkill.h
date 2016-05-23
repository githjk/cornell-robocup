#ifndef SIMPLE_SCREEN_SKILL_H
#define SIMPLE_SCREEN_SKILL_H

#include "skill.h"

class SimpleScreenSkill : public Skill
{
public:
  
  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  SimpleScreenSkill(SystemParameters *sp,
                    VisionModule *vm, 
                    RoboCupStrategyModule *strat,
                    RobotIndex id, 
					SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
  void initialize() { }

  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize(int NpointA_Type,
                  int NpointA_ID, 
                  int NpointB_Type,
                  int NpointB_ID,
                  int NpointC_Type,
                  int NpointC_ID);

  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize(int NpointA_Type,
                  int NpointA_ID, 
                  int NpointB_Type,
                  int NpointB_ID,
                  int NpointC_Type,
                  int NpointC_ID,
                  float NgraceDistance);

  
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
	  return "SimpleScreenSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();


  void grabPair(int pairType, int pairID, Pair* result);

  int pointA_Type, 
      pointA_ID, 
      pointB_Type, 
      pointB_ID, 
      pointC_Type, 
      pointC_ID;

  float graceDistance;

  Pair pointA, pointB, pointC;

  //these are the point "types"  
  #define SCREEN_FRIENDLY_ROBOT 0
  #define SCREEN_OPPONENT_ROBOT 1
  #define SCREEN_BALL 2

  //used when calculating destination
  float xDiff,yDiff;
  float dist;
  float percentage;
};

#endif //SIMPLE_SCREEN_SKILL_H
