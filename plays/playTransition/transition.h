#ifndef TRANSITION_H
#define TRANSITION_H

class Transition;

#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/vision/visionTimer.h"

/*************************
* Transition class  *
*************************/
class Transition : public ParameterOperations
{
public:
  Transition(VisionModule *vm) 
  { 
	  timer = new VisionTimer(vm);
  }

  virtual ~Transition() {delete timer;}

  virtual void executePlayTransition(VisionData& vision, 
                                     RobocupStrategyData* rsd, 
                                     BasePlay* currentPlay) = 0;

  void setPlay(int play,
               RobocupStrategyData* rsd);

  virtual void loadValues() = 0;

protected:
  VisionTimer *timer;
};
#endif