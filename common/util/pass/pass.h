#ifndef PASS_H
#define PASS_H

#include "ai.h"
#include "VisionModule.h"
#include "robocup_strategy_data.h"

//-------------------------------------------------
//Used in pass algorithms. Has a position and a score
struct SweetSpot
{
  Pair point;  //center of the sweet spot
  float score; //how good is it? lower score is better
};
//-------------------------------------------------

//used to find a point that is behind two enemy bots 
//(aka splitted defenders, useful for passing)
//
void extendVectorBallMidpoint(const Pair& ball,  
                              const Pair& pointA,
                              const Pair& pointB,
                              const float& extensionDistance,
                              //-----------------------------
                              // Extended point from midpoint
                              //-----------------------------
                              Pair* destination);

//Best pass for goalie, never returns a position that is above a third of the field away
//returns true if there is a passlane, else if not. postion with the best passlane is 
//passed by reference in receiver
bool goalieBestPass(const VisionData& field,
                    const RobocupStrategyData& sd, 
			        RobotIndex goalieID,	//position of passing robot
			        RobotIndex* receiver);  //position of best robot to receive

//---------------------------------------------------------------------------------------
// Finds the angle in which the robot should be facing to properly redirect a moving ball.
//  The angle phi is used, where phi=0 when the robot is facing in the positive y-direction.
//  The angle returned is using the standard angle theta. Raff did the math using phi, though,
//  so we didn't change it.
float reboundAngle(const Pair& ballVel,
                  const Pair& robotPos,
                  const Pair& ballDest,
                  const SystemParameters& p);

// Used by reboundAngle to numerically solve for the correct angle
float bisection(const Pair& ballVel,
                float X,
                float Y,
                const SystemParameters& p,
                float leftPoint,
                float rightPoint,
                float lValue,
                float rValue);





#endif //PASS_H
