#ifndef TOUCH_SHOOT_TRANSITION_H
#define TOUCH_SHOOT_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"

/*****************************
* TouchShoot play Transition    *
*****************************/
class TouchShootTransition : public Transition
{
public:
  TouchShootTransition::TouchShootTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}
  void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();

protected:
  // The max time allowed for the play
  float MAX_ELAPSED_TIME;
  float MIN_BALL_SPEED;
};
#endif // TOUCH_SHOOT_TRANSITION_H
