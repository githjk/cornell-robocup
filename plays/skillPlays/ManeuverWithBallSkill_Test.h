/*
 *	ManeuverWithBallSkill_test.h
 */
#ifndef PLAYS_MANEUVER_SKILL_TEST_H
#define PLAYS_MANEUVER_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/ManeuverSkill.h"
#include "hires_timer.h"

class ManeuverWithBallSkillTest : public BasePlay 
{
public:
  static Play playNum;
  ManeuverWithBallSkillTest(VisionModule *vm) : BasePlay(vm)
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
	  return "Maneuver With Ball Skill"; 
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
  ManeuverWithBallSkill* maneuverSkill;
  Pair center;
  Timer dribbleTimer;

};

#endif //PLAYS_MANEUVER_SKILL_TEST_H

