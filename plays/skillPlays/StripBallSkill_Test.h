/*
 *	PickSkill_test.h
 */
#ifndef STRIP_SKILL_PLAY_H
#define STRIP_SKILL_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/StripBallSkill.h"

class StripBallSkillPlay : public BasePlay 
{
public:
  static Play playNum;
  StripBallSkillPlay(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "StripBall Skill Play"; 
  }

  ~StripBallSkillPlay() {
    delete stripBallSkill;
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos); 

private:
  StripBallSkill* stripBallSkill;

};

#endif //STRIP_SKILL_PLAY_H

