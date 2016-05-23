#ifndef PENALTY_SHOT_US_OPEN_LOOP_2002_TRANSITION_H
#define PENALTY_SHOT_US_OPEN_LOOP_2002_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/******************************************
* Penalty Shot Us Open Loop 2002 Play Transition    *
*******************************************/
class PenaltyShotUsOpenLoop2002Transition : public Transition
{
public:
  PenaltyShotUsOpenLoop2002Transition::PenaltyShotUsOpenLoop2002Transition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
private:
  float TIMEOUT;
};
#endif // PENALTY_SHOT_US_OPEN_LOOP_2002_TRANSITION_H