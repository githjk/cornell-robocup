#ifndef STORM_H
#define STORM_H

class Storm;

#include "Cloud.h"
#include "common/datatypes/robocup_strategy_data.h"
#include "common/datatypes/SystemParameters.h"
#include "common/datatypes/ParameterOperations.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "VisionModule.h"

class Storm : public ParameterOperations
{
protected:
  Cloud** clouds;
  int numClouds;
  RoboCupStrategyModule* strategy;

public:
  Storm(RoboCupStrategyModule* strategy);
  ~Storm();

  void
    switchPositions(Position pos1, Position pos2);

  virtual void preformSwitching(VisionModule* vm,
                                SystemParameters* params);

};

#endif //STORM_H

