#include "transition.h"

//===================================================
void Transition::setPlay(int play,
                         RobocupStrategyData* rsd)
{
  //set the new play
  rsd->setNewPlay(play);
 
  //unitialize all skills
  for(RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++)
  {
    rsd->getStrategyModule().getSkillSet(i)->unitializeSkillz();
  }
}
//===================================================

