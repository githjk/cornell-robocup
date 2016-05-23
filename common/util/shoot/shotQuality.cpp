
#include "shotQuality.h"
#include "info/robot/robot.h"

#include "math/dist.h"
#include "math/angle.h"

#include "GUI/GUI.h" 

//===================================================================================
// STATIC VARIABLES
static VisionData futureField;

//===================================================================================
// LOCAL FUNCTIONS

// This function finds the lane quality for the ball past the obstacle
float findLaneQuality(Pair ballLoc, 
                      Pair ballVel, 
                      Pair obstacle, 
                      Pair obstacleVel,
                      float maxTime
);

// Takes the current field, predicts it forward one frame,
//    and puts the result into the static variable, 'futureField'
void buildFutureFrame(const VisionData &field,
                      const SystemParameters &p
);

//===================================================================================

// Returns true if the robot has a clear shot to the goal in the direction it is facing
//   Assumes that the robot has the ball
bool goodQualityShot(int   robotTeam,
                     int   robotIndex,
                     float ballSpeed,
                     const VisionData &field,
                     const SystemParameters &p,
                     float minLaneWidth,
                     float &laneWidth
)
{
  int team, index;

  laneWidth = 1000.0f;

  float tempWidth;
  float radius;

  Pair ballLoc, ballVel;
  float rotation;

  //--------------------------------------------
  // Calculate ball location and speed
  rotation = field.getRobot(robotTeam, robotIndex).getRotation();
  ballLoc = frontOfRobot((RobotIndex)robotIndex, field, p);
  ballVel.setX( ballSpeed*(float)cos(rotation) );
  ballVel.setY( ballSpeed*(float)sin(rotation) );

  //--------------------------------------------
  // Calculate time for ball to reach the goal line
  float distToGoalLine = p.field.THEIR_GOAL_LINE - ballLoc.getX();
  float yGoalIntersect = (float)TAN(rotation)*distToGoalLine + ballLoc.getY();

  // Will the shot miss the goal?
  if ( (yGoalIntersect < p.field.RIGHT_GOAL_POST + (3*p.general.BALL_RADIUS + minLaneWidth/2)) ||
       (yGoalIntersect > p.field.LEFT_GOAL_POST  - (3*p.general.BALL_RADIUS + minLaneWidth/2)) ||
       (rotation > PI/2) ||
       (rotation < -PI/2) 
     )
  {
    return false;
  }

  if ( twoOpponentsInTheirBox(field, p ) )
    return true;

  float shotTime;
  if (0.0f <= ballSpeed)
    shotTime = 1000.0f;
  else
    shotTime = dist (ballLoc.getX(), ballLoc.getY(), p.field.THEIR_GOAL_LINE, yGoalIntersect) /
                   ballSpeed;

  //--------------------------------------------
  // Check the lane quality for every robot
  for (team = 0; team < NUM_TEAMS; team++) {

    // Choose a radius for the obstacle
    if (p.general.TEAM == team) radius = p.general.PLAYER_RADIUS;
    else radius = p.general.OPPONENT_RADIUS;

    // Check each robot
    for (index = 0; index < NUM_PLAYERS_ON_TEAM; index++) 
    {
      // Ignore the shooting robot
      if ((robotTeam == team) && (robotIndex == index))
        continue;

      tempWidth = findLaneQuality(ballLoc, 
                                  ballVel, 
                                  field.getRobot(team, index).getPos(), 
                                  field.getRobot(team, index).getVel(), 
                                  shotTime );

      tempWidth = tempWidth - radius;

      if (tempWidth < laneWidth)
        laneWidth = tempWidth;
    }
  }

  //--------------------------------------------
  // Check the left goal post
  tempWidth = findLaneQuality(ballLoc, 
                              ballVel, 
                              Pair( p.field.THEIR_GOAL_LINE, p.field.LEFT_GOAL_POST), 
                              Pair( 0.0f, 0.0f ), 
                              shotTime );
  if (tempWidth < laneWidth)
    laneWidth = tempWidth;

  //--------------------------------------------
  // Check the right goal post
  tempWidth = findLaneQuality(ballLoc, 
                              ballVel, 
                              Pair( p.field.THEIR_GOAL_LINE, p.field.RIGHT_GOAL_POST), 
                              Pair( 0.0f, 0.0f ), 
                              shotTime );
  if (tempWidth < laneWidth)
    laneWidth = tempWidth;

  //--------------------------------------------
  // If this lane is smaller than the given minLaneWidth, return false
  if (laneWidth < minLaneWidth)
    return false;
  else
    return true;
}

//-------------------------------------------------------------------
// Returns true if the robot has a clear shot to the goal in the direction it is facing
//    AND that no better shot will occur in future frames
//    Assumes that the robot has the ball

const float OPPONENT_ACCELERATION = 5.0f;

bool goodImmediateShot(int   robotTeam,
                       int   robotIndex,
                       float ballSpeed,
                       const VisionData &field,
                       const SystemParameters &p,
                       float minLaneWidth
)
{
  float currentLaneQuality;
  float futureLaneQuality;
  bool goodShot;

  // Calculate the current lane quality and make sure we have a good
  //    current shot
  goodShot = goodQualityShot( robotTeam,
                              robotIndex,
                              ballSpeed,
                              field,
                              p,
                              minLaneWidth,
                              currentLaneQuality );

  // If we don't have a good shot right now, then return;
  if (! goodShot)
    return false;

  //char msg[80];
  //sprintf(msg,"Current shot good (%4.2f), beat lane width of %4.2f", currentLaneQuality, minLaneWidth);
  //GUI_Record.debuggingInfo.addDebugMessage(msg);

  if ( twoOpponentsInTheirBox(field, p ) )
    return true;

  // Otherwise, build one frame of VisionData into the future, and
  // calculate the lane quality for the future shot.  If the future shot
  // is significantly better, return false
  // (This function assumes it will be called again next frame, so
  //  returning false effectively postpones the shot for at least one
  //  frame)
  buildFutureFrame(field, p);

  goodShot = goodQualityShot( robotTeam,
                              robotIndex,
                              ballSpeed,
                              futureField,
                              p,
                              minLaneWidth,
                              futureLaneQuality );

  // If the current shot is good, but the future shot is bad,
  //    then shoot now
  if (! goodShot) {
    //char msg[80];
    //sprintf(msg,"Current shot good (%4.2f), Future shot unfeasible", currentLaneQuality, futureLaneQuality);
    //GUI_Record.debuggingInfo.addDebugMessage(msg);

    return true;
  }

  // improvementThreshold = 0.5f * acceleration( timeStep^2 )
  // timeStep = 1 / FRAME_RATE
  float improvementThreshold = 0.5f * OPPONENT_ACCELERATION / (p.general.FRAME_RATE * p.general.FRAME_RATE);

  // If the future shot is significantly better than the current
  //    shot, then postpone the shot
  if (futureLaneQuality > currentLaneQuality + improvementThreshold) {
    
    //char msg[80];
    //sprintf(msg,"Current shot good (%4.2f), Future shot better (%4.2f)", currentLaneQuality, futureLaneQuality);
    //GUI_Record.debuggingInfo.addDebugMessage(msg);

    return false;
  }
  else {
    //char msg[80];
    //sprintf(msg,"Current shot good (%4.2f), Future shot worse (%4.2f)", currentLaneQuality, futureLaneQuality);
    //GUI_Record.debuggingInfo.addDebugMessage(msg);

    return true;
  }
}


//-------------------------------------------------------------------
// This function finds the lane quality for the ball past the obstacle
float findLaneQuality(Pair ballLoc, 
                      Pair ballVel, 
                      Pair obstacleLoc, 
                      Pair obstacleVel,
                      float maxTime
)
{

  float xDiff, yDiff;
  float xVelDiff, yVelDiff;
  float interceptionTime, interceptDistance;

  //-------------------------------------------
  xDiff = ballLoc.getX() - obstacleLoc.getX();
  yDiff = ballLoc.getY() - obstacleLoc.getY();

  xVelDiff = ballVel.getX() - obstacleVel.getX();
  yVelDiff = ballVel.getY() - obstacleVel.getY();

  //-------------------------------------------
  // Calculate the separation at the maximum interception point (maxTime)
  interceptDistance = (float)sqrt( ((xDiff + xVelDiff*maxTime) * (xDiff + xVelDiff*maxTime)) +
                                   ((yDiff + yVelDiff*maxTime) * (yDiff + yVelDiff*maxTime))
                               );

  // If the velocities are identical, then the ball and obstacle are moving in parallel
  if ( (0.0f == xVelDiff) && (0.0f == yVelDiff) )
  {
    return interceptDistance;
  }

  //-------------------------------------------
  // Calculate the separation at the interception point (interceptionTime)
  interceptionTime = -1.0f * ( (xDiff    * xVelDiff) + (yDiff    * yVelDiff) ) / 
                 ( (xVelDiff * xVelDiff) + (yVelDiff * yVelDiff) );

  if (interceptionTime < 0.0f) {
    interceptionTime = 0.0f;
  }
  else if (interceptionTime > maxTime) {
    interceptionTime = maxTime;
  }

  interceptDistance = (float)sqrt( ((xDiff + xVelDiff*interceptionTime) * (xDiff + xVelDiff*interceptionTime)) +
                                   ((yDiff + yVelDiff*interceptionTime) * (yDiff + yVelDiff*interceptionTime))
                                );

  return interceptDistance;
}

//-------------------------------------------------------------------
// Takes the current field, predicts it forward one frame,
//    and puts the result into the static variable, 'futureField'
void buildFutureFrame(const VisionData &field,
                      const SystemParameters &p
)
{
  int team, index;
  float xNext, yNext, rotationNext;

  // Push forward each robot
  for (team = 0; team < NUM_TEAMS; team++) {
    for (index = 0; index < NUM_PLAYERS_ON_TEAM; index++) 
    {
      xNext =  field.getRobot(team, index).getXPos() + 
              (field.getRobot(team, index).getXVel() / p.general.FRAME_RATE );

      yNext =  field.getRobot(team, index).getYPos() + 
              (field.getRobot(team, index).getYVel() / p.general.FRAME_RATE );

      rotationNext =  field.getRobot(team, index).getRotation() + 
                     (field.getRobot(team, index).getRotVel() / p.general.FRAME_RATE );

      futureField.setRobotXPos(team, index, xNext);
      futureField.setRobotYPos(team, index, yNext);
      futureField.setRobotRotation(team, index, rotationNext);
    }
  }
}
