#include "ParameterOperations.h"
#ifndef CONTROL_PARAMS_H
#define CONTROL_PARAMS_H

//==================================================================
//==================================================================
//These parameters vary for each speed and type of robot
struct TrajectoryParams
{
  float	MAX_ACCELERATION;	//m/s/s
  float	MAX_VELOCITY;	    //m/s
  float LATERAL_GAIN;

  float MAX_THETA_ACCELERATION;
  float MAX_THETA_VELOCITY;
  float	ANGULAR_GAIN;
};
//==================================================================
//==================================================================
//This set of trajectory params applies to a 
//given robot type, say 2001 omni,
//2002 omni, etc.
struct RobotTypeParams
{
  float PROP_GAIN_DISTANCE;  //distance within which to use proportional gains on translation
  float STOP_DISTANCE;       //when robot is within this distance just stop translating robot

  float PROP_GAIN_THETA_DISTANCE;  //distance within which to use proportional gains on rotation
  float STOP_THETA_DISTANCE;       //when robot is within this distance just stop translating robot

  TrajectoryParams DEFAULT;
  TrajectoryParams GOALIE;
  TrajectoryParams SPIN_TO_KICK;
  TrajectoryParams CAREFUL_SPEED;
  TrajectoryParams BALL_POSSESSION_SPEED;
  TrajectoryParams BALL_APPROACH_SPEED;
  TrajectoryParams FORMATION_SPEED;
  TrajectoryParams SLOW_ROTATE;
};
//==================================================================
//==================================================================
//Control Params has set of Robot params for all robot types
class ControlParams : public ParameterOperations
{
public:
  //store params for each type of robot
  RobotTypeParams Omni2001;
  RobotTypeParams Omni2002;
  
  //constructor, calls loadValues 
  ControlParams();

  //opens param files for various omni types and fills structs with loaded values
  void loadValues();
};
//==================================================================
//==================================================================

#endif // CONTROL_PARAMS_H

