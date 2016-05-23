/**
 * lane.h
 */

#ifndef LANE_H
#define LANE_H

#include "base_types.h"
#include "info/robot/robot.h"
#include "common/datatypes/SystemParameters.h"

//===============================================================================


//This function finds the distance from the pass line to the edge (not center) 
//of the closest enemy robot, and returns that distance.  This function is 
//really a modified form of isLane().
//
//@param passerLoc: location of passing robot
//@param passDestination: desired destination of the pass
//RETURNS: distance from line to edge of closest enemy robot
//         (returns 0.0f if robot is on line)
float laneHalfWidth ( 
                 Pair passerLoc,              // Position of the passing robot.
                 Pair passDestination, // location of the pass destination
                 const VisionData &field,     // where everyone else is now
                 const SystemParameters &rp, // contains necessary game params       
         bool checkOurRobots=false);// determines if we chech our robots or not...

//================================================================================



//This function returns the smallest angle between the pass  
//line and the ray tangent to an enemy robot.
//
//@param passerLoc: location of passing robot
//@param passDestination: desired destination of the pass
//@param *extraObstacle: Location of an optional extra obstacle, with raius equal to our robots
//RETURNS: angle from pass line to edge of closest enemy robot
//         (returns 0.0f if robot is on line)
float laneHalfAngle (Pair passerLoc, // Position of the passing robot.
                     Pair passDestination, // location of the pass destination
                     const VisionData &field, // where everyone else is now
                     const SystemParameters &rp, // contains necessary game parameters
                     const Pair * extraObstacle = (Pair *) 0, // Check this (optional) location as an obstacle too
                     bool checkOurRobots=false); // determines if we chech our robots or not...
#endif // LANE_H
