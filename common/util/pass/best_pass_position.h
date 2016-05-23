/*
** best_pass_position.h
*/

#ifndef BEST_PASS_POSITION_H
#define BEST_PASS_POSITION_H

#include "base_types.h"
#include "info/robot/robot.h"

//====================================================================================

// This method finds the best location which is visible from two points on the field
// (say A & B) to receive a pass from A and pass the ball to B. An upper bound on the 
// sum of the lengths of the two passes is prescribed by the calling method. Also, a
// bound on the ratios of the two pass lengths is prescribed. Working within these
// two bounds, the method finds the best location on the field (downfield of the line 
// joining A & B) which avoids all obstacles (opponent robots and sidelines).
// Returns false if no suitable location can be found.
//
bool bestPassLocation (const Pair & passerLoc,          // Location of origin of the pass
                       const Pair & receiverLoc,        // Final pass destination
                       const float & maxPassLength,     // Max allowed sum of pass lengths
                       const float & minPassLength,     // Min allowed sum of pass lengths
                       const float & maxPassAngle,      // Max allowed angle between the two passes
                       const float & maxPassLengthRatio,// Max allowed pass length ratio (max/min)
                       const float & minLaneWidth,      // Minimum pass lane width required
                       const VisionData & field,	      // Where everyone else is now
	                   const SystemParameters & params,// Necessary game parameters
                       bool  ignoreOpponentGoalie,      // Ignore the opponent goalie?
                       Pair  * bestLocation);           // Suitable pass location  

float passLaneWidth (const Pair & passerLoc,        // Pass origin location
                     const Pair & receiverLoc,      // Pass destination
                     float obstacleCoords [5][2],   // Coords of all obstacles (maximum 5)
                     const int & numObstacles,      // Number of obstacles
                     const float & obstacleRadius); // Radius of an obstacle

//====================================================================================

#endif //BEST_PASS_POSITION_H
