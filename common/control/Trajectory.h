#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "TrajectoryStructs.h"

float distance(float x1, float y1, float x2, float y2);

//=================================================
// TIMING FUNCTIONS

void expectedPathTime(  ObjectPosVel start,
                        ObjectPosVel final,
                        float frameRate,
                        float maxAccel,
                        float maxVelocity,
                        float& pathTime
                      );

void expectedFastPathTime(  ObjectPosVel start,
                            ObjectPosVel final,
                            float frameRate,
                            float maxAccel,
                            float maxVelocity,
                            float& pathTime
                          );

//=================================================
// WALL COLLISION DETECTION FUNCTION

// Returns true if the zero-final-velocity path
// would ever exit the bounding conditions
bool detectWallCollision( ObjectPosVel start,
                          ObjectPosVel final,
                          float frameRate,
                          float maxAccel,
                          float maxVelocity,
                          float minX,
                          float maxX,
                          float minY,
                          float maxY
                      );

//=================================================
// TRAJECTORY GENERATION FUNCTIONS

void zeroFinalVelocityPath( ObjectPosVel start,
                            ObjectPosVel final,
                            float frameRate,
                            float maxAccel,
                            float maxThetaAccel,
                            float maxVelocity,
                            float maxThetaVelocity,
                            ObjectPath& thePath,
                            int numberOfSteps = -1
                          );

void fastPath( ObjectPosVel start,
               ObjectPosVel final,
               float frameRate,
               float maxAccel,
               float maxVelocity,
               ObjectPath& thePath,
               int numberOfSteps);

//=================================================
// TRAJECTORY GENERATION FUNCTIONS

void zeroFinalVelocityTheta(ObjectPosVel start,
                            ObjectPosVel final,
                            float frameRate,
                            float maxThetaAccel,
                            float maxThetaVelocity,
                            float & nextAngularVelocity
                          );



//=================================================
// PROPORTIONAL GAIN FUNCTIONS

void proportionalGainPath(ObjectPosVel start,
                          ObjectPosVel final,
                          float frameRate,
                          float translationalGain,
                          float angularGain,
                          float maxVelocity,
                          float maxThetaVelocity,
                          ObjectPath& thePath
                         );


void proportionalGainTheta(ObjectPosVel start,
                           ObjectPosVel final,
                           float frameRate,
                           float angularGain,
                           float maxThetaVelocity,
                           float & nextAngularVelocity
                          );


#endif TRAJECTORY_H
