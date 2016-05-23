#ifndef TIGHT_DEFENSE_TRANSITION_H
#define TIGHT_DEFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"
/*****************************
* TightDefense play Transition    *
*****************************/
class TightDefenseTransition : public Transition
{
public:
  TightDefenseTransition::TightDefenseTransition(VisionModule *vm) : Transition(vm)
  {
      loadValues();
	}
	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
};
#endif // TIGHT_DEFENSE_TRANSITION_H


