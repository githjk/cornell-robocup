/*
 *	dribble_point_skill_test.h
 */
#ifndef PLAYS_DRIBBLE_POINT_SKILL_TEST_H
#define PLAYS_DRIBBLE_POINT_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/DribblePointSkill.h"

class DribblePointSkillTest : public BasePlay 
{
public:
  static Play playNum;
  DribblePointSkillTest(VisionModule *vm) : BasePlay(vm)
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
    return "Dribble Point Skill"; 
  }
  //====================================
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  //====================================
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  //====================================
  virtual ActionFunction getActionFunction(Position pos); 
  //====================================
private:
  //Index of robot that will be dribbling
  RobotIndex ID;

  //Handle on skillset for the dribbling robot
  SkillSet* skills;

  //Handle on dribble point skill
  DribblePointSkill* dribblePointSkill;

  //robot we'll have dribbling
  RobotIndex DRIBBLE_ROBOT;
};

#endif //PLAYS_DRIBBLE_POINT_SKILL_TEST_H

