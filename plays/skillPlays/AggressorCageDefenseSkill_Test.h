#ifndef PLAYS_AGGRESSOR_CAGE_DEFENSE_SKILL_TEST_H
#define PLAYS_AGGRESSOR_CAGE_DEFENSE_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/CageDefenseSkill.h"

class AggressorCageDefenseSkillTest : public BasePlay 
{
public:
  static Play playNum;
  AggressorCageDefenseSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Aggressor Cage Defense Skill"; 
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
  CageDefenseSkill* skillHandle;
};


#endif //PLAYS_AGGRESSOR_CAGE_DEFENSE_SKILL_TEST_H



