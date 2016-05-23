#ifndef TOUCH_SHOOT_STORM_H
#define TOUCH_SHOOT_STORM_H

#include "../Storm.h"

class TouchShootStorm : public Storm
{
private:

public:
  TouchShootStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //TOUCH_SHOOT_STORM_H

