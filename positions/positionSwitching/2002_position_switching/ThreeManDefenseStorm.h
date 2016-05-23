#ifndef THREE_MAN_DEFENSE_STORM_H
#define THREE_MAN_DEFENSE_STORM_H

#include "../Storm.h"

class ThreeManDefenseStorm : public Storm
{
private:
public:
  ThreeManDefenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);

  void loadValues();
protected:
  float BUFFER_TIME;
};

#endif //THREE_MAN_DEFENSE_STORM_H