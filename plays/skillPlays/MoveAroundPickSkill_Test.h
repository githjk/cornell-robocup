/*
 *	PickSkill_test.h
 */
#ifndef MOVE_AROUND_PICK_SKILL_PLAY_H
#define MOVE_AROUND_PICK_SKILL_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/MoveAroundPickSkill.h"

class MoveAroundPickSkillPlay : public BasePlay 
{
public:
  static Play playNum;
  MoveAroundPickSkillPlay(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "MoveAroundPickSkill Play"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos); 

private:
  MoveAroundPickSkill* moveAroundPickSkill;
  Pair dest;

};

#endif //MOVE_AROUND_PICK_SKILL_PLAY_H

