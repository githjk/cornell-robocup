#ifndef REMOVE_BALL_DEFENSE_TRANSITION_H
#define REMOVE_BALL_DEFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"
/*****************************
* RemoveBallDefense play Transition    *
*****************************/
class RemoveBallDefenseTransition : public Transition
{
public:
  RemoveBallDefenseTransition::RemoveBallDefenseTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}
	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();

protected:
  // The max time allowed after the Blocker triggers the chip kick before transitioning
  float MAX_ELAPSED_TIME_SINCE_CHIP_KICK;

  //The max distance allowed between the Blocker and the ball before transitioning
  float MAX_DISTANCE_BETWEEN_BLOCKER_AND_BALL;		
};
#endif // REMOVE_BALL_DEFENSE_TRANSITION_H
