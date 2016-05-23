
#ifndef SHOT_QUALITY_H
#define SHOT_QUALITY_H

#include "ai.h"
#include "VisionData.h"
#include "common/datatypes/SystemParameters.h"


// Returns true if the robot has a clear shot to the goal in the direction it is facing
//   Assumes that the robot has the ball
bool goodQualityShot(int   robotTeam,
                     int   robotIndex,
                     float ballSpeed,
                     const VisionData &field,
                     const SystemParameters &p,
                     float minLaneWidth,
                     float &laneWidth
);

// Returns true if the robot has a clear shot to the goal in the direction it is facing
//    AND that no better shot will occur in future frames
//   Assumes that the robot has the ball
bool goodImmediateShot(int   robotTeam,
                       int   robotIndex,
                       float ballSpeed,
                       const VisionData &field,
                       const SystemParameters &p,
                       float minLaneWidth
);


#endif // SHOT_QUALITY_H








