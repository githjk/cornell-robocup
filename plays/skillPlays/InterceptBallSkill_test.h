/*
 *	InterceptBallSkill_test.h
 */
#ifndef PLAYS_INTERCEPT_SKILL_TEST_H
#define PLAYS_INTERCEPT_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/InterceptBallSkill.h"
#include "hires_timer.h"

class InterceptBallSkillTest : public BasePlay 
{
public:
  static Play playNum;
  InterceptBallSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Intercept Ball Skill"; 
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
  RobotIndex ID,simpleID;
  //Handle on skillset for the spinning robot
  SkillSet* skills;

  //Handle on spinning skill
  InterceptBallSkill* skill;
  float SafeZone;
  bool init;

};

#endif //PLAYS_INTERCEPT_SKILL_TEST_H

