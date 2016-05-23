#ifndef WEATHER_H
#define WEATHER_H

class Weather;

#include "Storm.h"
#include "plays/playbook.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/datatypes/SystemParameters.h"

class Weather
{
private:
  Storm** storms;
  RoboCupStrategyModule* strategy;

public:
  Weather(RoboCupStrategyModule* strategy);
  ~Weather();
  void preformSwitching(VisionModule* vm,
                        SystemParameters* params);
  void loadWeatherParams();
};

#endif //WEATHER_H

