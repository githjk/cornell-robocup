#ifndef CROSS_CREASE_PASS_TRANSITION_H

#define CROSS_CREASE_PASS_TRANSITION_H



#include "robocup_strategy_data.h"
#include "..\transition.h"


/*****************************

* CrossCreasePass play Transition    *

*****************************/

class CrossCreasePassTransition : public Transition

{

	
public:
    CrossCreasePassTransition::CrossCreasePassTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);

	void loadValues();


protected:
  // The max time allowed for the play
  float MAX_ELAPSED_TIME;

};

#endif // CROSS_CREASE_PASS_TRANSITION_H
