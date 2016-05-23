/*
 *	TestSkill_play.h
 */
#ifndef TEST_SKILL_PLAY_H
#define TEST_SKILL_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class TestSkillPlay : public BasePlay 
{
public:
  static Play playNum;
  TestSkillPlay(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Test Skill"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos); 
};

#endif //TEST_SKILL_PLAY_H

