/*
 *	LooseDSpecOpSkill_Test.h
 */
#ifndef LOOSE_D_SPECOP_PLAY_H
#define LOOSE_D_SPECOP_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/LooseDSpecialOpSkill.h"

class LooseDSpecOpTest : public BasePlay 
{
public:
  static Play playNum;
  LooseDSpecOpTest(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Loose D SpecOp Play"; 
  }
  
  ~LooseDSpecOpTest() {
    delete skills;
    delete skill;
  }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos); 

private:
  LooseDSpecialOpSkill* skill;
  SkillSet* skills;
  RobotIndex ID;
};

#endif //LOOSE_D_SPECOP_PLAY_H

