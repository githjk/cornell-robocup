#ifndef PENALTY_SHOT_US_SIDE_STEP_2002_TRANSITION_H
#define PENALTY_SHOT_US_SIDE_STEP_2002_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/******************************************
* Penalty Shot Us 2002 Play Transition    *
*******************************************/
class PenaltyShotUsSideStep2002Transition : public Transition
{
public:
  PenaltyShotUsSideStep2002Transition::PenaltyShotUsSideStep2002Transition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
private:
  float TIMEOUT;
};
#endif // PENALTY_SHOT_US_SIDE_STEP_2002_TRANSITION_H