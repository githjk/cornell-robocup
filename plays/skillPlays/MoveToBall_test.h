/*
 *	MoveToBall_test.h
 */
#ifndef MOVE_TO_BALL_PLAY_H
#define MOVE_TO_BALL_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/MoveToBallSkill.h"

class MoveToBallPlay : public BasePlay 
{
public:
  static Play playNum;
  MoveToBallPlay(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Move To Ball Skill"; 
  }
  
  ~MoveToBallPlay() {
    delete moveToBallSkill;
  }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos); 

  MoveToBallSkill *moveToBallSkill;
};

#endif //MOVE_TO_BALL_PLAY_H

