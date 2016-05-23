#ifndef SYSTEM_PARAMETERS_H
#define SYSTEM_PARAMETERS_H

class TrajectoryModule;
class RoboCupStrategyModule;

class SystemParameters;

#include "ControlParams.h"
#include "FieldParams.h"
#include "StrategyParams2002.h"
#include "GeneralParams.h"

//==================================================================
//==================================================================
//Contains all system parameters
class SystemParameters
{
public:
  //when objecst are created values loaded automatically so constructor does nothing
  SystemParameters();

  //reloads all values
  void loadValues();
  //------------------------------------
  //Robot Specific Trajectory Related Params (eg default max velocity, etc)
  ControlParams control;

  //Field boundaries
  FieldParams field;

  //Unorganized 2001 strategy params
  StrategyParams2002 strategy2002;
  
  //Other uncatagorized params
  GeneralParams general;
};
//==================================================================
//==================================================================

#endif //SYSTEM_PARAMETERS_H

