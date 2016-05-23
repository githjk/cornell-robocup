#ifndef WALL_FORMATION_STORM_H
#define WALL_FORMATION_STORM_H

#include "../Storm.h"

class WallFormationStorm : public Storm
{
private:
  Position rPosition[NUM_ROBOTS-1];
  RobotIndex rID[NUM_ROBOTS-1];
  Pair rLocation[NUM_ROBOTS-1];
   
public:
  WallFormationStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //WALL_FORMATION_STORM_H

