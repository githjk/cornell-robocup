#ifndef THREE_MAN_DEFENSE_TRANSITION_H
#define THREE_MAN_DEFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"
/*****************************
* LooseDefense play Transition    *
*****************************/
class ThreeManDefenseTransition : public Transition
{
public:
    ThreeManDefenseTransition::ThreeManDefenseTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}
	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
};
#endif // THREE_MAN_DEFENSE_TRANSITION_H