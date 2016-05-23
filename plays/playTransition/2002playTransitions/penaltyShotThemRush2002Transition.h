#ifndef PENALTY_SHOT_THEM_RUSH_2002_TRANSITION_H
#define PENALTY_SHOT_THEM_RUSH_2002_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* UpfieldDefense play Transition    *
*****************************/
class PenaltyShotThemRush2002Transition : public Transition
{
public:
  PenaltyShotThemRush2002Transition::PenaltyShotThemRush2002Transition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();

private:
  float TIMEOUT;
};
#endif // PENALTY_SHOT_THEM_RUSH_2002_TRANSITION_H