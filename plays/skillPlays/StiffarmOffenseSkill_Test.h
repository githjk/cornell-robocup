#ifndef PLAYS_STIFFARM_OFFENSE_SKILL_TEST_H
#define PLAYS_STIFFARM_OFFENSE_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/StiffarmOffenseSkill.h"

class StiffarmOffenseSkillTest : public BasePlay 
{
public:
  static Play playNum;
  StiffarmOffenseSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Stiffarm Offense Skill"; 
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
  StiffarmOffenseSkill* skillHandle;
};


#endif //PLAYS_STIFFARM_OFFENSE_SKILL_TEST_H



