#ifndef CREATOR_ASSIST_OFFENSE_STORM_H
#define CREATOR_ASSIST_OFFENSE_STORM_H

#include "../Storm.h"

class CreatorAssistOffenseStorm : public Storm
{
private:

public:
  CreatorAssistOffenseStorm(RoboCupStrategyModule* strategy);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);
};

#endif //CREATOR_ASSIST_OFFENSE_STORM_H

