/*
 *	TurnAndKickSkill_test.h
 */
#ifndef PLAYS_TURN_AND_KICK_TEST_H
#define PLAYS_TURN_AND_KICK_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/TurnAndKickSkill.h"
#include "hires_timer.h"

class TurnAndKickSkillTest : public BasePlay 
{
public:
  static Play playNum;
  TurnAndKickSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Turn And Kick Skill"; 
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
  TurnAndKickSkill* kickSkill;
  Pair center;
  Timer dribbleTimer;

};

#endif //PLAYS_TURN_AND_KICK_TEST_H

