//####################################################
//###  Shift location from neg<->pos range         ###
//###  to zero<->2*pos range                       ###
//###  This is because matrices must have postive  ###
//###  coordinates                                 ###
//####################################################

#include "find_open_space.h"
#include "ai.h" 
#include "info/robot/robot.h"

Pair shiftPoint(const float ourGoalLine,
                const float rightSideLine,
                const float realWorldPointX,
                const float realWorldPointY)
{
  //This will contain the new point, which has been
  //shifted to always contain positive coordinates
  Pair shiftWorldPoint;

  //
  //            LEFT SIDE LINE (+y)
  //        ___________________________
  //       |                           |
  // OUR   |                           |    THEIR GOAL
  // GOAL__|                           |___ (+x)
  //    |                                  |
  //    |                                  |
  //    |__                             ___|
  // (-x)  |                           |
  //       |                           |
  //       |___________________________|
  //
  //             RIGHT SIDE LINE (-y)
  //

  //Basically, add abs negative component to
  //current component to get shifted value.

  //So (-x,-y) -> (0,0) and (x,y) -> (2x,2y)
  shiftWorldPoint.setX(realWorldPointX + ABS(ourGoalLine));
  shiftWorldPoint.setY(realWorldPointY + ABS(rightSideLine));

  //Return shifted point
  return shiftWorldPoint;
}

//####################################################
//###  UnShift location from zero<->2*pos range    ###
//###  to neg<->pos range                          ###
//###  This is because matrices must have postive  ###
//###  coordinates                                 ###
//####################################################
Pair unshiftPoint(const float ourGoalLine,
                  const float rightSideLine,                  
                  const float shiftedWorldPointX,
                  const float shiftedWorldPointY)
{
  //This will contain unshifted point, which has
  //coodiantes ranging from negative<->positive
  Pair realWorldPoint;

  //Basically, subtract abs negative component to
  //current component to get unshifted value.

  //So (0,0) -> ()-x,-y) and (2x,2y) -> (x,y)

  realWorldPoint.setX(shiftedWorldPointX - ABS(ourGoalLine));
  realWorldPoint.setY(shiftedWorldPointY - ABS(rightSideLine));

  //Return unshifted point
  return realWorldPoint;
}

//##############################################################
//###  determineOpenPoint is the main algorithm. Given       ###
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
//###                                                        ###
//###                                                        ###
//##############################################################

//================Static Variables=================

//Store boundries of board when split in half
static int xMin1, xMax1, xMin2, xMax2;
static int yMin1, yMax1, yMin2, yMax2;

//Store sums of each half of bord. Used to determine
//which half to recurse on
static int sum1, sum2;

//===============End Static Variables==============

void determineOpenPoint(board& abstractBoard,
                        int xMin,
                        int xMax,
                        int yMin,
                        int yMax,
                        Pair& openPoint,
                        bool& noObstacles)
{
  //--------------------------------
  //Terminal case 1: single unit
  //If we are down to a single
  //unit squre, return it
  //--------------------------------
  if(
     ((xMax - xMin) == 0) &&
     ((yMin - yMin) == 0)
    )
  {
    noObstacles = true;
    openPoint.setX((float)xMax);
    openPoint.setY((float)yMax);
    return;
  }

  //--------------------------
  //Else, split board on longer half
  //--------------------------

  //==========================================
  //If longer on x-dim, split along x-axis
  //==========================================
  if(
     (xMax - xMin) >=
     (yMax - yMin)
    )
  {
    //----------------------
    //first half
    xMin1 = xMin;
    xMax1 = ((xMin + xMax) / 2);
    yMin1 = yMin;
    yMax1 = yMax;
    //----------------------
    //second half
    xMin2 = xMax1 + 1;
    xMax2 = xMax;
    yMin2 = yMin;
    yMax2 = yMax;
    //----------------------
  }
  //==========================================
  //Else, longer on y-dim, split along y-axis
  //==========================================
  else
  {
    //----------------------
    //first half
    xMin1 = xMin;
    xMax1 = xMax;
    yMin1 = yMin;
    yMax1 = ((yMin + yMax) / 2);
    //----------------------
    //second half
    xMin2 = xMin;
    xMax2 = xMax;
    yMin2 = yMax1 + 1;
    yMax2 = yMax;
    //----------------------
  }

  //---------------------------------
  //Sum up mass in each half of board
  //---------------------------------
  //Initialize both sums to be empty
  sum1 = 0;
  sum2 = 0;

  //==========================================
  //Sum up first half...
  for(int x=xMin1; x<=xMax1; x++)
  {
    for(int y=yMin1; y<=yMax1; y++)
    {
      sum1 = sum1 + abstractBoard.Field[x][y];
    }
  }
  //==========================================
  //Sum up second half...
  for(x=xMin2; x<=xMax2; x++)
  {
    for(int y=yMin2; y<=yMax2; y++)
    {
      sum2 = sum2 + abstractBoard.Field[x][y];
    }
  }

  //---------------------------------------
  //Check both sums, recurse on half
  //with smaller sum, iff ((sum1+sum2) !=0)
  //---------------------------------------
  //If entire board is empty, return midpoint
  if((sum1 + sum2) == 0.0f)
  {
    noObstacles = true;
    openPoint.setX( (float)(xMin + xMax) / 2 );
    openPoint.setY( (float)(yMin + yMax) / 2);
    return;
  }
  //---------------------------------------
  //Else, pick smaller half, and recurse...
  {
    //If sum1 is smaller, we guess
    //the first half of board has fewer obstacles,
    //so recurse on it...
    if(sum1 < sum2)
      determineOpenPoint(abstractBoard,
                         xMin1,
                         xMax1,
                         yMin1,
                         yMax1,
                         openPoint,
                         noObstacles);
    //Else, other half we guess
    //has fewer obstacles, so recurse
    //on it...
    else
      determineOpenPoint(abstractBoard,
                         xMin2,
                         xMax2,
                         yMin2,
                         yMax2,
                         openPoint,
                         noObstacles);

    //clearly the sum in the entire region is not 0 so 
    //some obstacles were found so overwite returned value to false because
    //in this level fo the recursion we did find some obstacles
    noObstacles = false;
    return;
  }
}















//----------------------------------
//-----The Actual Algorithm---------
//-----Finds open space in zone-----
//----------------------------------
//      ____
//  ___|    |____ Their Goal
// |            |
// | TL      TR |
// |            |
// |            |   
// |            |
// |            |
// | BL      BR |
// |___      ___|
//     |____|     Our Goal
//
//This imagine putting a grid over the field. The Field_Left is the left side of the above picture,
//Field_Right is the right side, Field_Top is the farthest point at the top of the field (by their goal)
//and Field_Bottom is the closest you can get to our goal by the wall.
//
//Columns and Rows basically determine how acturately you want the algorithm the be. It is going to put a grid over
//the field so Rows and Columns determine the vertical and horizontal resolution respectfully.
//
//------>    Assume the BOTTOM RIGHT is (0,0)    <------
//Why this corner?
//Short Answer: I felt like it. And it is easy to go from a coordinate plane to that. =-)
//Long Answer: At any the Field_Bottom might be positive OR NEGATIVE, so their is no reason to have (0,0) on the top
//left.
//Final Anser: Deal with it. =-) No matter which values are postive and negative, the algorithm will deal with that.
//
//Column_Left and Column_Right determine the left and right bounds to be searched and
//Row_Top and Row_Bottom determine the top and bottom bounds to be searched, respectfully.
//
//Obstacles is this nify array of nine objects (5 enemy robots, 4 of your friendly teammates) that you want
//to avoid. Why? You can't go through any of them, and don't assume any will get out of the away, so you have
//to consider your friends as well. However, you are never an obstacle to yourself (DUH!) so don't worry about
//yourself. Note: It makes NO differnt the order of the obstacles, since they are all obstacles. =-)
//
//Finally, the method returns a location which you should go to. It is the determined most open place to go to in the
//range specified.

//Used for placing blips
static int Blip[37][3] = {
                          //(X, Y, Value)
						              
                          //The 2's
                          {-1, 3, 2}, 
                          {0, 3, 2},
                          {1, 3, 2},
                          {2, 2, 2},
                          {3, 1, 2},
                          {3, 0, 2},
                          {3, -1, 2},
                          {2, -2, 2},
                          {1, -3, 2},
                          {0, -3, 2},
                          {-1, -3, 2},
                          {-2, -2, 2},
                          {-3, -1, 2},
                          {-3, 0, 2},
                          {-3, 1, 2},
                          {-2, 2, 2},
                          //The 5's
                          {-1, 2, 5},
                          {0, 2, 5},
                          {1, 2, 5},
                          {2, 1, 5},
                          {2, 0, 5},
                          {2, -1, 5},
                          {1, -2, 5},
                          {0, -2, 5},
                          {-1, -2, 5},
                          {-2, -1, 5},
                          {-2, 0, 5},
                          {-2, 1, 5},
                          //The 7's
                          {-1, 1, 7},
                          {0, 1, 7},
                          {1, 1, 7},
                          {1, 0, 7},
                          {1, -1, 7},
                          {0, -1, 7},
                          {-1, -1, 7},
                          {-1, 0, 7},
                          //The 9 (the center)
                          {0,0, 9}
                         };

//----------------------------------
//-----The Actual Algorithm---------
//-----Finds open space in zone-----
//----------------------------------
void findOpenSpace(float fieldLeft, 
                   float fieldRight, 
                   float fieldTop, 
                   float fieldBottom,
                   //boundaries in the matrix we actually care about =-)
                   //format: (min->max along width
                   //         min->max along length
                   int xMin,
                   int xMax,
                   int yMin,
                   int yMax,
                   Pair obstacles[9],
                   Destination* command,
                   bool& noObstacles)
{
	//-------------------------------------------------
  //Make the field matrix
	board playing;
	//-------------------------------------------------
  //Initialize the entire field to 0 (aka open space)
	for(int x=0; x<Columns; x++)
	{
		for(int y=0; y<Rows; y++)
		{
			playing.Field[x][y] = 0;
		}
	}
	//-------------------------------------------------
	Pair shiftedObject;//Contains the shifted point, not scaled for matrix.
	Pair scaledObject; //Contains the scaled object ready for putting on the matrix.
	//Contains the shifted top-right corner of the field. Used for scaling purposes.

  //This is the max (pos,pos) corner that will be used for scaling below
  Pair leftSideTheirCornerMax = shiftPoint(fieldBottom,
                                           fieldRight,
                                           fieldTop,
                                           fieldLeft);
  //-----------------------------------
	//Propigate the field with objects represented with potive values.
		for(int i=0; i<9; i++)
		{
			if(obstacles[i].getX() != -32000.0)//if it is found
			{
				//For each obstacle:
				//------------------
				
				//Shift point (coordinates are flipped on purpose)
				shiftedObject = shiftPoint(fieldBottom,
                                   fieldRight,
                                   obstacles[i].getX(),
                                   obstacles[i].getY());
          				
				//Scale each point so it is an integer that falls in matrix dimentions.
				//----------------------------------------------------------------------
				scaledObject.setX( 
                          (
                           shiftedObject.getX() / 
                           leftSideTheirCornerMax.getX()
                          ) * 
                          (Columns - 1) 
                         );
        scaledObject.setY( 
                          (
                           shiftedObject.getY() / 
                           leftSideTheirCornerMax.getY()
                          ) * 
                          (Rows - 1) 
                         );
				//--------------------------------------------------------
        //Place circle on matrix, with origin at the Scaled_Point.
				//--------------------------------------------------------
				//
				//   222
				//  25552
				// 2577752
				// 2579752
				// 2577752
				//  25552
				//   222
				//
				//9 is center of object. Positive values will be seen as "bad." =-)
				//37 points in all		
				for(int point = 0; point < 37; point++)
				{
					//For each blip
					int X, Y, Value;
					X = ((int)(scaledObject.getX())) + Blip[point][0];
					Y = ((int)(scaledObject.getY())) + Blip[point][1];
					Value = Blip[point][2];
					
					//If blip is in matrix put it
					if((X >= 0) && (X < Columns) && (Y >= 0) && (Y < Rows))
					{
						(playing.Field)[X][Y] += Value;
          }
				}
				//Done putting obstacle
			}
		}
	//Done putting objects

	//Ok, matrix is now ready for inspection. Use the starting range to start the search.
	Pair bestPoint;

  //ok, find the most open point in the range! =-)
	determineOpenPoint(playing, 
                     xMin, 
                     xMax, 
                     yMin, 
                     yMax,
                     bestPoint,
                     noObstacles);

	//Yeah, we know have the point on the field that is the most clear. At least we hope. But this is the
	//translated point. Multiple and translate back to get the point in the original dimension.

	//First scale back...
	float shiftedDestinationX = (
                               bestPoint.getX() / 
                               (Columns-1) *
                               leftSideTheirCornerMax.getX()
                              );

  float shiftedDestinationY = (
                               bestPoint.getY() / 
                               (Rows - 1) *
                               leftSideTheirCornerMax.getY()
                              );

	//Finally, shift back into place and return!
  command->setPos( unshiftPoint(fieldBottom, 
                                fieldRight, 
                                shiftedDestinationX, 
                                shiftedDestinationY));
 
}


//-------------------------------------------------
//******************************************
//******************************************
//************Find Open Space***************
//******************************************
//******************************************

static Obstacles theObstacles;
//---------------------------------------------------------
//Use this algorithm to send robots into open space
//---------------------------------------------------------
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
                            RobotIndex screeningRobot)
{
  //----------------------------------------------
  // First shift coordinates of 
  // where we are looking into geq (greater than
  // or equal to zero) format
  //----------------------------------------------
  //This is the max (pos,pos) corner that will be used for scaling below
  Pair leftSideTheirCornerMax = shiftPoint(params.field.OUR_GOAL_LINE,
                                           params.field.RIGHT_SIDE_LINE,
                                           params.field.THEIR_GOAL_LINE,
                                           params.field.LEFT_SIDE_LINE);
  //-----------------------------------
  //left sideline, their goaline corner
  //-> this is the (pos,pos) corner of the field  
  Pair leftSideTheirCorner= shiftPoint(params.field.OUR_GOAL_LINE, 
                                       params.field.RIGHT_SIDE_LINE,
                                       topBoundary,
                                       leftBoundary);
  //-----------------------------------
	//right sidline, our goaline corner
  //-> this is the (neg,neg) corner of the field
  Pair rightSideOurCorner= shiftPoint(params.field.OUR_GOAL_LINE, 
                                      params.field.RIGHT_SIDE_LINE,
                                      bottomBoundary,
                                      rightBoundary);
  //-------------------------------------------------
	//Then scale the boundaries into matrix coordiantes
  //-------------------------------------------------
	Pair scaledLeftSideTheirCorner;
	scaledLeftSideTheirCorner.setX( 
                                 (
                                  leftSideTheirCorner.getX() / 
                                  leftSideTheirCornerMax.getX()
                                 ) * 
                                 (Columns - 1) 
                                );
	scaledLeftSideTheirCorner.setY( 
                                 (
                                  leftSideTheirCorner.getY() / 
                                  leftSideTheirCornerMax.getY()
                                 ) * 
                                 (Rows - 1) 
                                );
  //-----------------------------------
	Pair scaledRightSideOurCorner;
	scaledRightSideOurCorner.setX( 
                                (
                                 rightSideOurCorner.getX() / 
                                 leftSideTheirCornerMax.getX()
                                ) * 
                                (Columns - 1) 
                               );
	scaledRightSideOurCorner.setY( 
                                (
                                 rightSideOurCorner.getY() / 
                                 leftSideTheirCornerMax.getY()
                                ) * 
                                (Rows - 1) 
                               );
  //-----------------------------------
	//ok, scaledLeftSideTheirCorner and scaledRightSideOurCorner 
  //now delimit the RANGE in the matrix that we care about.
  //That's nice. next construct the array of obstacles, and make sure
  //not to include a friendly screening robot. he's our friend. =-)
  //-----------------------------------
	//Make array of obstacles
  findObstacles(field,
                params,
                ID,
                theObstacles.getObstacleArray());
	
  //if there is a screening robot, remove him!
  if(screeningRobot != NO_ROBOT)
  {
    //if his id is less than our id, then ignore his id
    //if his id is greater than our id, since we don't count, ignore his id-1
    //this is because we have already been taken out of the obstacle array
    if(screeningRobot < ID)
      theObstacles.ignoreFriend(screeningRobot);
    else
      theObstacles.ignoreFriend(screeningRobot-1); 
  }  
	//---------------------------------------------------------
	//Ok, find the open space =-)

  findOpenSpace(//dimensions of the field
                params.field.LEFT_SIDE_LINE,
                params.field.RIGHT_SIDE_LINE,
                params.field.THEIR_GOAL_LINE,
                params.field.OUR_GOAL_LINE,
                //boundaries in the matrix we actually care about =-)
                //format: (min->max along length
                //         min->max along width
                (int)scaledRightSideOurCorner.getX(),
                (int)scaledLeftSideTheirCorner.getX(),
                (int)scaledRightSideOurCorner.getY(),
                (int)scaledLeftSideTheirCorner.getY(),
                //obstacle array
                theObstacles.getObstacleArray(),
                //ultimate destination
                command,
                noObstacles);
}
