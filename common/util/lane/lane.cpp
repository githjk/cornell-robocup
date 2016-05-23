/*
 * lane.cpp
 */

#include "lane.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "math/angle.h"

//==================================================================================

// Find the width of a clear lane between the two locations passed as pairs.
// Returns 0.0 if the lane is blocked by any other robot.
float laneHalfWidth ( 
        Pair passerLoc,               // Position of the passing robot.
        Pair passDestination,  // location of the pass destination
        const VisionData &field,      // where everyone else is now
        const SystemParameters &rp,// contains necessary game parameters                  
      bool checkOurRobots)  //to see if we check for our robots or not...
{
    
    float minPassBuffer = 10000.0f;  //Distance from pass line to nearest enemy robot
  float tempPassBuffer; //Used when comparing buffer from one robot to min buffer
    Pair closestPoint;      //Point on pass line closest to robot
    bool onSegment;         //true if closestPoint is on pass line segment
  
  Pair originalDest(passDestination.getX(),passDestination.getY());
  //extend the pass destination point so an enemy a little behind will be considered
  float lineLength = dist(passerLoc,passDestination);
  passDestination.setX(passDestination.getX() + 2*rp.general.PLAYER_RADIUS*( (passDestination.getX() - passerLoc.getX()) / lineLength));
  passDestination.setY(passDestination.getY() + 2*rp.general.PLAYER_RADIUS*( (passDestination.getY() - passerLoc.getY()) / lineLength));
  
  //Check to make sure no enemy robots are in the way
    for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) {       
        //If enemy robot is found, check to see if it is in the way
        if(theirRobotFound(i, field, rp)) {      
            tempPassBuffer = distFromPointToSegment(passerLoc, passDestination, getTheirRobotLocation(i, field, rp), &closestPoint, onSegment)-rp.general.OPPONENT_RADIUS;

            //If enemy is on segment, return 0.  Pass is occluded
            if(onSegment && tempPassBuffer <0.0f ) return 0.0f;

      //Replace the min pass buffer if smaller buffer is found.
            if (tempPassBuffer < minPassBuffer && onSegment)
        minPassBuffer = tempPassBuffer;
        }    
    }   
    
  //CODE TO CHECK OUR ROBOTS ALSO.  REMEMBER TO IGNORE PASSER & RECEIVER
    if(checkOurRobots) {
      //Check to make sure no enemy robots are in the way
      for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) {     
          //If our robot is found, and he is not the passer or receiver => check to see if it is in the way
          if(robotFound(i, field, rp)&&
        (dist(i,passerLoc,field,rp)>rp.general.PLAYER_RADIUS)&&
        (dist(i,originalDest,field,rp)>rp.general.PLAYER_RADIUS))
      {      
              tempPassBuffer = distFromPointToSegment(passerLoc, passDestination, getLocation(i, field, rp), &closestPoint, onSegment)- rp.general.PLAYER_RADIUS;

              //If robot is on segment, return 0.  Pass is occluded
              if(onSegment && tempPassBuffer <0.0f) return 0.0f;

        //Replace the min pass buffer if smaller buffer is found.
              if (tempPassBuffer < minPassBuffer && onSegment)
          minPassBuffer = tempPassBuffer;
          }    
      } 
    }

    //Buffer = distance from pass line to edge (not center) of nearest enemy.
    float buf = minPassBuffer;
    if(buf <= 0.0f) return 0.001f; //to avoid divide-by-zero errors, and so dist>0
    else return buf;
}
//===================================================================================
float laneHalfAngle (Pair passerLoc, // Position of the passing robot.
                     Pair passDestination, // location of the pass destination
                     const VisionData &field, // where everyone else is now
                     const SystemParameters &rp, // contains necessary game parameters  
                     const Pair * extraObstacle, // Check this (optional) location as an obstacle too 
                     bool checkOurRobots) //to see if we check for our robots or not...
{

  float minLaneAngle = PI/2.0f; // Initialize the lane half angle to PI/2 (a clear lane)

  // Lane parameters
  float laneLength = passerLoc.distanceTo (passDestination);
  laneLength = laneLength + rp.general.PLAYER_RADIUS + rp.general.BALL_RADIUS;
  float laneDirection = angleWithXAxis (passerLoc, passDestination);

  // Obstacle parameters
  Pair obstacleLoc;         // Obstacle centre location
  float obstacleDist;       // Distance of the obstacle centre from the passerLoc
  float obstacleDirection;  // Angle made by the segment from passerLoc to obstacle with the x-axis
  float obstacleHalfAngle;  // Half the angle subtended by the obstacle on the passer loc
  float angleWithLane1;     // Angle between the tangents drawn from passerLoc to obstacle with 
  float angleWithLane2;     // the lane direction


  // Check opponent robots first
  RobotIndex i;
  for (i = ROBOT0; i < NUM_ROBOTS; i++) {

    if (theirRobotFound (i, field, rp)) {

      // Get obstacle direction and distance from the passerLoc
      obstacleLoc = getTheirRobotLocation (i, field, rp);
      obstacleDist = passerLoc.distanceTo (obstacleLoc);
  
      // Get the direction of the segment from passerLoc to obstacle with the x-axis
      obstacleDirection = angleWithXAxis (passerLoc, obstacleLoc);
      obstacleHalfAngle = ATAN2 (rp.general.OPPONENT_RADIUS, obstacleDist);

      // Evaluate angles only if the passer loc is closer to the obtacle than the lane length
      if (obstacleDist <= laneLength) {

        // If the difference between the obstacle direction and lane direction is within the half
        // angle subtended by the obstacle on the passer loc, the lane is being blocked
        if (ABS (laneDirection - obstacleDirection) <= obstacleHalfAngle)
          return 0.0f;

        // Get the directions of the tangent to the obstacle from the passer loc and their angular
        // difference from the lane direction
        angleWithLane1 = ABS (angleDifference (obstacleDirection + obstacleHalfAngle, laneDirection));
        angleWithLane2 = ABS (angleDifference (obstacleDirection - obstacleHalfAngle, laneDirection));
      

        // Compare the angle of the tangent closer to the lane diretion
        float smallerAngle = MIN (angleWithLane1, angleWithLane2);
        if (smallerAngle < minLaneAngle)
          minLaneAngle = smallerAngle;

      }
    }

  }

  //IF WE WANT TO TAKE INTO ACCOUNT OUR OWN ROBOTS...
  if (checkOurRobots)
  {
    for (i = ROBOT0; i < NUM_ROBOTS; i++) {

      //If our robot is found, and he is not the passer or receiver => check to see if it is in the way
          if(robotFound(i, field, rp)&&(dist(i,passerLoc,field,rp)>rp.general.PLAYER_RADIUS)&&(dist(i,passDestination,field,rp)>rp.general.PLAYER_RADIUS)) {      

        // Get obstacle direction and distance from the passerLoc
        obstacleLoc = getLocation (i, field, rp);
        obstacleDist = passerLoc.distanceTo (obstacleLoc);
  
        // Get the direction of the segment from passerLoc to obstacle with the x-axis
        obstacleDirection = angleWithXAxis (passerLoc, obstacleLoc);
        obstacleHalfAngle = ATAN2 (rp.general.PLAYER_RADIUS, obstacleDist);

        // Evaluate angles only if the passer loc is closer to the obtacle than the lane length
        if (obstacleDist <= laneLength) {

          // If the difference between the obstacle direction and lane direction is within the half
          // angle subtended by the obstacle on the passer loc, the lane is being blocked
          if (ABS (laneDirection - obstacleDirection) <= obstacleHalfAngle)
            return 0.0f;

          // Get the directions of the tangent to the obstacle from the passer loc and their angular
          // difference from the lane direction
          angleWithLane1 = ABS (angleDifference (obstacleDirection + obstacleHalfAngle, laneDirection));
          angleWithLane2 = ABS (angleDifference (obstacleDirection - obstacleHalfAngle, laneDirection));
      

          // Compare the angle of the tangent closer to the lane diretion
          float smallerAngle = MIN (angleWithLane1, angleWithLane2);
          if (smallerAngle < minLaneAngle)
            minLaneAngle = smallerAngle;

        }
      }

    }
  }


  // Check the passed extra location as an obstacle having the radius the same as our robots
  if (extraObstacle) {

    // Get obstacle direction and distance from the passerLoc
    obstacleLoc = * extraObstacle;
    obstacleDist = passerLoc.distanceTo (obstacleLoc);

    // Get the direction of the segment from passerLoc to obstacle with the x-axis
    obstacleDirection = angleWithXAxis (passerLoc, obstacleLoc);
    obstacleHalfAngle = ATAN2 (rp.general.PLAYER_RADIUS, obstacleDist);

    // Evaluate angles only if the passer loc is closer to the obtacle than the lane length
    if (obstacleDist <= laneLength) {

      // If the difference between the obstacle direction and lane direction is within the half
      // angle subtended by the obstacle on the passer loc, the lane is being blocked
      if (ABS (laneDirection - obstacleDirection) <= obstacleHalfAngle)
        return 0.0f;

      // Get the directions of the tangent to the obstacle from the passer loc and their angular
      // difference from the lane direction
      angleWithLane1 = ABS (angleDifference (obstacleDirection + obstacleHalfAngle, laneDirection));
      angleWithLane2 = ABS (angleDifference (obstacleDirection - obstacleHalfAngle, laneDirection));
      

      // Compare the angle of the tangent closer to the lane diretion
      float smallerAngle = MIN (angleWithLane1, angleWithLane2);
      if (smallerAngle < minLaneAngle)
        minLaneAngle = smallerAngle;

    }
  }

  return minLaneAngle;
}