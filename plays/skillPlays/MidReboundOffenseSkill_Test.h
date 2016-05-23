#ifndef PLAYS_MID_REBOUND_OFFENSE_SKILL_TEST_H
#define PLAYS_MID_REBOUND_OFFENSE_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/MidReboundOffenseSkill.h"

class MidReboundOffenseSkillTest : public BasePlay 
{
public:
  static Play playNum;
  MidReboundOffenseSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Mid Rebound Offense Skill"; 
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
  MidReboundOffenseSkill* skillHandle;
};


#endif //PLAYS_MID_REBOUND_OFFENSE_SKILL_TEST_H



