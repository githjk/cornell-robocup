/**
 * Math: Angle-finding functions
 */
#ifndef ANGLE_H
#define ANGLE_H

#include "base_types.h"
#include "ai.h"
#include "cmath.h"
#include "common/datatypes/SystemParameters.h"


//***********ANGLE FUNCTIONS*************************************************************

// returns a dicrete angle which is approximately 'angle'
// This could be used to decrease the number of calculations based on
// angles changing.(but only slightly)
float Discrete_Angle(float angle);

// Finds the angle a point to another point
inline float angleBetween(const Pair &a, const Pair &b)
{
  return float(ATAN2( b.getY() - a.getY(), b.getX() - a.getX() ) );//switched from atan2 to ATAN2
}


// Finds the angle a point to another point
inline float angleBetween( const float& x1, const float& y1, const float& x2, const float& y2)
{
  return float( ATAN2( y2 - y1, x2 - x1) );//switched from atan2 to ATAN2
}

inline float angleWithXAxis (const Pair & from, const Pair & to)
{
  float angle = ATAN2 (to.getY() - from.getY(), to.getX() - from.getX());

  if (to.getY() > from.getY() && angle < 0.0) 
    angle += PI;
  else if (to.getY() < from.getY() && angle > 0.0) 
    angle -= PI;

  return angle;
}

inline float angleWithPositiveXAxis (const Pair & from, const Pair & to)
{
  float angle = ATAN2 (to.getY() - from.getY(), to.getX() - from.getX());

  if (to.getY() > from.getY() && angle < 0.0)        // secodn quadrant
    angle += PI;
  else if (to.getY() < from.getY() && angle > 0.0)   // third quadrant
    angle += PI;
  else if (to.getY() < from.getY() && angle < 0.0)   // fourth quadrant
  	angle += TWO_PI;

  return angle;
}

//Finds angle between our robot and the ball
inline float angleToBall (RobotIndex index,  
                          const VisionData &field,
                          const SystemParameters& params)
{
  Pair a = field.getRobot( params.general.TEAM, index ).getPos();
  Pair b = field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos();

  //if ball and robot in same place (gui drag'n'drop doohickie)
  //then return 0.0f to avoid taking the atan2 of 0,0
  if(a.getX() == b.getX() &&
     a.getY() == b.getY())
    return 0.0f;

  float dx = b.getX() - a.getX(), dy = b.getY() - a.getY();
  return float( ATAN2( dy, dx ) );//switched from atan2 to ATAN2
}

// Finds the angle from object one to the ball
inline float angleToBall ( const MovingObject &mo,
			   const VisionData &field)
{
  Pair a = mo.getPos();
  Pair b = field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos();
  return angleBetween(a,b); // calls angleBetween(Pair&, Pair&)
}


// Finds the angle from point to the ball
inline float angleToBall ( const Pair &p, const VisionData &field){
  Pair b = field.getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos();
  return angleBetween(p,b); // calls angleBetween(Pair&, Pair&)
}

// Finds the angle from object one to object two
inline float angleBetween ( const MovingObject& a, const MovingObject &b)
{
  return angleBetween(a.getPos(), b.getPos());
}

// Finds the angle one of our robots to another
inline float angleBetween (RobotIndex p1, 
						   RobotIndex p2,
						   const VisionData &field,
						   const SystemParameters& params)
{
  MovingObject a = field.getRobot( params.general.TEAM, p1 );
  MovingObject b = field.getRobot( params.general.TEAM, p2 );
  return angleBetween(a.getPos(), b.getPos()); // angleBetween(Pair&, Pair&)
}

// Finds the angle from object_one to a point
inline float angleBetween ( const MovingObject &mo, const Pair &p){
  return angleBetween(mo.getPos(), p);
}

// Finds the angle from a point to object_one
inline float angleBetween ( const Pair &p, const MovingObject &mo){
  return TWO_PI - angleBetween(mo, p); // it's the reverse of MO-to-P
}

// Finds the angle from one of our robots to a point
inline float angleBetween ( RobotIndex index, 
                            const Pair &p, 
                            const VisionData &field,
                            const SystemParameters& params )
{
  MovingObject a = field.getRobot( params.general.TEAM, index );
  return angleBetween(a.getPos(), p); // calls angleBetween(Pair&, Pair&)
}


// Normalizes and returns the difference between two angles...
inline float angleDifference ( float angleA, float angleB )
{
  return normalizeAngle( angleA - angleB );
}


inline float dotProduct (const Pair & vector1, const Pair & vector2) 
{
  return (vector1.getX() * vector2.getX() + vector1.getY() * vector2.getY());
}

inline float crossProduct (Pair origin,
                           Pair leftSide,
                           Pair rightSide)
{
  Pair vector1(leftSide.getX() - origin.getX(),
               leftSide.getY() - origin.getY());

  Pair vector2(rightSide.getX() - origin.getX(),
               rightSide.getY() - origin.getY());

  float norm1 = origin.distanceTo(leftSide);
  float norm2 = origin.distanceTo(rightSide);

  if(norm1 == 0 || norm2 == 0)
  {
    //error case
    return 0.0f;
  }
  else
  {
    return (
             vector1.getX() * vector2.getY() -
             vector1.getY() * vector2.getX()
           ) / (norm1 * norm2);
  }
}

#endif // ANGLE_H
