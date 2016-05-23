#ifndef PENALTY_SHOT_THEM_SMART_RUSH_2002_TRANSITION_H
#define PENALTY_SHOT_THEM_SMART_RUSH_2002_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* UpfieldDefense play Transition    *
*****************************/
class PenaltyShotThemSmartRush2002Transition : public Transition
{
public:
  PenaltyShotThemSmartRush2002Transition::PenaltyShotThemSmartRush2002Transition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();

private:
  float TIMEOUT;
  float BALL_THRESH;
};
#endif // PENALTY_SHOT_THEM_SMART_RUSH_2002_TRANSITION_H