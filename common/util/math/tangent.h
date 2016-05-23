/**
 * Tangents.h
 */

#ifndef MATH_TANGETNS_H
#define MATH_TANGETNS_H

#include "base_types.h"

#define LARGE_RADIUS 100000
#define SMALL_RADIUS 0.000001

bool findTangents(
                  //Find tangents around this...
                  const Pair &object, 
                  //Where the light source is this...
		  const Pair &source, 
                  //Radius of object
		  const float &radius, 
		  Pair *tangent1,
		  Pair *tangent2);

// Find two circles, tangential to a given circle, and also to a secant passing through
// a point (fromPoint) and in a given direction (secantDir)
void findTangentCircles (
  const Pair  & centre,               // (in) centre of the circle to which the tangents are to be found
  const float & radius,               // (in) radius of the circle
  const Pair  & fromPoint,            // (in) point through which the secant passes
  const Pair  & secantDir,            // (in) slope vector of the secant
  Pair        * tangentPt1,           // (out) Point1 at which the output circle is tangential to our circle
  float       & tangentCircleRadius1, // (out) Radius of output circle1
  Pair        * tangentPt2,           // (in) Point2 at which the output circle is tangential to our circle
  float       & tangentCircleRadius2  // (out) Radius of output circle2
  );



//==========================================

#endif // MATH_TANGETNS_H
