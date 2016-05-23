/*
 *	PickSkill_test.h
 */
#ifndef PICK_SKILL_PLAY_H
#define PICK_SKILL_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/PickSkill.h"

class PickSkillPlay : public BasePlay 
{
public:
  static Play playNum;
  PickSkillPlay(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Pick Skill Play"; 
  }
  
  ~PickSkillPlay() {
    delete skills;
    delete pickSkill;
  }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos); 

private:
  PickSkill* pickSkill;
  SkillSet* skills;
  RobotIndex ID;
};

#endif //PICK_SKILL_PLAY_H

