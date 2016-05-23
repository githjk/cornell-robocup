#ifndef SHORTEST_DISTANCE_STORM_H
#define SHORTEST_DISTANCE_STORM_H

#include "../Storm.h"

class ShortestDistanceStorm : public Storm
{
private:
  Position rPosition[NUM_ROBOTS-1];
  RobotIndex rID[NUM_ROBOTS-1];
  Pair rLocation[NUM_ROBOTS-1];
  Pair rDestI, rDestJ;
public:
  ShortestDistanceStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //SHORTEST_DISTANCE_STORM_H

