/*
 *	spinaroundballskill_test.h
 */
#ifndef PLAYS_SPIN_SKILL_TEST_H
#define PLAYS_SPIN_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/SpinSkill.h"
#include "hires_timer.h"

class SpinAroundBallSkillTest : public BasePlay 
{
public:
  static Play playNum;
  SpinAroundBallSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Spin Around Ball Skill"; 
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
  SpinAroundBallSkill* spinSkill;
    Pair center;
    Timer dribbleTimer;

};

#endif //PLAYS_SPIN_SKILL_TEST_H

