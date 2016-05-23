#ifndef BACK_PASS_STORM_H
#define BACK_PASS_STORM_H

#include "../Storm.h"

class BackPassStorm : public Storm
{
private:

public:
  BackPassStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //BACK_PASS_STORM_H

