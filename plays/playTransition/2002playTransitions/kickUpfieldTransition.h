#ifndef KICK_UPFIELD_TRANSITION_H
#define KICK_UPFIELD_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* KickUpfield play Transition    *
*****************************/
class KickUpfieldTransition : public Transition
{
public:
  KickUpfieldTransition::KickUpfieldTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();

private:
  float TIMEOUT;
};

#endif // KICK_UPFIELD_TRANSITION_H