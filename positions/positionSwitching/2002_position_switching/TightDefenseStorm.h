#ifndef TIGHT_DEFENSE_STORM_H
#define TIGHT_DEFENSE_STORM_H

#include "../Storm.h"

class TightDefenseStorm : public Storm
{
private:

public:
  TightDefenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //TIGHT_DEFENSE_STORM_H

