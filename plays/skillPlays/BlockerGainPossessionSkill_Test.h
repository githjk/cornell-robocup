#ifndef PLAYS_BLOCKER_GAIN_POSSESSION_SKILL_TEST_H
#define PLAYS_BLOCKER_GAIN_POSSESSION_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/BlockerGainPossessionSkill.h"

class BlockerGainPossessionSkillTest : public BasePlay 
{
public:
  static Play playNum;
  BlockerGainPossessionSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Blocker Gain Possession Skill"; 
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
  BlockerGainPossessionSkill* skillHandle;
};


#endif //PLAYS_BLOCKER_GAIN_POSSESSION_SKILL_TEST_H



