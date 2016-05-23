#ifndef DOUBLE_PICK_OFFENSE_TRANSITION_H
#define DOUBLE_PICK_OFFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* DoublePickOffense play Transition    *
*****************************/
class DoublePickOffenseTransition : public Transition
{
public:
  DoublePickOffenseTransition::DoublePickOffenseTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}
	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();

protected:
  // The max time allowed for the play
  float MAX_ELAPSED_TIME;
  float FINISH_DIST;
};
#endif // DOUBLE_PICK_OFFENSE_TRANSITION_H
