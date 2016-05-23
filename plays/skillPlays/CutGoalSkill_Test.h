/*
 *	cut_goal_skill_test.h
 */
#ifndef PLAYS_CUT_GOAL_SKILL_TEST_H
#define PLAYS_CUT_GOAL_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/CutGoalSkill.h"

class CutGoalSkillTest : public BasePlay 
{
public:
  static Play playNum;
  CutGoalSkillTest(VisionModule *vm) : BasePlay(vm)
  {
  }

  //====================================
  Play getID() const 
  {
	  return playNum;
  }
  //====================================
  char* getName() const 
  { 
	  return "Cut Goal Skill"; 
  }
  //====================================
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  //====================================
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  //====================================
  virtual ActionFunction getActionFunction(Position pos); 
  //====================================
private:
  //Index of robot that will be screening
  RobotIndex ID;

  //Handle on skillset for the screening robot
  SkillSet* skills;

  //Handle on simple defense skill
  CutGoalSkill* cutGoalSkill;
};

#endif //PLAYS_CUT_GOAL_SKILL_TEST_H