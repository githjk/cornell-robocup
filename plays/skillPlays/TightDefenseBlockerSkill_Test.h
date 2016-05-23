/*
 *	TightDefenseBlockerSkill_Test.h
 */
#ifndef PLAYS_TIGHT_DEFENSE_BLOCKER_SKILL_TEST_H
#define PLAYS_TIGHT_DEFENSE_BLOCKER_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/TightDefenseBlockerSkill.h"

class TightDefenseBlockerSkillTest : public BasePlay 
{
public:
  static Play playNum;
  TightDefenseBlockerSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Tight Defense Blocker Skill"; 
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
  TightDefenseBlockerSkill* blockerSkill;
};

#endif //PLAYS_TIGHT_DEFENSE_BLOCKER_SKILL_TEST_H



