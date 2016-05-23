#include "GeneralParams.h"
#include "assert.h"
#include <stdio.h>
#include "ControlData.h"  //for default dribble speed

// For TEAM_ONE and TEAM_TWO
#include "vision_globals.h"

//==================================================================
//==================================================================
//constructor, calls loadValues 
GeneralParams::GeneralParams()
{
  loadValues();
}
//==================================================================
//==================================================================
//load field params
void GeneralParams::loadValues()
{
  //-----------------------------------------
  ifstream mainFile;
  mainFile.open("Params/GeneralParams.txt", ios::nocreate);
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN Params/GeneralParams.txt!");
  //-------------------
  READ_FLOAT(PLAY_TRANSITION_HISTORISIS);
  READ_FLOAT(FRAME_RATE);
  READ_FLOAT(LATENCY_FRAMES);
  READ_FLOAT(OTHER_LATENCY_FRAMES);
  READ_INT(OUR_LOST_FRAMES);
  READ_INT(BALL_LOST_FRAMES);
  READ_INT(OPPONENT_LOST_FRAMES);
  READ_INT(TEAM);

  switch (TEAM) 
  {
    case TEAM_ONE: OTHER_TEAM = TEAM_TWO; break;
    case TEAM_TWO: OTHER_TEAM = TEAM_ONE; break;
    default: 
      TEAM = TEAM_ONE;
      OTHER_TEAM = TEAM_TWO;
      break;
  }

  READ_FLOAT(BALL_RADIUS);
  READ_FLOAT(PLAYER_RADIUS);
  READ_FLOAT(OPPONENT_RADIUS);

  READ_FLOAT(WALL_BUFFER);
  READ_FLOAT(VISION_OFFSET);

  READ_INT(GAIN_POSSESSION_FRAMES);
  READ_INT(LOSE_POSSESSION_FRAMES);
  READ_FLOAT(DIST_TO_DRIBBLER);
  READ_FLOAT(DRIBBLER_LENGTH);
  READ_FLOAT(POSSESSION_GAIN_TOLERANCE);
  READ_FLOAT(POSSESSION_LOSE_TOLERANCE);

  READ_INT(DEFAULT_DRIBBLER_SPEED);
  DRIBBLE_DEFAULT=(DribbleSpeed)DEFAULT_DRIBBLER_SPEED;
  V_DRIBBLE_DEFAULT=(VerticalDribbleSpeed)DEFAULT_DRIBBLER_SPEED;

  READ_FLOAT(ANGLE_OFFSET_ROBOT_ZERO);
  READ_FLOAT(ANGLE_OFFSET_ROBOT_ONE);
  READ_FLOAT(ANGLE_OFFSET_ROBOT_TWO);
  READ_FLOAT(ANGLE_OFFSET_ROBOT_THREE);
  READ_FLOAT(ANGLE_OFFSET_ROBOT_FOUR);

  //--
  //2002 Obstacle Avoidance Params
  READ_FLOAT(SLOW_CLEARANCE);
  READ_FLOAT(FAST_CLEARANCE);
  READ_FLOAT(FAST_CLEARANCE_VELOCITY);
  READ_FLOAT(MAX_WALL_OFFSET);
  READ_FLOAT(MAX_WALL_OFFSET_DISTANCE);
  //--
  //2001 Obstacle Avoidance Params 
  READ_FLOAT(OBSTACLE_CLEARANCE);
  READ_FLOAT(MAX_AVOIDANCE_ACCEL);
  READ_FLOAT(MAX_AVOIDANCE_VEL);
  //--  
  
  READ_STRING(SERVER_ADDR);
  READ_INT(VISION_PORT);
  READ_INT(SIMULATOR_PORT);
  READ_INT(GOALIE_INDEX);
  READ_INT(COLOR_SCHEME);
  READ_INT(COMPLEX_MODE);
  READ_STRING(COM_PORT);
  READ_INT(WIRLESS_BOARD);
  READ_INT(THREAD_PRIORITY);
  //calculate minimum separation distance
  MIN_SEPARATION_DISTANCE = PLAYER_RADIUS + 
                            OPPONENT_RADIUS + 
                            2.0f*OBSTACLE_CLEARANCE;

      // Opponent Speed Values
  READ_FLOAT(OPPONENT_SPEED.MAX_ACCELERATION);
  READ_FLOAT(OPPONENT_SPEED.MAX_VELOCITY);


  READ_INT(ENABLE_PREDICTION_AND_FILTERING_OPPONENTS);
  //-------------------
  mainFile.close(); 
  //-----------------------------------------
}
//==================================================================
//==================================================================
