#ifndef SINGLE_ASSIST_OFFENSE_STORM_H
#define SINGLE_ASSIST_OFFENSE_STORM_H

#include "../Storm.h"

class SingleAssistOffenseStorm : public Storm
{
private:

public:
  SingleAssistOffenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //SINGLE_ASSIST_OFFENSE_STORM_H

