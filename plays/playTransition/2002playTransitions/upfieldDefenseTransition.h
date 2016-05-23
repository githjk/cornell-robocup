#ifndef UPFIELD_DEFENSE_TRANSITION_H
#define UPFIELD_DEFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* UpfieldDefense play Transition    *
*****************************/
class UpfieldDefenseTransition : public Transition
{
public:
  UpfieldDefenseTransition::UpfieldDefenseTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
};
#endif // UPFIELD_DEFENSE_TRANSITION_H