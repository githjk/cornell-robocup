/**
 * Math: General and useful functions
 */

//#include "base_types.h"

#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <stdio.h>
//#include <iostream.h>

// Constants
#ifndef PI
#define PI	3.1415926f
#endif

#ifndef TWO_PI
#define TWO_PI  6.2831853f
#endif

const float SQRT_2 = (float) sqrt(2);


// The sine function cast to a float
#ifndef SIN
#define SIN(theta) ( (float) sin(theta) )
#endif

#ifndef COS
#define COS(theta) ( (float) cos(theta) )
#endif

inline float ABS(float x) {
  return (x < 0) ? -x : x;
}

inline float TAN(float x) {
  float xCos = COS(x);
  if( ABS (xCos) < 0.001f) {
    if (xCos >= 0.0f)
      return SIN(x) / 0.001f;
    else
      return SIN(x) / -0.001f;
  }
  else
    return (float)tan(x);
}


//computes the arctangent of y/x
//#ifndef ATAN2
//#define ATAN2(x, y) ( (float) atan2(x, y) )
//#endif

inline float ASIN(float x) {
    if(x > 1.f) return (float) PI/2;
    if(x < -1.f) return (float) -PI/2;
    return (float) asin(x);
}

inline float ACOS(float x) {
    if(x > 1) return 0.0f;
    if(x < -1) return PI;
    return (float) acos(x);
}

inline float ATAN2(float y, float x) {
    if(y == 0.f && x == 0.f) {
        printf("ERROR -- ATAN2 given args: y = %f , x = %f\n", y, x);
        return 0.f;
    }
    return (float) atan2(y,x);
}

#ifndef MAX
#define MAX(x, y) ( (x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef SQRT
#define SQRT(x) ( (float) sqrt(x) )
#endif

#ifndef SIGN
#define SIGN(x) ( ( x > 0 ) ? 1.0f : -1.0f )
#endif

//Don't use this on an expression with side-effects.
#ifndef ENSURE_WITHIN_RANGE
#define ENSURE_WITHIN_RANGE(var, min, max) {var = MAX(min , MIN(max,var));}
#endif

float normalizeAngle(float x);

// returns which of (x, y, z) is smallest
// if x smallest, returns 0, if y => 1, if z => 2
int findSmallestIndex(float x, float y, float z);

//==========================================

#endif // MATH_H
