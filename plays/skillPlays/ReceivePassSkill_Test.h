/*
 *	receive_pass-skill_test.h
 */
#ifndef PLAYS_RECEIVE_PASS_SKILL_TEST_H
#define PLAYS_RECEIVE_PASS_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/ReceivePassSkill.h"

class ReceivePassSkillTest : public BasePlay 
{
public:
  static Play playNum;
  ReceivePassSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Receive Pass Skill"; 
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
  ReceivePassSkill* screenSkill;
};

#endif //PLAYS_RECEIVE_PASS_SKILL_TEST_H

