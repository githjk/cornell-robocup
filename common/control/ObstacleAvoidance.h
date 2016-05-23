#ifndef OBSTACLE_AVOIDANCE_H
#define OBSTACLE_AVOIDANCE_H

#include "Trajectory.h"
#include "VisionModule.h"

//---------------------------------------------------------
// This pre-calculates several variables used for obstacle avoidance
void updateObstacleAvoidance
(
  float playerRadius,
  float opponentRadius,
  float ballRadius,
  float ourGoalLine,
  float ourGoalieBox,
  float theirGoalLine,
  float theirGoalieBox,
  float rightSideLine,
  float rightGoalieBox,
  float leftSideLine,
  float leftGoalieBox,
  float slowClearance,
  float fastClearance,
  float fastClearanceVelocity,
  float latencyFrames
);

//---------------------------------------------------------
// This builds the static prediction array
void setupObstacleAvoidance(VisionData* currentField, float frameRate);

//---------------------------------------------------------
// Using the static prediction array, find a path from start
//    to final which will not hit any obstacles
void findOpenPath(ObjectPosVel start,
                  ObjectPosVel final,
                  int robotTeam,
                  int robotIndex,
                  float frameRate,
                  float maxAccel,
                  float maxThetaAccel,
                  float maxVelocity,
                  float maxThetaVelocity,
                  ObjectPath & thePath);

//=====================================================================
// Using the static prediction array, find a path from start
//    to final which will not hit any obstacles
void findTestOpenPath(ObjectPosVel start,
                      ObjectPosVel final,
                      int robotTeam,
                      int robotIndex,
                      float frameRate,
                      float maxAccel,
                      float maxThetaAccel,
                      float maxVelocity,
                      float maxThetaVelocity,
                      ObjectPath & thePath);

//---------------------------------------------------------
// Using the static prediction array, find a path from start
//    to final which will not hit any obstacles
void findFastOpenPath(ObjectPosVel start,
                      ObjectPosVel final,
                      int robotTeam,
                      int robotIndex,
                      float frameRate,
                      float maxAccel,
                      float maxThetaAccel,
                      float maxVelocity,
                      float maxThetaVelocity,
                      ObjectPath & thePath,
                      int numberOfSteps = -1);

#endif //OBSTACLE_AVOIDANCE_H
