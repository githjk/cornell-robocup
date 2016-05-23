#ifndef PUNT_KICK_TRANSITION_H
#define PUNT_KICK_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* UpfieldDefense play Transition    *
*****************************/
class PuntKickTransition : public Transition
{
public:
  PuntKickTransition::PuntKickTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
};
#endif // PUNT_KICK_TRANSITION_H