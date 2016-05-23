/*
 *	simple_screen_skill_test.h
 */
#ifndef PLAYS_SIMPLE_SCREEN_SKILL_TEST_H
#define PLAYS_SIMPLE_SCREEN_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/SimpleScreenSkill.h"

class SimpleScreenSkillTest : public BasePlay 
{
public:
  static Play playNum;
  SimpleScreenSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Simple Screen Skill"; 
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

  //Handle on screening skill
  SimpleScreenSkill* screenSkill;
};

#endif //PLAYS_SIMPLE_SCREEN_SKILL_TEST_H

