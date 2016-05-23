#include "ParameterOperations.h"
#ifndef STRATEGY_PARAMS_2002_H
#define STRATEGY_PARAMS_2002_H

//==================================================================
//==================================================================
//Random Strategy Params for 2002
class StrategyParams2002 : public ParameterOperations
{
public:
  //----------------------------------------------------------------
  //constructor, calls loadValues 
  StrategyParams2002();

  //opens param files for various omni types and fills structs with loaded values
  void loadValues();
  //----------------------------------------------------------------
  float GOALIE_OCCLUSION_THRESH;
  float GOALIE_FRONT_RADIUS;
  float BALL_VEL_THRESH_FOR_GOALIE;
  float BALL_VEL_THRESH_FOR_INTERCEPT;
  float GOALIE_ROAM_RADIUS;
  float GOALIE_OPTIMAL_PASS_DISTANCE;
  float GOALIE_MAX_HOLD_TIME;
  float GOALIE_TO_OPPONENT_DIST_THRESH;
  float GOALIE_MIN_LANE_WIDTH;
  float GOALIE_PASS_TIME_LIMIT;
  float OUR_PENALTY_ANGLE;
  float MIDFIELD_AREA_RADIUS;
  float MIDFIELD_SHIELD_BALL_DISTANCE;
  float MIDFIELD_MAX_STRIP_DISTANCE;
  float SPIN_TOL;
  float KICK_TOL;
  float PASS_TOL;
  float KICK_VELOCITY;
  float MAX_ACCEL;
  float PENALTY_SHOT_ANGLE;
  float SHOOT_THRESH_LINE;
  float CLEAR_UPFIELD_THRESH_LINE;
  float CLEAR_UPFIELD_TARGET_LINE;
  float RUN_PAST_BALL_DISTANCE;
  float SLOW_STRIP_VELOCITY;
  float CLEAR_UPFIELD_LINE;
  float FAR_FORWARD_LINE;
  float SIDELINE_DIVIDER;
  float SIDELINE_Y_RECEIVE_PASS;
  float DIST_TO_PASS_DEST;
  float PASS_LANE_THRESH;
  float PASS_LANE_ANGLE_THRESH;
  float SHOOT_LANE_THRESH;
  float MIN_GOALIE_DIST;
  float PASS_LENGTHS_RATIO;
  float PASS_COMBO_MAX_LENGTH;
  float PASS_COMBO_MIN_LENGTH;
  float PASS_COMBO_MAX_ANGLE;
  float PASS_COMBO_CLEARANCE_THRESH;
  float PASS_SHOT_MAX_LENGTH;
  float PASS_SHOT_MIN_LENGTH;
  float PASS_SHOT_MAX_ANGLE;
  float PASS_SHOT_CLEARANCE_THRESH;
  float INTERCEPTION_SLOW_BALL_SPEED;
  float INTERCEPTION_DELAY_PARAM;
  float INTERCEPTION_DELAY_OFFSET;
  float CORNER_KICK_TIME_LIMIT;
  float CORNER_DIST;
  float LEFT_CORNER_POINT_1X;
  float LEFT_CORNER_POINT_1Y;
  float LEFT_CORNER_POINT_2X;
  float LEFT_CORNER_POINT_2Y;
  float LEFT_CORNER_POINT_3X;
  float LEFT_CORNER_POINT_3Y;
  float LEFT_CORNER_POINT_4X;
  float LEFT_CORNER_POINT_4Y;
  float LEFT_CORNER_POINT_5X;
  float LEFT_CORNER_POINT_5Y;
  float LEFT_CORNER_POINT_6X;
  float LEFT_CORNER_POINT_6Y;
  float LEFT_CORNER_POINT_7X;
  float LEFT_CORNER_POINT_7Y;
  float RIGHT_CORNER_POINT_1X;
  float RIGHT_CORNER_POINT_1Y;
  float RIGHT_CORNER_POINT_2X;
  float RIGHT_CORNER_POINT_2Y;
  float RIGHT_CORNER_POINT_3X;
  float RIGHT_CORNER_POINT_3Y;
  float RIGHT_CORNER_POINT_4X;
  float RIGHT_CORNER_POINT_4Y;
  float RIGHT_CORNER_POINT_5X;
  float RIGHT_CORNER_POINT_5Y;
  float RIGHT_CORNER_POINT_6X;
  float RIGHT_CORNER_POINT_6Y;
  float RIGHT_CORNER_POINT_7X;
  float RIGHT_CORNER_POINT_7Y;
  float STEP1COUNTER;
  float STEP2COUNTER;
  float STEP3COUNTER;
  float STEP4COUNTER;
  float STEP5COUNTER;
  float STEP6COUNTER;
  float STEP7COUNTER;
  int TEST_METHOD;
  float TEST_SPEED;
  float TEST_ROTATION;
  float TEST_ROT_VEL;
  float ENABLE_SIMPLE_OFFENSE;
  float USE_CLEAR_UPFIELD;
  float USE_CORNER_KICK;
  float USE_DIRECT_KICK;
  float USE_UPFIELD_PASSING;

  float MIN_PASS_DEST;
  float PASS_TO_SHOT_DISTANCE;
  
  //----------------------------------------------------------------
  int ENABLE_BACK_PASS;
  int ENABLE_CREATOR_ASSIST;
  int ENABLE_OPPOSITE_COVER_SKILL;
  int ENABLE_FOLLOW_DEFNDER_SKILL;
  int ENABLE_SMART_ROTATE_AROUND_BALL;
  int ENABLE_AGGRESSIVE_PULL_OFF_WALL;
  int ENABLE_AGGRESSIVE_PULL_OFF_CORNER;
  int JAM_AND_SHOOT_MODE;
  int ENABLE_CAREFUL_STRIP_BALL;
  //----------------------------------------------------------------
};
//==================================================================
//==================================================================

#endif //STRATEGY_PARAMS_2002_H

