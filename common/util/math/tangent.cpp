/*
 * tangent.cpp
 */

#include "tangent.h"
//==========================================
static float alpha, 
             beta, 
             alphaSquared, 
             betaSquared;

static float temp, 
             distanceSquared;

static float case1SinTheta, 
             case1CosTheta, 
             case2SinTheta, 
             case2CosTheta;

static float case1RadiusAIntersectionX, 
             case1RadiusAIntersectionY;
static float case2RadiusAIntersectionX, 
             case2RadiusAIntersectionY;
//----------------------------------------
bool findTangents(//Find tangents around this...
                  const Pair &object,
                  //Where the light source is this...
                  const Pair &source,
                  //Radius of object
                  const float &radius,
                  Pair *tangent1,
                  Pair *tangent2)
{
  alpha = ( source.getX() - object.getX() ) / radius;
  beta  = ( source.getY() - object.getY() ) / radius;
  alphaSquared = alpha * alpha;
  betaSquared = beta * beta;

  //if we can, use it (we are outside its radius), 
  //otherwise, ignore it and move on
  if((alphaSquared + betaSquared) <= 1)
    return false;

  // Build some temp variables for calculating the two detour points
  temp = (float)(sqrt(betaSquared + alphaSquared - 1));
  distanceSquared = alphaSquared + betaSquared;

  case1SinTheta = ( beta  + ( alpha * temp ) ) / distanceSquared;
  case1CosTheta = ( alpha - ( beta  * temp ) ) / distanceSquared;

  case2SinTheta = ( beta  - ( alpha * temp ) ) / distanceSquared;
  case2CosTheta = ( alpha + ( beta  * temp ) ) / distanceSquared;

  case1RadiusAIntersectionX = object.getX() + (radius * case1CosTheta);
  case1RadiusAIntersectionY = object.getY() + (radius * case1SinTheta);
  case2RadiusAIntersectionX = object.getX() + (radius * case2CosTheta);
  case2RadiusAIntersectionY = object.getY() + (radius * case2SinTheta);

  //====================================================================
  // First Detour Position
  tangent1->setX( case1RadiusAIntersectionX - object.getX() );
  tangent1->setY( case1RadiusAIntersectionY - object.getY() );

  // Second Detour Position
  tangent2->setX( case2RadiusAIntersectionX - object.getX() );
  tangent2->setY( case2RadiusAIntersectionY - object.getY() );

  return true;
}

//==========================================================================================================

void findTangentCircles (
  const Pair  & centre,               // (in) centre of the circle to which the tangents are to be found
  const float & radius,               // (in) radius of the circle
  const Pair  & fromPoint,            // (in) point through which the secant passes
  const Pair  & secantDir,            // (in) slope (unit) vector of the secant
  Pair        * tangentPt1,           // (out) Point1 at which the output circle is tangential to our circle
  float       & tangentCircleRadius1, // (out) Radius of output circle1
  Pair        * tangentPt2,           // (in) Point2 at which the output circle is tangential to our circle
  float       & tangentCircleRadius2  // (out) Radius of output circle2
  )
{

  float X = fromPoint.getX() - centre.getX();
  float Y = fromPoint.getY() - centre.getY();

  tangentCircleRadius1 = (X * X + Y * Y - radius * radius) / 2.0f;
  tangentCircleRadius2 = tangentCircleRadius1;

  float denominator = radius + X * secantDir.getY() - Y * secantDir.getX();
  if (ABS (denominator) < 0.00001f) {
    if (denominator * tangentCircleRadius1 > 0.0f) tangentCircleRadius1 = LARGE_RADIUS;
    else                                           tangentCircleRadius1 = -1.0f * LARGE_RADIUS;
  } 

  else {
    tangentCircleRadius1 /= denominator;
  }

  denominator = radius - X * secantDir.getY() + Y * secantDir.getX();
  if (ABS (denominator) < 0.00001f) {
    if (denominator * tangentCircleRadius2 > 0.0f) tangentCircleRadius2 = LARGE_RADIUS;
    else                                           tangentCircleRadius2 = -1.0f * LARGE_RADIUS;
  } 

  else {
    tangentCircleRadius2 /= denominator;
  }

  float theta1 = ATAN2 (Y + secantDir.getX() * tangentCircleRadius1, 
                        X - secantDir.getY() * tangentCircleRadius1);
  float theta2 = ATAN2 (Y - secantDir.getX() * tangentCircleRadius2, 
                        X + secantDir.getY() * tangentCircleRadius2);

  // If the radii calculated for the tangent circles have different signs then one of the circles
  // encompasses the circle to which the tangents are being found. This circle has to have the larger 
  // radius
  if (tangentCircleRadius1 * tangentCircleRadius2 < 0.0f) {

    if (ABS (tangentCircleRadius1) > ABS (tangentCircleRadius2))
      theta1 += PI;

    else if (ABS (tangentCircleRadius2) > ABS (tangentCircleRadius1))
      theta2 += PI;
  }

  tangentPt1->set (centre.getX() + radius * COS (theta1), centre.getY() + radius * SIN (theta1));
  tangentPt2->set (centre.getX() + radius * COS (theta2), centre.getY() + radius * SIN (theta2));
}
//==========================================================================================================