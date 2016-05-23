#ifndef JAM_AND_SHOOT_STORM_H
#define JAM_AND_SHOOT_STORM_H

#include "../Storm.h"

class JamAndShootStorm : public Storm
{
private:

public:
  JamAndShootStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //JAM_AND_SHOOT_STORM_H

