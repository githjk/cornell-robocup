#ifndef BACK_PASS_TRANSITION_H
#define BACK_PASS_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* UpfieldDefense play Transition    *
*****************************/
class BackPassTransition : public Transition
{
public:
  BackPassTransition::BackPassTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
protected:
  // The max time allowed for the play
  float MAX_ELAPSED_TIME;

};
#endif // BACK_PASS_TRANSITION_H