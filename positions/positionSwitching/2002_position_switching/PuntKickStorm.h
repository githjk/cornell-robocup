#ifndef PUNK_KICK_STORM_H
#define PUNK_KICK_STORM_H

#include "../Storm.h"

class PuntKickStorm : public Storm
{
private:

public:
  PuntKickStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //PUNK_KICK_STORM_H

