#ifndef CREATOR_PASS_STORM_H
#define CREATOR_PASS_STORM_H

#include "../Storm.h"

class CreatorPassStorm : public Storm
{
private:

public:
  CreatorPassStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //CREATOR_PASS_STORM_H

