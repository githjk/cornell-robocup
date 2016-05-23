#ifndef PLAYS_SPECIAL_OP_CAGE_DEFENSE_SKILL_TEST_H
#define PLAYS_SPECIAL_OP_CAGE_DEFENSE_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/SpecialOpCageDefenseSkill.h"

class SpecialOpCageDefenseSkillTest : public BasePlay 
{
public:
  static Play playNum;
  SpecialOpCageDefenseSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Special Op Cage Defense Skill"; 
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
  SpecialOpCageDefenseSkill* skillHandle;
};


#endif //PLAYS_SPECIAL_OP_CAGE_DEFENSE_SKILL_TEST_H



