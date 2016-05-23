#ifndef CROSS_CREASE_PASS_STORM_H
#define CROSS_CREASE_PASS_STORM_H

#include "../Storm.h"

class CrossCreasePassStorm : public Storm
{
private:

public:
  CrossCreasePassStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //CROSS_CREASE_PASS_STORM_H

