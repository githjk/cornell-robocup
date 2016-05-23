#ifndef JAM_AND_SHOOT_TRANSITION_H
#define JAM_AND_SHOOT_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"
/*****************************
* JamAndShoot play Transition    *
*****************************/
class JamAndShootTransition : public Transition
{
public:
  JamAndShootTransition::JamAndShootTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}
	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
};
#endif // JAM_AND_SHOOT_TRANSITION_H
