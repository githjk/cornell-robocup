#ifndef PLAY_TRANSITION_H
#define PLAY_TRANSITION_H

class PlayTransition;

#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "transition.h"

/*************************
* PlayTransition class  *
*************************/
class PlayTransition
{
private:
  Transition** transitionBook;
public:
  PlayTransition(VisionModule* vision);
  ~PlayTransition();
  void transition(VisionData* vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
  void loadTransitionParams();
};
#endif