#ifndef PAIR_H
#define PAIR_H

#include "ai.h"
#include "VisionModule.h"
#include "common/datatypes/SystemParameters.h"

//========================================================================= 
//returns true if the ball is between two Pairs
bool ballIsBetween(Pair A, 
                   Pair C, 
                   VisionData& field,
                   const SystemParameters& params,
                   float maxAwayDist=-32000.0f,
                   float maxSideDist=-32000.0f);
//========================================================
//returns true if a given robot is between two Pairs
bool isBetween(Pair A, 
               Pair B, 
               int team, 
               RobotIndex ID, 
               VisionData& field,
               const SystemParameters& params,
               float maxDepthDist=-32000.0f,
               float maxSideDist=-32000.0f);
//========================================================
//ends a line through A and B past B by extensionDistance to find point C
void extendPoint(Pair A,
                 Pair B, 
                 float extensionDistance,
                 Pair& C);
//========================================================
//rotates a point about an arbitrary origin by an angle (radians)
void rotateAboutPoint(Pair originalPoint,
                      Pair origin,
                      float rotateAngle,
                      Pair& result);
//========================================================
//extrapolates a line to a given Y
void extrapolateForX(Pair A,
                 Pair B,
                 float y,
                 Pair& C);
//========================================================
//extrapotales a line to a given X
void extrapolateForY(Pair A,
                 Pair B,
                 float x,
                 Pair& C);

#endif //PAIR_H
