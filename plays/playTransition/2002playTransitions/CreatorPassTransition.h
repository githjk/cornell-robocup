#ifndef CREATOR_PASS_TRANSITION_H
#define CREATOR_PASS_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* CreatorPass play Transition    *
*****************************/
class CreatorPassTransition : public Transition
{
public:
  CreatorPassTransition::CreatorPassTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
protected:
  // The max time allowed for the play
  float MAX_ELAPSED_TIME;
};
#endif // CREATOR_PASS_TRANSITION_H
