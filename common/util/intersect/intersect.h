//intersect.h
//valuable intersetion algorithms..

#include "base_types.h"

#ifndef UTIL_INTERSECT_H
#define UTIL_INTERSECT_H

#include "ai.h"
#include "VisionModule.h"
#include "common/datatypes/SystemParameters.h"

const float NEVER_INTERSECT = -32000;
//----------------
//Finds the intersection between two lines
void findLineIntersection(const Line &A, 
                          const Line &B,
                          Pair *intersection);
//----------------
//Finds the intersection between two lines (same as above)
inline Pair findLineIntersection(const Line &A, 
                                 const Line &B)
{
  Pair temp;
  findLineIntersection(A, B, &temp);
  return temp;
}
//----------------
//Finds the intersection between two lines (same as above)
inline Pair findLineIntersection(const Pair &line1pt1,
                                 const Pair &line1pt2,
                                 const Pair &line2pt1,
                                 const Pair &line2pt2)
{
  Pair temp;  
  findLineIntersection(Line(line1pt1,line1pt2), Line(line2pt1,line2pt2), &temp);
  return temp;
}
//----------------
void intersectFastMovingBall( const VisionData& field, 
                              const SystemParameters& params,
                              float xValueOfDefendingLine,
                              Destination* command);
//----------------
void intersectBallGoal( const VisionData& field, 
                        const SystemParameters& params,
                        float xValueOfDefendingLine,
                        float goalLeft,	//sometimes we want to restrict
                        float goalRight,  //which part of the goal we will protect
                        Destination* command);
//----------------
inline float ballInterceptionForTheGoalieAndDefender(
		float			newx, 
		const MovingObject&	ball
)
{
	if (ball.getXVel() == 0.0f)
		return ball.getYPos();

	return float(ball.getYPos() + ball.getYVel()*(newx-ball.getXPos())/ball.getXVel());
}
//----------------
#endif // UTIL_INTERSECT_H
