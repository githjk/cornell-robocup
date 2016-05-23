#ifndef PENALTY_SHOT_US_PIVOT_2002_TRANSITION_H
#define PENALTY_SHOT_US_PIVOT_2002_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/******************************************
* Penalty Shot Us Pivot 2002 Play Transition    *
*******************************************/
class PenaltyShotUsPivot2002Transition : public Transition
{
public:
  PenaltyShotUsPivot2002Transition::PenaltyShotUsPivot2002Transition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
private:
  float TIMEOUT;
};
#endif // PENALTY_SHOT_US_PIVOT_2002_TRANSITION_H