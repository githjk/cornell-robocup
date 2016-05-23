#include "FieldParams.h"
#include "assert.h"
#include <stdio.h>
//for SQRT
#include "common/util/math/cmath.h"
//==================================================================
//==================================================================
//constructor, calls loadValues 
FieldParams::FieldParams()
{
  loadValues();
}
//==================================================================
//==================================================================
//load field params
void FieldParams::loadValues()
{
  //-----------------------------------------
  ifstream mainFile;
  mainFile.open("Params/FieldParams.txt", ios::nocreate);
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN Params/FieldParams.txt!");
  //-------------------
  READ_FLOAT(OUR_GOAL_LINE);
  READ_FLOAT(THEIR_GOAL_LINE);
  READ_FLOAT(OUR_LEFT_GOAL_WALL);
  READ_FLOAT(OUR_RIGHT_GOAL_WALL);
  READ_FLOAT(THEIR_LEFT_GOAL_WALL);
  READ_FLOAT(THEIR_RIGHT_GOAL_WALL);
  READ_FLOAT(RIGHT_SIDE_LINE);
  READ_FLOAT(RIGHT_GOALIE_BOX);
  READ_FLOAT(RIGHT_GOAL_POST);
  READ_FLOAT(LEFT_GOAL_POST);
  READ_FLOAT(LEFT_GOALIE_BOX);
  READ_FLOAT(LEFT_SIDE_LINE);
  READ_FLOAT(GOAL_WIDTH);
  READ_FLOAT(GOALIE_LENGTH);
  READ_FLOAT(GOAL_DEPTH);
  READ_FLOAT(GOALIEBOX_DEPTH);
  READ_FLOAT(CENTER_CIRCLE_RADIUS);
  READ_FLOAT(FREE_KICK_X_OFFSET);
  READ_FLOAT(FREE_KICK_Y_OFFSET);
  READ_FLOAT(PENALTY_KICK_X_OFFSET);

  READ_FLOAT(OUR_LEFT_X);
  READ_FLOAT(OUR_LEFT_Y);
  READ_FLOAT(OUR_RIGHT_X);
  READ_FLOAT(OUR_RIGHT_Y);
  READ_FLOAT(THEIR_LEFT_X);
  READ_FLOAT(THEIR_LEFT_Y);
  READ_FLOAT(THEIR_RIGHT_X);
  READ_FLOAT(THEIR_RIGHT_Y);
  READ_FLOAT(CENTER_X);
  READ_FLOAT(CENTER_Y);

  //--------------------------------------
  HALF_LINE = (OUR_GOAL_LINE + THEIR_GOAL_LINE)/2.0f;
  SPLIT_LINE = (LEFT_SIDE_LINE + RIGHT_SIDE_LINE)/2.0f;
  OUR_GOALIE_BOX = OUR_GOAL_LINE + GOALIEBOX_DEPTH;
  THEIR_GOALIE_BOX = THEIR_GOAL_LINE - GOALIEBOX_DEPTH;
  FIELD_LENGTH = THEIR_GOAL_LINE - OUR_GOAL_LINE;
  FIELD_WIDTH = LEFT_SIDE_LINE - RIGHT_SIDE_LINE;
  FIELD_DIAGONAL = SQRT(FIELD_WIDTH*FIELD_WIDTH + FIELD_LENGTH*FIELD_LENGTH);
  //-------------------------------------
  //2002 strategy lines =-)
  //--
  READ_FLOAT(DEATH_ZONE_LINE_OFFSET);
  DEATH_ZONE_LINE = OUR_GOAL_LINE + DEATH_ZONE_LINE_OFFSET;
  //--
  READ_FLOAT(DEFENSE_ZONE_LINE_OFFSET);
  DEFENSE_ZONE_LINE = DEATH_ZONE_LINE + DEFENSE_ZONE_LINE_OFFSET;
  //--
  READ_FLOAT(KILL_ZONE_LINE_OFFSET);
  KILL_ZONE_LINE = THEIR_GOAL_LINE - KILL_ZONE_LINE_OFFSET;
  //--
  READ_FLOAT(OFFENSE_ZONE_LINE_OFFSET);
  OFFENSE_ZONE_LINE = KILL_ZONE_LINE - OFFENSE_ZONE_LINE_OFFSET;
  //--
  READ_FLOAT(LEFT_MIDDLE_SECTOR_OFFSET);
  LEFT_MIDDLE_SECTOR = LEFT_GOAL_POST - LEFT_MIDDLE_SECTOR_OFFSET;
  //--
  READ_FLOAT(RIGHT_MIDDLE_SECTOR_OFFSET);
  RIGHT_MIDDLE_SECTOR = RIGHT_GOAL_POST + RIGHT_MIDDLE_SECTOR_OFFSET;
  //--
  READ_FLOAT(LEFT_SWEET_SECTOR_OFFSET);
  LEFT_SWEET_SECTOR = LEFT_SIDE_LINE - LEFT_SWEET_SECTOR_OFFSET;
  //--
  READ_FLOAT(RIGHT_SWEET_SECTOR_OFFSET);
  RIGHT_SWEET_SECTOR = RIGHT_SIDE_LINE + RIGHT_SWEET_SECTOR_OFFSET;
  //--------------------------------------
  mainFile.close(); 
  //-----------------------------------------
}
//==================================================================
//==================================================================
//save field params
void FieldParams::saveValues()
{
  //-----------------------------------------
  //create a file pointer
  FILE *parameterFile;
  //-----------------------------------------
  //attempt to create the file based on the filename provided
  //if unsuccessful, bail out
  parameterFile = fopen("Params/FieldParams.txt","w");
  if (parameterFile == NULL)
  {
    printf("Error creating Params/FieldParams.txt :( \n");
    return;
  }
  //-----------------------------------------
  fprintf(parameterFile, "\n####################\n");
  fprintf(parameterFile, "### Field Params ###\n");
  fprintf(parameterFile, "####################\n");
  //--------
  fprintf(parameterFile, "\nOUR_GOAL_LINE ");
  fprintf(parameterFile, "%f\n", OUR_GOAL_LINE);
  //--------
  fprintf(parameterFile, "\nTHEIR_GOAL_LINE ");
  fprintf(parameterFile, "%f\n", THEIR_GOAL_LINE);
  //--------
  fprintf(parameterFile, "\n# pretty much the same as OUR_GOAL_LINE, but used for goalie\n");
  fprintf(parameterFile, "OUR_LEFT_GOAL_WALL ");
  fprintf(parameterFile, "%f\n", OUR_LEFT_GOAL_WALL);
  //--------
  fprintf(parameterFile, "\nOUR_RIGHT_GOAL_WALL ");
  fprintf(parameterFile, "%f\n", OUR_RIGHT_GOAL_WALL);
  //--------
  fprintf(parameterFile, "\nTHEIR_LEFT_GOAL_WALL ");
  fprintf(parameterFile, "%f\n", THEIR_LEFT_GOAL_WALL);
  //--------
  fprintf(parameterFile, "\nTHEIR_RIGHT_GOAL_WALL ");
  fprintf(parameterFile, "%f\n", THEIR_RIGHT_GOAL_WALL);
  //--------
  fprintf(parameterFile, "\nRIGHT_SIDE_LINE ");
  fprintf(parameterFile, "%f\n", RIGHT_SIDE_LINE);
  //--------
  fprintf(parameterFile, "\nRIGHT_GOALIE_BOX ");
  fprintf(parameterFile, "%f\n", RIGHT_GOALIE_BOX);
  //--------
  fprintf(parameterFile, "\nRIGHT_GOAL_POST ");
  fprintf(parameterFile, "%f\n", RIGHT_GOAL_POST);
  //--------
  fprintf(parameterFile, "\nLEFT_GOAL_POST ");
  fprintf(parameterFile, "%f\n", LEFT_GOAL_POST);
  //--------
  fprintf(parameterFile, "\nLEFT_GOALIE_BOX ");
  fprintf(parameterFile, "%f\n", LEFT_GOALIE_BOX);
  //--------
  fprintf(parameterFile, "\nLEFT_SIDE_LINE ");
  fprintf(parameterFile, "%f\n", LEFT_SIDE_LINE);
  //--------
  fprintf(parameterFile, "\nGOAL_WIDTH ");
  fprintf(parameterFile, "%f\n", GOAL_WIDTH);
  //--------
  fprintf(parameterFile, "\nGOALIE_LENGTH ");
  fprintf(parameterFile, "%f\n", GOALIE_LENGTH);
  //--------
  fprintf(parameterFile, "\nGOAL_DEPTH ");
  fprintf(parameterFile, "%f\n", GOAL_DEPTH);
  //--------
  fprintf(parameterFile, "\nGOALIEBOX_DEPTH ");
  fprintf(parameterFile, "%f\n", GOALIEBOX_DEPTH); 
  //--------
  fprintf(parameterFile, "\nCENTER_CIRCLE_RADIUS ");
  fprintf(parameterFile, "%f\n", CENTER_CIRCLE_RADIUS); 
  //--------
  fprintf(parameterFile, "\nFREE_KICK_X_OFFSET ");
  fprintf(parameterFile, "%f\n", FREE_KICK_X_OFFSET); 
  //--------
  fprintf(parameterFile, "\nFREE_KICK_Y_OFFSET ");
  fprintf(parameterFile, "%f\n", FREE_KICK_Y_OFFSET); 
  //--------
  fprintf(parameterFile, "\nPENALTY_KICK_X_OFFSET ");
  fprintf(parameterFile, "%f\n", PENALTY_KICK_X_OFFSET); 
  //--------  
  fprintf(parameterFile, "\nOUR_LEFT_X ");
  fprintf(parameterFile, "%f\n", OUR_LEFT_X); 
  //--------
  fprintf(parameterFile, "\nOUR_LEFT_Y ");
  fprintf(parameterFile, "%f\n", OUR_LEFT_Y); 
  //--------
  fprintf(parameterFile, "\nOUR_RIGHT_X ");
  fprintf(parameterFile, "%f\n", OUR_RIGHT_X); 
  //--------
  fprintf(parameterFile, "\nOUR_RIGHT_Y ");
  fprintf(parameterFile, "%f\n", OUR_RIGHT_Y); 
  //--------
  fprintf(parameterFile, "\nTHEIR_LEFT_X ");
  fprintf(parameterFile, "%f\n", THEIR_LEFT_X); 
  //--------
  fprintf(parameterFile, "\nTHEIR_LEFT_Y ");
  fprintf(parameterFile, "%f\n", THEIR_LEFT_Y); 
  //--------
  fprintf(parameterFile, "\nTHEIR_RIGHT_X ");
  fprintf(parameterFile, "%f\n", THEIR_RIGHT_X); 
  //--------
  fprintf(parameterFile, "\nTHEIR_RIGHT_Y ");
  fprintf(parameterFile, "%f\n", THEIR_RIGHT_Y); 
  //--------
  fprintf(parameterFile, "\nCENTER_X ");
  fprintf(parameterFile, "%f\n", CENTER_X); 
  //--------
  fprintf(parameterFile, "\nCENTER_Y ");
  fprintf(parameterFile, "%f\n", CENTER_Y); 
  //--------  
  fprintf(parameterFile, "\nDEATH_ZONE_LINE_OFFSET ");
  fprintf(parameterFile, "%f\n", DEATH_ZONE_LINE_OFFSET); 
  //--------
  fprintf(parameterFile, "\nDEFENSE_ZONE_LINE_OFFSET ");
  fprintf(parameterFile, "%f\n", DEFENSE_ZONE_LINE_OFFSET); 
  //--------
  fprintf(parameterFile, "\nKILL_ZONE_LINE_OFFSET ");
  fprintf(parameterFile, "%f\n", KILL_ZONE_LINE_OFFSET); 
  //--------
  fprintf(parameterFile, "\nOFFENSE_ZONE_LINE_OFFSET ");
  fprintf(parameterFile, "%f\n", OFFENSE_ZONE_LINE_OFFSET); 
  //--------
  fprintf(parameterFile, "\nLEFT_MIDDLE_SECTOR_OFFSET ");
  fprintf(parameterFile, "%f\n", LEFT_MIDDLE_SECTOR_OFFSET); 
  //--------
  fprintf(parameterFile, "\nRIGHT_MIDDLE_SECTOR_OFFSET ");
  fprintf(parameterFile, "%f\n", RIGHT_MIDDLE_SECTOR_OFFSET); 
  //--------
  fprintf(parameterFile, "\nLEFT_SWEET_SECTOR_OFFSET ");
  fprintf(parameterFile, "%f\n", LEFT_SWEET_SECTOR_OFFSET); 
  //--------
  fprintf(parameterFile, "\nRIGHT_SWEET_SECTOR_OFFSET ");
  fprintf(parameterFile, "%f\n", RIGHT_SWEET_SECTOR_OFFSET); 
  //======================================================
  fclose(parameterFile);
  //-----------------------------------------
}
//==================================================================
//==================================================================
void FieldParams::switchSides()
{
  float temp;
  //--------------------------------------
  temp = RIGHT_GOAL_POST;
  RIGHT_GOAL_POST = -LEFT_GOAL_POST;
  LEFT_GOAL_POST = -temp;

  temp = RIGHT_SIDE_LINE;
  RIGHT_SIDE_LINE = -LEFT_SIDE_LINE;
  LEFT_SIDE_LINE = -temp;

  temp = RIGHT_GOALIE_BOX;
  RIGHT_GOALIE_BOX = -LEFT_GOALIE_BOX;
  LEFT_GOALIE_BOX = -temp;

  temp = OUR_GOAL_LINE;
  OUR_GOAL_LINE = -THEIR_GOAL_LINE;
  THEIR_GOAL_LINE = -temp;

  temp = OUR_GOALIE_BOX;
  OUR_GOALIE_BOX = -THEIR_GOALIE_BOX;
  THEIR_GOALIE_BOX = -temp;

  temp = OUR_LEFT_GOAL_WALL;
  OUR_LEFT_GOAL_WALL = -THEIR_LEFT_GOAL_WALL;
  THEIR_LEFT_GOAL_WALL = -temp;
  
  temp = OUR_RIGHT_GOAL_WALL;
  OUR_RIGHT_GOAL_WALL = -THEIR_RIGHT_GOAL_WALL;
  THEIR_RIGHT_GOAL_WALL = -temp;
  //--------------------------------------
  HALF_LINE = (OUR_GOAL_LINE + THEIR_GOAL_LINE)/2.0f;
  SPLIT_LINE = (LEFT_SIDE_LINE + RIGHT_SIDE_LINE)/2.0f;
  OUR_GOALIE_BOX = OUR_GOAL_LINE + GOALIEBOX_DEPTH;
  THEIR_GOALIE_BOX = THEIR_GOAL_LINE - GOALIEBOX_DEPTH;
  FIELD_LENGTH = THEIR_GOAL_LINE - OUR_GOAL_LINE;
  FIELD_WIDTH = LEFT_SIDE_LINE - RIGHT_SIDE_LINE;
  FIELD_DIAGONAL = SQRT(FIELD_WIDTH*FIELD_WIDTH + FIELD_LENGTH*FIELD_LENGTH);
  //-------------------------------------
  DEATH_ZONE_LINE = OUR_GOAL_LINE + DEATH_ZONE_LINE_OFFSET;
  DEFENSE_ZONE_LINE = DEATH_ZONE_LINE + DEFENSE_ZONE_LINE_OFFSET;
  KILL_ZONE_LINE = THEIR_GOAL_LINE - KILL_ZONE_LINE_OFFSET;
  OFFENSE_ZONE_LINE = KILL_ZONE_LINE - OFFENSE_ZONE_LINE_OFFSET;
  LEFT_MIDDLE_SECTOR = LEFT_GOAL_POST - LEFT_MIDDLE_SECTOR_OFFSET;
  RIGHT_MIDDLE_SECTOR = RIGHT_GOAL_POST + RIGHT_MIDDLE_SECTOR_OFFSET;
  LEFT_SWEET_SECTOR = LEFT_SIDE_LINE - LEFT_SWEET_SECTOR_OFFSET;
  RIGHT_SWEET_SECTOR = RIGHT_SIDE_LINE + RIGHT_SWEET_SECTOR_OFFSET;
  //-------------------------------------
  OUR_LEFT_X = -THEIR_RIGHT_X;
  OUR_LEFT_Y = -THEIR_RIGHT_Y;

  OUR_RIGHT_X = -THEIR_LEFT_X;
  OUR_RIGHT_Y = -THEIR_LEFT_Y;

  THEIR_LEFT_X = -OUR_RIGHT_X;
  THEIR_LEFT_Y = -OUR_RIGHT_Y;

  THEIR_RIGHT_X = -OUR_LEFT_X;
  THEIR_RIGHT_Y = -OUR_LEFT_Y;
  //-------------------------------------
}
//==================================================================
//==================================================================
float FieldParams::getOurGoalLine(float y)
{
  return OUR_RIGHT_X + 
         (
           ( (y - OUR_RIGHT_Y) / (OUR_LEFT_Y - OUR_RIGHT_Y) ) * 
           (OUR_LEFT_X - OUR_RIGHT_X)
         );
}
//-------------------------------------
float FieldParams::getTheirGoalLine(float y)
{
  return THEIR_RIGHT_X + 
         (
           ( (y - THEIR_RIGHT_Y) / (THEIR_LEFT_Y - THEIR_RIGHT_Y) ) * 
           (THEIR_LEFT_X - THEIR_RIGHT_X)
         );
}
//-------------------------------------
float FieldParams::getLeftSideLine(float x)
{
  return OUR_LEFT_Y + 
         (
           ( (x - OUR_LEFT_X) / (THEIR_LEFT_X - OUR_LEFT_X) ) * 
           (THEIR_LEFT_Y - OUR_LEFT_Y)
         );
}
//-------------------------------------
float FieldParams::getRightSideLine(float x)
{
  return OUR_RIGHT_Y + 
         (
           ( (x - OUR_LEFT_X) / (THEIR_RIGHT_X - OUR_RIGHT_X) ) * 
           (THEIR_RIGHT_Y - OUR_RIGHT_Y)
         );
}
//-------------------------------------

