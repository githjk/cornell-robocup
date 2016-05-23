#ifndef BACK_PASS_SKILL_H
#define BACK_PASS_SKILL_H

#include "skill.h"



class BackPassSkill : public Skill
{
public:

  static int skillNum;

  /**@name    public functions  */
  //@{
  
  ///Constructor.  Calls the base class Skill constructor
  BackPassSkill(SystemParameters *sp,
                          VisionModule *vm, 
                          RoboCupStrategyModule *strat,
                          RobotIndex id, 
                          SkillSet* set);

  /**Call this function to check the prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize();

  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();

  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();

  ///tell whether the skill is finished yet.
  bool isFinished(float tolerance);

  ///Loads parameter values from a text file
  void loadValues();

  ///Calculate the effect of goalie
  float BackPassSkill::calculateGoalieEffect();
  
  ///Returns the skill's name
  char* getName() const { 
	  return "BackPassSkill"; 
  }
  //}@

protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();


  /// The place where we will like the creator to go
  Pair creatorPos;

  /// Offset for the receivePassLine
  float OFFSET;

  /// Distance from their goal line to the right bound
  float MIN_WIDTH;

  /// The bound to angle difference between current robot rotation and the anle we will like it to be.
  float MAXIMUM_ANGLE;

  /// Multiplied to pass threshold to adjust the lane
  float FACTOR;

  /// How much far should their goalie be from creator
  float GOALIE_DISTANCE_THRESHOLD;

  /// How fast the pass boundary moves with respect to the aggressor's X
  float BOUND_FACTOR;

  //how close we need to be to the pass dest
  float PASS_DIST;

  //How close to check around the creator
  float CLOSE_BOUND;

  float MIN_DIST;

  float ANGLE_LIMIT;
  
  /// Boundary Line
  float boundaryLine;
  
  /// The maximum you will go out
  Pair boundaryPoint;

  float topBound;
  float bottomBound;
  float passLine;


};

#endif //BACK_PASS_SKILL_H
