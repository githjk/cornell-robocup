/**
 * Math: Distance-finding functions
 */

#include "base_types.h"
#include "ai.h"
#include "math.h"
#include "info/ball/ball.h"
#include "common/datatypes/SystemParameters.h"

#ifndef DIST_H
#define DIST_H

//==========================================
inline float dist(int x1, 
                int y1,     
                int x2, 
                int y2) 
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  return SQRT( dx*dx + dy*dy );
}
//==========================================
inline float dist(float x1, 
                  float y1, 
                  float x2, 
                  float y2) 
{
  float dx = x2 - x1;
  float dy = y2 - y1;
  return SQRT( dx*dx + dy*dy );
}
//==========================================
inline double dist(double x1, 
                   double y1, 
                   double x2, 
                   double y2) 
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  return SQRT( dx*dx + dy*dy );
}
//==========================================
inline float dist(const Pair &a, 
		  const Pair &b)
{
  float dx = a.getX() - b.getX();
  float dy = a.getY() - b.getY();
  return SQRT( dx*dx + dy*dy );
}
//==========================================

// Finds distance between two objects
inline float dist(const MovingObject &a, const MovingObject &b)
{
  return dist( a.getPos(), b.getPos() );   // calls dist(Pair&, Pair&)
}

// Finds distance between two of our robots
inline float dist(RobotIndex a, RobotIndex b, 
		  const VisionData &field, const SystemParameters&params)
{
  return dist (
	       field.getRobot( params.general.TEAM, a ),
	       field.getRobot( params.general.TEAM, b )
	      );   // calls dist( MovingObject& , MovingObject& )
}

// Finds distance between our robot and an opponent
inline float distToOpponent(RobotIndex ourRobot, 
							RobotIndex opponentRobot, 
							const VisionData &field, 
							const SystemParameters&params)
{
  return dist (
	       field.getRobot( params.general.TEAM, ourRobot ),
	       field.getRobot( params.general.OTHER_TEAM, opponentRobot )
	      );   // calls dist( MovingObject& , MovingObject& )
}

// Finds distance between an object and a point
inline float dist( const MovingObject &mo, const Pair &p ){
  return dist( mo.getPos(), p );  // calls dist( Pair&, Pair& )
}

// Finds distance between our robot and a point
inline float dist( RobotIndex index, const Pair &p, 
		   const VisionData &field, 
		   const SystemParameters& params ){
  return dist(field.getRobot( params.general.TEAM, index ).getPos(), p);   // calls dist (Pair&, Pair&)
}

// Finds distance between our robot and a point
inline float dist(RobotIndex index,
                  float x,
				  float y,
				  const VisionData &field, 
				  const SystemParameters& params )
{
  return dist(field.getRobot( params.general.TEAM, index ).getXPos(), 
              field.getRobot( params.general.TEAM, index ).getYPos(),
              x, y);  
}


// Finds distance between a robot and a point
inline float dist( int team, RobotIndex index, const Pair &p,
		   const VisionData &field)
{
  MovingObject mo = field.getRobot(team, index);
  return dist(mo.getPos(), p); // calls dist (Pair&, Pair&)
}


// Finds distance between a robot and the ball
inline float distToRoboCupBall( int team, RobotIndex index, const VisionData &field)
{
  Pair a = field.getRobot(team, index).getPos();
  Pair b = getRobocupBall(field).getPos();
  return dist(a, b); // calls dist (Pair&, Pair&)
}

// Finds distance between our robots and the ball
inline float distToRoboCupBall(RobotIndex index,
			                   const VisionData &field, 
							   const SystemParameters &params)
{
	return distToRoboCupBall( params.general.TEAM, index, field);
}

// Finds distance between a point and the ball
inline float distToRoboCupBall( const Pair &p, const VisionData &field)
{
  return dist(p, getRobocupBall(field).getPos()); // calls dist (Pair&, Pair&)
}

// returns the point along the line from currentLocation to target that is exactly desiredDistance
// away from currentLocation.
inline Pair scaleTarget( const Pair& currentLocation, const Pair& target, float desiredDistance)
{
  float xDiff = target.getX() - currentLocation.getX();
	float yDiff = target.getY() - currentLocation.getY();

	float trueDistance = dist(currentLocation, target);
  if(trueDistance == 0.0f)
  {
    trueDistance = 0.000001f; //never divide by 0
  }

	return Pair( currentLocation.getX() + xDiff*(desiredDistance/trueDistance),
               currentLocation.getY() + yDiff*(desiredDistance/trueDistance));
}

//finds the shortest distance between a line (in y = m*x + b form) and a point
float distFromPointToLine(float line_m, float line_b, const Pair& point);
//Finds distance from an exterior point to a line defined by two points
float distFromPointToLine(const Pair& linePoint1,const Pair& linePoint2,const Pair& exteriorPoint);

// Finds the shortest distance between a point and the line coincident with the given line 
// segment between the start and end points. The argument onSegment is returned true if the 
// point which is closest to the input point lies on the segment (between start and end points)
// Coordinates of the closest point are returned in closestPoint.
float distFromPointToSegment ( const Pair & segmentStart, const Pair & segmentEnd,
                               const Pair & point, Pair * closestPoint, bool & onSegment);

// This method returns the shortest time taken by a robot (of a specified team) to reach
// a given location, based upon the omni trajectory.
float timeToClosestRobot (
  const Pair & location,        // Location for which to check
  const VisionData & field,     // Where everyone is
  const SystemParameters & params,    // Parameters
  const RobotIndex & ignoreBot, // Ignore this robot (should be greater than NUM_ROBOTS if
                                // nothing to be ignored)
  bool ourTeam,                 // Our team (true) or theirs (false)?
  RobotIndex & fastestRobot     // ID of the robot which can reach the location fastest
  );



#endif // DIST_H
