#ifndef CAGE_DEFENSE_STORM_H
#define CAGE_DEFENSE_STORM_H

#include "../Storm.h"

class CageDefenseStorm : public Storm
{
private:

public:
  CageDefenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //CAGE_DEFENSE_STORM_H

