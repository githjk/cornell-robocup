#ifndef GENERAL_PARAMS_H
#define GENERAL_PARAMS_H

#include "ParameterOperations.h"
#include "ControlParams.h" // for opponent speed.

//==================================================================
//==================================================================
//Field Boundaries
class GeneralParams : public ParameterOperations
{
public:
  //----------------------------------------------------------------
  //constructor, calls loadValues 
  GeneralParams();

  //opens param files for various omni types and fills structs with loaded values
  void loadValues();
  //----------------------------------------------------------------
  float PLAY_TRANSITION_HISTORISIS;
  float FRAME_RATE;
  float LATENCY_FRAMES;
  float OTHER_LATENCY_FRAMES;
  int OUR_LOST_FRAMES;
  int BALL_LOST_FRAMES;
  int OPPONENT_LOST_FRAMES;
  int TEAM;
  int OTHER_TEAM;
  float BALL_RADIUS;
  float PLAYER_RADIUS;
  float OPPONENT_RADIUS;
  TrajectoryParams OPPONENT_SPEED;

  float WALL_BUFFER;
  float VISION_OFFSET;

  int GAIN_POSSESSION_FRAMES;
  int LOSE_POSSESSION_FRAMES;
  float DIST_TO_DRIBBLER;
  float DRIBBLER_LENGTH;
  float DRIBBLER_LENGTH_TOL;
  float POSSESSION_GAIN_TOLERANCE;
  float POSSESSION_LOSE_TOLERANCE;

  int DEFAULT_DRIBBLER_SPEED;

  float ANGLE_OFFSET_ROBOT_ZERO;
  float ANGLE_OFFSET_ROBOT_ONE;
  float ANGLE_OFFSET_ROBOT_TWO;
  float ANGLE_OFFSET_ROBOT_THREE;
  float ANGLE_OFFSET_ROBOT_FOUR;

  //--
  //2002 Obstacle Avoidance Params
  float SLOW_CLEARANCE;
  float FAST_CLEARANCE;
  float FAST_CLEARANCE_VELOCITY;

  float MAX_WALL_OFFSET;
  float MAX_WALL_OFFSET_DISTANCE;
  //--
  //2001 Obstacle Avoidance Params
  float OBSTACLE_CLEARANCE;
  float MAX_AVOIDANCE_ACCEL;
  float MAX_AVOIDANCE_VEL;
  //--

  char SERVER_ADDR[80];
  int VISION_PORT;
  int SIMULATOR_PORT;
  int GOALIE_INDEX;
  int COLOR_SCHEME;
  int COMPLEX_MODE;
  char COM_PORT[80];
  int WIRLESS_BOARD;
  int THREAD_PRIORITY;
  float MIN_SEPARATION_DISTANCE;

  int ENABLE_PREDICTION_AND_FILTERING_OPPONENTS;
  //----------------------------------------------------------------
};
//==================================================================
//==================================================================

#endif //GENERAL_PARAMS_H

