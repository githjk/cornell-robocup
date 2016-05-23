#ifndef PLAYS_CROSS_CREASE_PASS_SKILL_TEST_H
#define PLAYS_CROSS_CREASE_PASS_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/CrossCreasePassSkill.h"

class CrossCreasePassSkillTest : public BasePlay 
{
public:
  static Play playNum;
  CrossCreasePassSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Cross Crease Pass Skill"; 
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
  CrossCreasePassSkill* skillHandle;
};


#endif //PLAYS_CROSS_CREASE_PASS_SKILL_TEST_H



