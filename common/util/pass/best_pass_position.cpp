/*
** best_pass_position.cpp
*/

#include "best_pass_position.h"
#include "..\math\angle.h"
#include "..\math\coordinate_transform.h"
#include "..\math\dist.h"
#include "..\lane\lane.h"
#include "..\info\ball\ball.h"

//================================================================================================

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
                       bool ignoreOpponentGoalie,       // Ignore the opponent goalie?
                       Pair  * bestLocation)            // Suitable pass location  
{

  if (maxPassLength <= minPassLength) return false;

  // Make sure the passer is not also the receiver
  if ((passerLoc.getX() == receiverLoc.getX()) && (passerLoc.getY() == receiverLoc.getY())) {
    bestLocation->set(passerLoc);
    return false;
  }

  // COORDINATE TRANSFORMATION PARAMETERS
  // ------------------------------------
  //
  // The new origin is the center of passer and receiver positions
  Pair newOrigin ((passerLoc.getX() + receiverLoc.getX())/2.0f,
                  (passerLoc.getY() + receiverLoc.getY())/2.0f);

  // Angle between the field axes and the line from passer to receiver
  float rotationAngle = angleBetween (receiverLoc, passerLoc);

  if (passerLoc.getY() > receiverLoc.getY() && rotationAngle < 0.0) 
    rotationAngle += PI;
  else if (passerLoc.getY() < receiverLoc.getY() && rotationAngle > 0.0) 
    rotationAngle -= PI;

  //  ELLIPSE GENERATION PARAMETERS
  //  -----------------------------
  //
  // Transform the coordinates of the passerLoc and receiverLoc to lie on the x-axis
  // of our new coordinate system. These locations then become foci of an ellips on which 
  // we generate our test points for pass locations.
  Pair focus1 = transformCoords (passerLoc, newOrigin, rotationAngle);
  focus1.setY (0.0f);

  Pair focus2 (-1.0f * focus1.getX(), 0.0f);

  // Distance of the foci from the origin, also known as c, for the ellipse.
  float c = focus1.getX();
  float cSquared = c * c;

  // ALL THE FOLLOWING FORMULAE ARE FOR AN ELLIPSE ALIGNED WITH THE COORDINATE AXES. THE MAJOR
  // AXIS OF THE ELLIPSE IS ALIGNED WITH THE X-AXIS, AND THE MINOR AXIS IS ALIGNED WITH THE 
  // Y-AXIS. So, we transform all the field coordinates to lie in the local coordinates of the
  // ellipse axes.
  // 
  // BOUNDS ON THE VALUES OF THE ECCENTRICITIES OF THE ELLIPSES WE SHOULD CHECK
  // --------------------------------------------------------------------------
  //
  // For an ellipse, if the distance between the two foci is 2c, and the legth of the major
  // axis is 2a, then the eccentricity, e = c/a. Also, the sum of distances of any point from 
  // the two foci is constant, and equal to 2a. Hence, for our case, maxPassLength is the upper
  // bound on 2a. Also, the parameter minPasslLengths puts a lower bound on the value of a.
  // 
  // Hence amax = maxPassLength / 2
  //       amin = minPassLength / 2
  //
  // Also, if the lenght of the minor axis is 2b, then
  // b^2 = a^2 - c^2

  if (maxPassLength == 0.0f || minPassLength == 0.0f) return false;

  float aMax = maxPassLength / 2.0f;
  float bMax = SQRT (aMax * aMax - cSquared);
  
  // We never want to check on an ellipse with the minor axis shorter than a specified value.
  // This value is specified in one of two ways:
  // i) maxPassAngle. (TAN(maxPassAngle) = c / bmin)
  // ii) minPassLength = 2Lmin (then, L^2 = bmin^2 + c^2)
  // and chose the bigger b from the above two methods

  float tanMaxAngle = TAN (maxPassAngle / 2.0f);
  if (tanMaxAngle == 0.0f) return false; // Check for division by 0

  float bMin = c  / tanMaxAngle;
  float aMin2 = SQRT (cSquared + bMin * bMin);

  float aMin = minPassLength / 2.0f;

  if (aMin2 > aMin) aMin = aMin2;

  // RATIO OF THE TWO PASS LENGTHS
  // -----------------------------
  //
  // The geometric equation of an ellipse is
  // x^2/a^2 + y^2/b^2 = 1
  // 
  // For a point (x,y) lying on the ellipse, the length of the lines joining the point to the foci
  // (c,0) and (-c,0) are (a - ex) and (a + ex) respectively. Hence, if we have an upper bound on
  // the ratio of the longer pass length to the smaller pass length, then we can find the limits 
  // of the closed interval within which x should lie. 
  // 
  // If (x > 0), a + ex > a - ex, and 
  // (a + ex) / (a - ex) < ratio, giving the relationship
  // x < (a/e) (ratio - 1) / (ratio + 1)
  //
  // Hence, if a limit on the lengths of the longer pass to the length of the smaller pass is 
  // specified, say L, then
  //  - (a/e).(L - 1)/(L + 1) <= x <= (a/e).(L - 1)/(L + 1)
  // or,
  // - a^2/c' <= x <= a^2/c'
  //
  // where we define c' as the normalised focal-length
  // c' = c.(L + 1)/(L - 1)

  if (maxPassLengthRatio == 1.0f) return false;
  float focalLength = c * (maxPassLengthRatio + 1) / (maxPassLengthRatio - 1);

  // Now all that needs to be done is to generate a fixed number of points on each ellipse
  // and find the best point to receive the pass at. The best point is defined as the one for which
  // there are no obstacles to either the passerLoc or receiverLoc and the minimum of the two lane 
  // widths is maximum for all the points searched


  // The transformation of the coordinate axes is such that, for our Robocup field orientation,
  // a rotation angle in the right half-plane results in the transformed negative y-axis pointing
  // towards our goal, while a rotation angle in the left half plane results in the transformed
  // positive y-axis pointing towards our goal. We use a director multiplier to make sure that the
  // y-coordinate value which we get from the ellipse equation is "backward" of the line joining
  // the passer and receiver locations

  float direction = -1.0f;
  if (rotationAngle <= PI && rotationAngle >= 0.0f) direction = 1.0f;

  // For now, generate 10 ellipses, with 25 sampling points on each, starting with the largest ellipse
  // first
  int numEllipses = 10;
  int numPoints = 25;

  float bestLaneWidth = 0.0f;
  Pair bestPassLocationOnField; // This is the coordinate in the original system, not the transformed

  float bestClearLocationLane = 0.0f;
  Pair bestClearLocation; // This is the best location on the field with no obstacles around 

  float deltaA = (aMax - aMin) / (numEllipses - 1);
  float thisA = aMin - deltaA;

  float obstacleCoords [5][2];
  int   numObstacles = 0;
  
  float effectiveObstacle = params.general.OPPONENT_RADIUS + minLaneWidth;

  //Check to make sure no enemy robots are in the way
  RobotIndex goalieId;
	for(RobotIndex ri = ROBOT0; ri < NUM_ROBOTS; ri++) {	
    
		if(theirRobotFound(ri, field, params)) {     
    
      if (getTheirGoalie (field, params, &goalieId) && ri == goalieId && ignoreOpponentGoalie) continue;

      // If the transformed coordinates of the opponent robot are within the box bounded by
      // aMax and bMax, then check it as an obstacle. Otherwise ignore it safely.
			Pair robotPos = getTheirRobotLocation(ri, field, params);
      Pair transformedPos = transformCoords (robotPos, newOrigin, rotationAngle);

      if ((transformedPos.getY() * direction >= 0.0f) && 
          (ABS(transformedPos.getY()) <= bMax + effectiveObstacle) &&
          (ABS(transformedPos.getX()) <= aMax + effectiveObstacle)) {
        obstacleCoords [numObstacles][0] = transformedPos.getX();
        obstacleCoords [numObstacles][1] = transformedPos.getY();
        numObstacles ++;
      }
    }
	}	

  int i;
  for (i = 0; i < numEllipses; i++) {

    // semi axes lengths
    thisA += deltaA;
    float aSquared = thisA * thisA;

    float thisB = SQRT (aSquared - cSquared);

    // The limits on the values of x-coordinates of the points we will sample
    float maxX = aSquared / focalLength;
    if (maxX >= thisA)  maxX = 0.95f * thisA;

    float minX = -1.0f * maxX;
    float deltaX = (maxX - minX)/(numPoints - 1);

    // Start point for sampling. Will be incremented in the for loop
    float x = minX - deltaX;

    // Sample all the points on this ellipse
    int j;
    for (j = 0; j < numPoints; j++) {
  
      // Get the sampling points
      x += deltaX;
      float y = direction * thisB * SQRT (1.0f - x * x / aSquared);
      Pair currentLoc (x, y);

      // Transform the ellipse point back to the field and ensure that the location lies
      // on the field. For legal field locations check the best lane widths available.
      Pair fieldLoc = inverseTransformCoords (currentLoc, newOrigin, rotationAngle);
      if (offField (params, fieldLoc, params.general.PLAYER_RADIUS) || 
          insideTheirGoalieBox (params, fieldLoc) || insideOurGoalieBox (params, fieldLoc)) continue;

      // We don't process currentLoc further if the lane available for pass from fieldLoc
      // is either blocked, or smaller than the lane available for an already found point
      float pass1LaneWidth = passLaneWidth (focus1, currentLoc, obstacleCoords, numObstacles,
                                            params.general.OPPONENT_RADIUS);
      if (pass1LaneWidth < minLaneWidth || pass1LaneWidth < bestLaneWidth) {
        continue;
      }

      // We don't process currentLoc further if the lane available for pass from receiverLoc
      // is either blocked, or smaller than the lane available for an already found point
      float pass2LaneWidth = passLaneWidth (currentLoc, focus2, obstacleCoords, numObstacles,
                                            params.general.OPPONENT_RADIUS);
      if (pass2LaneWidth < minLaneWidth || pass2LaneWidth < bestLaneWidth) {
        continue;
      }

      // find the minimum of the widths of the two available pass lanes
      float leanPassWidth = MIN (pass1LaneWidth, pass2LaneWidth);

      // If this location offers wider pass lanes than already found points then this point 
      // is better than any of the points found so far. If the location is not close to any
      // obstacles, it is the best clear location
      if (leanPassWidth > bestLaneWidth) {
        bestLaneWidth = leanPassWidth;  
        bestPassLocationOnField = fieldLoc;
      }

      // Check whether the current location is sufficiently far away from all opponent robots
      // to allow our robot to get there and receive and make a pass
      if (leanPassWidth > bestClearLocationLane) {

        float normalAngle = ATAN2 (y * aSquared, 
                                   x * (aSquared - cSquared));
        Pair normalVector;
        normalVector.setX (COS (normalAngle));
        normalVector.setY (SIN (normalAngle));

        Pair focusToPoint;
        focusToPoint.set (x - c, y);

        if (dotProduct (normalVector, focusToPoint) < 0.0f) {
          normalVector.setX (-1.0f * normalVector.getX());
          normalVector.setY (-1.0f * normalVector.getY());
        }
        
        Pair requiredRobotCenter;
        requiredRobotCenter.setX (x + params.general.PLAYER_RADIUS * normalVector.getX());
        requiredRobotCenter.setY (y + params.general.PLAYER_RADIUS * normalVector.getY());

        Pair requiredCenterOnField = inverseTransformCoords (requiredRobotCenter, 
                                                             newOrigin, rotationAngle);
     
        bool goodClearLocation = false;

        if (! offField (params, requiredCenterOnField, params.general.PLAYER_RADIUS) && 
            ! insideTheirGoalieBox (params, requiredCenterOnField) &&
            ! insideOurGoalieBox (params, requiredCenterOnField)) {

          goodClearLocation = true;

 	        for(RobotIndex ri = ROBOT0; ri < NUM_ROBOTS; ri++) {	

		        if(theirRobotFound(ri, field, params)) { 

			        Pair robotPos = getTheirRobotLocation(ri, field, params);
            
              if (robotPos.squareDistanceTo (requiredCenterOnField) <
                (params.general.PLAYER_RADIUS + params.general.OPPONENT_RADIUS) * (params.general.PLAYER_RADIUS + params.general.OPPONENT_RADIUS)) {
                goodClearLocation = false;
                break;
              }
            }
	        }	
        }

        if (goodClearLocation)  {
          bestClearLocation.set (bestPassLocationOnField);
          bestClearLocationLane = leanPassWidth;
        }
      }
    }
  }

  // If a point found on the field not close to any opponent robots, return its coordinates
  if (bestClearLocationLane > 0.0f) {
    bestLocation->set (bestClearLocation);
    return true;
  }
  
  // If no clear location found, return the next best location founf
  if (bestLaneWidth == 0.0f)
    return false;
  else {
    bestLocation->set (bestPassLocationOnField);
    return true;
  }
}

float passLaneWidth (const Pair & passerLoc,        // Pass origin location
                     const Pair & receiverLoc,      // Pass destination
                     float obstacleCoords [5][2],   // Coords of all obstacles (maximum 5)
                     const int & numObstacles,      // Number of obstacles
                     const float & obstacleRadius)  // Radius of an obstacle
{
  Pair closestPoint;
  float minimumDist = 10000.0;

  float obstacleRadSquared = obstacleRadius * obstacleRadius;

  int i;
  for (i = 0; i < numObstacles; i++) {

    Pair obstacleLoc (obstacleCoords[i][0], obstacleCoords[i][1]);
    bool onSegment;

    float normalDist = distFromPointToSegment (passerLoc, receiverLoc, obstacleLoc, &closestPoint, 
                                               onSegment);
    if (onSegment) {  
      if (normalDist <= obstacleRadius) return 0.0f;
      else if (normalDist < minimumDist) minimumDist = normalDist;
    }

    else {
      if (closestPoint.squareDistanceTo (passerLoc) <= obstacleRadSquared ||
          closestPoint.squareDistanceTo (receiverLoc) <= obstacleRadSquared) return 0.0f;
    }

  }

  return minimumDist - obstacleRadius;
}
//==================================================================================================
