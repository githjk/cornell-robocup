/**
 * Math: General and useful functions
 */

#include "cmath.h"
#include <iostream.h>
#include <float.h>

float normalizeAngle(float angle)
{
  if(
      _isnan(angle) || //not a number
     !_finite(angle)  //infinite
    )
  {
    cout << "Error:  NORMALIZE_ANGLE called on " << angle << endl;
    return 0.0f;
  }

  float sangle = SIN(angle);
  float cangle = COS(angle);
  return ATAN2(sangle, cangle);
}


int findSmallestIndex(float x, float y, float z) 
{
  int smallest = 0;
  float currSmallest = x;

  if (y < currSmallest) {
    currSmallest = y;
    smallest = 1;
  }
  
  if (z < currSmallest) {
    smallest = 2;
  }

  return smallest;
}