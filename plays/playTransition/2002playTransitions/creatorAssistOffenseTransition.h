#ifndef CREATOR_ASSIST_OFFENSE_TRANSITION_H
#define CREATOR_ASSIST_OFFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"
/*****************************
* CreatorAssistOffense play Transition    *
*****************************/
class CreatorAssistOffenseTransition : public Transition
{
public:
  CreatorAssistOffenseTransition::CreatorAssistOffenseTransition(VisionModule *vm) : Transition(vm)
	{
    creatorTime=true;
    loadValues();
	}
	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();

protected:
  // The max time allowed for the play
  bool creatorTime;
  float MAX_CREATOR_TIME;
  float MAX_AGGRESSOR_TIME;
};
#endif // CREATOR_ASSIST_OFFENSE_TRANSITION_H
