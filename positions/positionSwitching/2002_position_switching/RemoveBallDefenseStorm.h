#ifndef REMOVE_BALL_DEFENSE_STORM_H
#define REMOVE_BALL_DEFENSE_STORM_H

#include "../Storm.h"

class RemoveBallDefenseStorm : public Storm
{
private:

public:
  RemoveBallDefenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //REMOVE_BALL_DEFENSE_STORM_H

