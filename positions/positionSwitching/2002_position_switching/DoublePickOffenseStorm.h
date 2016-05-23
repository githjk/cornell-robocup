#ifndef DOUBLE_PICK_OFFENSE_STORM_H
#define DOUBLE_PICK_OFFENSE_STORM_H

#include "../Storm.h"

class DoublePickOffenseStorm : public Storm
{
private:

public:
  DoublePickOffenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //DOUBLE_PICK_OFFENSE_STORM_H

