
#include "dist.h"
#include <float.h>
#include "GUI/GUI.h"
#include "common/control/Trajectory.h"
//===============================================================================================
//finds the shortest distance between a line (in y = m*x + b form) and a point
float distFromPointToLine(float line_m, float line_b, const Pair& point)
{
  float slope = line_m;
  
  //--------------------------------
  //ensure the slop is valid
  if(
       _isnan(slope) || //not a number
       !_finite(slope)  //infinite
    )
  {
    slope = 1000.0f; //set the slope to be very steep
  }
  //--------------------------------

  float verticalDistance = slope * point.getX() + line_b - point.getY();
  float angleBetweenLineAndVertical = PI/2.0f - (float)atan(slope);
  return ABS(verticalDistance * SIN(angleBetweenLineAndVertical));
}

float distFromPointToLine(const Pair& linePoint1, const Pair& linePoint2, const Pair& exteriorPoint) {

  float slope = ( (linePoint2.getY() - linePoint1.getY()) / 
                (linePoint2.getX() - linePoint1.getX()) );

  //--------------------------------
  //ensure the slop is valid
  if(
       _isnan(slope) || //not a number
       !_finite(slope)  //infinite
    )
  {
    slope = 1000.0f; //this is the error value
  }
  //--------------------------------

  float b = linePoint1.getY() - ( slope * linePoint1.getX() );
  return distFromPointToLine(slope, b, exteriorPoint);
}

//===============================================================================================

// Finds the shortest distance between a point and the line coincident with the given line 
// segment between the start and end points. The argument onSegment is returned 1 if the 
// point which is closest to the input point lies on the segment (between start and end points)
// Coordinates of the closest point are returned in closestPoint.
float distFromPointToSegment ( const Pair & segmentStart, const Pair & segmentEnd,
                               const Pair & point, Pair * closestPoint, bool & onSegment)
{
  float segmentY = segmentEnd.getY() - segmentStart.getY();
  float segmentX = segmentEnd.getX() - segmentStart.getX();
  float pointToSegmentEndY = segmentEnd.getY() - point.getY();
  float pointToSegmentEndX = segmentEnd.getX() - point.getX();
  
  float segmentLengthSquared = segmentX * segmentX + segmentY * segmentY;

  float parametricLocation = 
    (segmentY * pointToSegmentEndY + segmentX * pointToSegmentEndX) / segmentLengthSquared;

  closestPoint->setX (segmentEnd.getX() - segmentX * parametricLocation);
  closestPoint->setY (segmentEnd.getY() - segmentY * parametricLocation);

  if (parametricLocation < 0.0 || parametricLocation > 1) onSegment = false;  
  else onSegment = true;
  return closestPoint->distanceTo (point);
}

//================================================================================================

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
  )
{
  float leastTimeSoFar = 1000.0f;
  fastestRobot = NO_ROBOT;

  // Instantiate and initialize a destination object
  Destination finalLoc;
  finalLoc.initialize();

  if (ourTeam)  finalLoc.setSpeed (DEFAULT_SPEED);
  else          finalLoc.setSpeed (OPPONENT_SPEED);

  for (RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++) {

    if (i == ignoreBot)   continue;

    MovingObject robot;
    if (ourTeam) {
      if (!robotFound (i, field, params))   continue;
      else robot = field.getRobot(params.general.TEAM, i);
    }

    else  {
      if (!theirRobotFound (i, field, params))  continue; 
      else  robot = field.getRobot(params.general.OTHER_TEAM, i);
    }

    ObjectPosVel start, finish;
    start.xPos = robot.getXPos();
    start.yPos = robot.getYPos();
    start.xVel = robot.getXVel();
    start.yVel = robot.getYVel();
    start.rotation= robot.getRotation();
    start.rotVel= robot.getRotVel();
        
    finish.xPos = finalLoc.getXPos();
    finish.yPos = finalLoc.getYPos();
    finish.rotation=robot.getRotation();
    finish.xVel = 0.0f;
    finish.yVel = 0.0f;
    finish.rotVel= 0.0f;

    float travelTime;
    expectedPathTime(start,
                     finish,
                     params.general.FRAME_RATE,
                     params.control.Omni2001.DEFAULT.MAX_ACCELERATION, // maxAccel,
                     params.control.Omni2001.DEFAULT.MAX_VELOCITY,// maxVelocity,
                     travelTime);

    if (travelTime  < leastTimeSoFar) 
    {
      leastTimeSoFar = travelTime;
      fastestRobot = i;
    }
  }

  return leastTimeSoFar;  
}

