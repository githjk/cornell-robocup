#ifndef LOOSE_DEFENSE_TRANSITION_H
#define LOOSE_DEFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"
/*****************************
* LooseDefense play Transition    *
*****************************/
class LooseDefenseTransition : public Transition
{
public:
    LooseDefenseTransition::LooseDefenseTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}
	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
};
#endif // LOOSE_DEFENSE_TRANSITION_H
