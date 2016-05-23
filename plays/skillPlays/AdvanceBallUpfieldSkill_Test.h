#ifndef PLAYS_ADVANCE_BALL_UPFIELD_SKILL_TEST_H
#define PLAYS_ADVANCE_BALL_UPFIELD_SKILL_TEST_H
#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/AdvanceBallUpfieldSkill.h"

class AdvanceBallUpfieldSkillTest : public BasePlay 
{
public:
  static Play playNum;
  AdvanceBallUpfieldSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Advance Ball Upfield Skill"; 
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
  AdvanceBallUpfieldSkill* skillHandle;
};


#endif //PLAYS_ADVANCE_BALL_UPFIELD_SKILL_TEST_H



