#ifndef WALL_AVOIDANCE_H
#define WALL_AVOIDANCE_H

#include "Trajectory.h"

//---------------------------------------------------------
// This stores several variables used for wall avoidance
void updateWallAvoidance
(
  float ourGoalLine,
  float theirGoalLine,
  float rightSideLine,
  float rightGoalBox,
  float rightGoalPost,
  float leftSideLine,
  float leftGoalBox,
  float leftGoalPost,
  float ourGoalieBox,
  float theirGoalieBox,
  float playerRadius,
  float maxWallOffset,
  float maxWallOffsetDistance
);

//---------------------------------------------------------
// Choose a new destination such that the robot will not
// hit the corners of the goal
void avoidGoalCorners(ObjectPosVel  start,
                      ObjectPosVel &final );


//---------------------------------------------------------
// Make sure the nextStep of the robot's path will not lead
// it into the walls
void avoidWalls (float frameRate,
                 ObjectPosVel final,
                 ObjectPosVel &nextStep );


//---------------------------------------------------------
void avoidGoalieWalls (float frameRate,
                       ObjectPosVel final,
                       ObjectPosVel &nextStep );

//---------------------------------------------------------
// Make sure the nextStep of the robot's path will not lead
// it into either goalie box
void avoidGoalieBox (ObjectPosVel final,                    
                     ObjectPosVel &nextStep,
                     float frameRate );



#endif //OBSTACLE_AVOIDANCE_H
