#ifndef FREE_KICK_TRANSITION_H
#define FREE_KICK_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* KickUpfield play Transition    *
*****************************/
class FreeKickTransition : public Transition
{
public:
  FreeKickTransition::FreeKickTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();

private:
  float TIMEOUT;
};

#endif // FREE_KICK_TRANSITION_H