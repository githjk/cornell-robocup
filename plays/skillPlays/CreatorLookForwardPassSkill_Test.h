/*
 *	CreatorLookForwadPassSkill_Test.h
 */
#ifndef PLAYS_CREATOR_LOOK_FORWARD_PASS_SKILL_TEST_H
#define PLAYS_CREATOR_LOOK_FORWARD_PASS_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/CreatorLookForwardPassSkill.h"

class CreatorLookForwardPassSkillTest : public BasePlay 
{
public:
  static Play playNum;
  CreatorLookForwardPassSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Creator Look Forward Pass Skill"; 
  }
  //====================================
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  //====================================
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  //====================================
  virtual ActionFunction getActionFunction(Position pos); 
  //====================================
private:
  //Index of robot that will be spinning
  RobotIndex ID;

  //Handle on skillset for the spinning robot
  SkillSet* skills;

  //Handle on spinning skill
  CreatorLookForwardPassSkill* lookForwardPassSkill;
};

#endif //PLAYS_CREATOR_LOOK_FORWARD_PASS_SKILL_TEST_H



