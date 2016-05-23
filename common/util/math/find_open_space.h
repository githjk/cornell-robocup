/**
 * find_open_space: a nasty alogorithm
 * that "determines" the most open spot
 * in a given range on the field
 */

#include "ai.h"
#include "obstacle.h"
#include "robocup_strategy_data.h"
//-------------------------------------------------------

//##############################################################
//### Want a robot to go into an open space? Call this       ###
//### function today! Left and Right boundry are on the      ###
//### Y-axis, topa dn bottom boundary are on the x-axis.     ###
//##############################################################
void sendRobotintoOpenSpace(const VisionData& field,
                            const SystemParameters& params,
                            //so we can set the final destination
                            Destination* command,
                            //boundaries of search...
                            //left and right like sidelines
                            float leftBoundary,
                            float rightBoundary,
                            //top is like their goal line
                            //bottom is like our goal line
                            float topBoundary,
                            float bottomBoundary,
                            //did we find any obstacles in the range?
                            bool& noObstacles,
                            //ID for this robot
                            RobotIndex ID,
                            //ignore this friendly robot
                            RobotIndex screeningRobot = NO_ROBOT);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~ You most likley never need to look below this line. The           ~~~~
//~~~~ above function does everything known to mankind, well, everything ~~~~
//~~~~ you'd ever want a robot to do. The below functions just help      ~~~~
//~~~~ implement it. =-)                                                 ~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//##################################################
//### Board is represented by matrix of integers ###
//### Bigger matrix -> better resolution -> more ###
//### "acurate" results, but slower computation  ###
//##################################################

//Columns refer to the x-axis,
//that is, along the length field
#define Columns 64

//Rows refer to the y-axis,
//that is, along the width of the field
#define Rows 32

//Struct to hold the board data
//0's represent pure open space.
//Larger integer values exist in
//centers of robots.
struct board
{
  int Field[Columns][Rows];
};

//################################################################
//### The findOpenSpace algorithm. give a field side,          ###
//### boundaries we care about, and an array of                ###
//### valid obstacles that we care about, sets the destination ###
//### of this robot in the most open space!                    ###
//################################################################
void findOpenSpace(//dimensions of the field
                   float fieldLeft, 
                   float fieldRight, 
                   float fieldTop, 
                   float fieldBottom,
                   //boundaries in the matrix we actually care about =-)
                   //format: (min->max along length
                   //         min->max along width
                   int minY,
                   int maxY,
                   int minX,
                   int maxX,
                   Pair obstacles[9],
                   Destination* command,
                   bool& noObstacles);


//##############################################################
//###  determineOpenPoint is the main recursive algorithm.   ###
//###  Given:                                                ###
//###  A matrix that represents the board with and           ###
//###  all obstacles, it recursively divides up the matrix   ###
//###  and zones in on a single board unit, the "most open"  ###
//###  unit.                                                 ###
//###                                                        ###
//###  Simply put, it divides the board in half, and         ###
//###  sums up all the values in each half. Which ever half  ###
//###  has a smaller sum, it considers to be more "open"     ###
//###  and recursively calls the alogorithm on that half.    ###
//###                                                        ###
//###  If the board ever has no objects in it (sum of both   ###
//###  halves are equal to 0) then algorithm halts, setting  ###
//###  the center of the current board to be the most open   ###
//###  point.                                                ###
//###                                                        ###
//###  column and row values mark range in matrix to search  ###
//###  openPoint stores final answer. noObstacles set true   ###
//###  if nothing in current matrix. Useful for dribbling    ###
//###  decisions in dribbling up the field.                  ###
//##############################################################
void determineOpenPoint(board& abstractBoard,
                        int xMin,
                        int xMax,
                        int yMin,
                        int yMax,
                        Pair& openPoint,
                        bool& noObstacles);

//####################################################
//###  Shift location from neg<->pos range         ###
//###  to zero<->2*pos range                       ###
//###  This is because matrices must have postive  ###
//###  coordinates                                 ###
//####################################################
Pair shiftPoint(const float ourGoalLine,
                const float rightSideLine,
                const float realWorldPointX,
                const float realWorldPointY);

//####################################################
//###  UnShift location from zero<->2*pos range    ###
//###  to neg<->pos range                          ###
//###  This is because matrices must have postive  ###
//###  coordinates                                 ###
//####################################################
Pair unshiftPoint(const float ourGoalLine,
                  const float rightSideLine,                  
                  const float shiftedWorldPointX,
                  const float shiftedWorldPointY);

//##############################################################
//###  determineOpenPoint is the main recursive algorithm.   ###
//###  Given:                                                ###
//###  A matrix that represents the board with and           ###
//###  all obstacles, it recursively divides up the matrix   ###
//###  and zones in on a single board unit, the "most open"  ###
//###  unit.                                                 ###
//###                                                        ###
//###  Simply put, it divides the board in half, and         ###
//###  sums up all the values in each half. Which ever half  ###
//###  has a smaller sum, it considers to be more "open"     ###
//###  and recursively calls the alogorithm on that half.    ###
//###                                                        ###
//###  If the board ever has no objects in it (sum of both   ###
//###  halves are equal to 0) then algorithm halts, setting  ###
//###  the center of the current board to be the most open   ###
//###  point.                                                ###
//###                                                        ###
//###  column and row values mark range in matrix to search  ###
//###  openPoint stores final answer. noObstacles set true   ###
//###  if nothing in current matrix. Useful for dribbling    ###
//###  decisions in dribbling up the field.                  ###
//##############################################################
void determineOpenPoint(board& abstractBoard,
                        int columnLeft,
                        int columnRight,
                        int rowTop,
                        int rowBottom,
                        Pair& openPoint,
                        bool& noObstacles);











