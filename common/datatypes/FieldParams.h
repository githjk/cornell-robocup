#include "ParameterOperations.h"
#ifndef FIELD_PARAMS_H
#define FIELD_PARAMS_H

//==================================================================
//==================================================================
//Field Boundaries
class FieldParams : public ParameterOperations
{
public:
  //----------------------------------------------------------------
  //constructor, calls loadValues 
  FieldParams();

  //opens param files for various omni types and fills structs with loaded values
  void loadValues();

  //saves current values to param file
  void saveValues();

  //flips all lines when switching sides
  void switchSides();
  //----------------------------------------------------------------
  float getOurGoalLine(float y);
  float getTheirGoalLine(float y);
  float getRightSideLine(float x);
  float getLeftSideLine(float x);
  float OUR_GOAL_LINE;
  float THEIR_GOAL_LINE;
  float OUR_LEFT_GOAL_WALL;
  float OUR_RIGHT_GOAL_WALL;
  float THEIR_LEFT_GOAL_WALL;
  float THEIR_RIGHT_GOAL_WALL;
  float RIGHT_SIDE_LINE;
  float RIGHT_GOALIE_BOX;
  float RIGHT_GOAL_POST;
  float LEFT_GOAL_POST;
  float LEFT_GOALIE_BOX;
  float LEFT_SIDE_LINE;
  float GOAL_WIDTH;
  float GOALIE_LENGTH;
  float GOAL_DEPTH;
  float GOALIEBOX_DEPTH;
  float HALF_LINE;
  float SPLIT_LINE;
  float OUR_GOALIE_BOX;
  float THEIR_GOALIE_BOX;
  float FIELD_LENGTH;
  float FIELD_WIDTH;
  float FIELD_DIAGONAL;
  float CENTER_CIRCLE_RADIUS;
  float FREE_KICK_X_OFFSET;
  float FREE_KICK_Y_OFFSET;
  float PENALTY_KICK_X_OFFSET;
  float DEATH_ZONE_LINE;
  float DEFENSE_ZONE_LINE;
  float KILL_ZONE_LINE;
  float OFFENSE_ZONE_LINE;
  float LEFT_MIDDLE_SECTOR;
  float RIGHT_MIDDLE_SECTOR;
  float LEFT_SWEET_SECTOR;
  float RIGHT_SWEET_SECTOR;

  float OUR_LEFT_X;
  float OUR_LEFT_Y;
  float OUR_RIGHT_X;
  float OUR_RIGHT_Y;
  float THEIR_LEFT_X;
  float THEIR_LEFT_Y;
  float THEIR_RIGHT_X;
  float THEIR_RIGHT_Y;
  float CENTER_X;
  float CENTER_Y;

private:
  float DEATH_ZONE_LINE_OFFSET;
  float DEFENSE_ZONE_LINE_OFFSET;
  float KILL_ZONE_LINE_OFFSET;
  float OFFENSE_ZONE_LINE_OFFSET;
  float LEFT_MIDDLE_SECTOR_OFFSET;
  float RIGHT_MIDDLE_SECTOR_OFFSET;
  float LEFT_SWEET_SECTOR_OFFSET;
  float RIGHT_SWEET_SECTOR_OFFSET;
  //----------------------------------------------------------------
};
//==================================================================
//==================================================================

#endif //FIELD_PARAMS_H

