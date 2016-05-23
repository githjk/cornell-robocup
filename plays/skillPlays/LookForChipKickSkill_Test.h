#ifndef PLAYS_LOOK_FOR_CHIP_KICK_SKILL_TEST_H
#define PLAYS_LOOK_FOR_CHIP_KICK_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/LookForChipKickSkill.h"

class LookForChipKickSkillTest : public BasePlay 
{
public:
  static Play playNum;
  LookForChipKickSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Look For Chip Kick Skill"; 
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
  LookForChipKickSkill* skillHandle;
};


#endif //PLAYS_LOOK_FOR_CHIP_KICK_SKILL_TEST_H



