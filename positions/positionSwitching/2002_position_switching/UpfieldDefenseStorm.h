#ifndef UPFIELD_DEFENSE_STORM_H
#define UPFIELD_DEFENSE_STORM_H

#include "../Storm.h"

class UpfieldDefenseStorm : public Storm
{
private:

public:
  UpfieldDefenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //UPFIELD_DEFENSE_STORM_H

