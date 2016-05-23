#ifndef LOOSE_DEFENSE_STORM_H
#define LOOSE_DEFENSE_STORM_H

#include "../Storm.h"

class LooseDefenseStorm : public Storm
{
private:

public:
  LooseDefenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //LOOSE_DEFENSE_STORM_H

