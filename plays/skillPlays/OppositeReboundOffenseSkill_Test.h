#ifndef PLAYS_OPPOSITE_REBOUND_OFFENSE_SKILL_TEST_H
#define PLAYS_OPPOSITE_REBOUND_OFFENSE_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/OppositeReboundOffenseSkill.h"

class OppositeReboundOffenseSkillTest : public BasePlay 
{
public:
  static Play playNum;
  OppositeReboundOffenseSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Opposite Rebound Offense Skill"; 
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
  OppositeReboundOffenseSkill* skillHandle;
};


#endif //PLAYS_OPPOSITE_REBOUND_OFFENSE_SKILL_TEST_H



