#ifndef KICK_UPFIELD_STORM_H
#define KICK_UPFIELD_STORM_H

#include "../Storm.h"

class KickUpfieldStorm : public Storm
{
private:

public:
  KickUpfieldStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //KICK_UPFIELD_STORM_H

