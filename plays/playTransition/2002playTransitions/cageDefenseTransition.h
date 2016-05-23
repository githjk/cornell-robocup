#ifndef CAGE_DEFENSE_TRANSITION_H
#define CAGE_DEFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* CageDefense play Transition    *
*****************************/
class CageDefenseTransition : public Transition
{

public:
  CageDefenseTransition::CageDefenseTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay *currentPlay);
	void loadValues();
};
#endif // CAGE_DEFENSE_TRANSITION_H
