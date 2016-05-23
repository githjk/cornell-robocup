/**
 * coordinate_transform.cpp
 */

#include "coordinate_transform.h"

//====================================================================================

// This method finds the coordinates of any given point (X,Y) on the field to another 
// coordinate system (X',Y'), where (X',Y') system is obtained by shifting the origin
// to (Xo,Yo) and rotating the axes by an angle Q. The equations for finding (X',Y')
// are:
//         _   _         _      _   _               _
//        |     |       |        | |                 |
//        |  X' |       | X - Xo | |  cos(Q)  sin(Q) |
//        |     |  =    |        |.|                 |
//        |  Y' |       | Y - Yo | | -sin(Q)  cos(Q) |
//        |_   _|       |_      _| |_               _|
//
Pair transformCoords (const Pair & originalLocation,  // Location to be transformed
                      const Pair & translationVector, // Location of the new origin
                      const float & rotationAngle)    // Rotation of original axes 


{
  Pair transformedLocation;

  float translationX = originalLocation.getX() - translationVector.getX();
  float translationY = originalLocation.getY() - translationVector.getY();

  transformedLocation.setX (translationX * COS (rotationAngle) +
                            translationY * SIN (rotationAngle));

  transformedLocation.setY (translationY * COS (rotationAngle) -
                            translationX * SIN (rotationAngle));

  return transformedLocation;
}


// This method finds the inverse transformation to map any coordinate (X',Y') in the new
// coordinate system (obtained by translating the origin to (Xo,Yo) and rotating the axes
// by an angle Q) to the field coordinate system.
//         _   _         _  _   _               _     _   _
//        |     |       |    | |                 |   |     |
//        |  X  |       | X' | |  cos(Q) -sin(Q) |   |  Xo |
//        |     |  =    |    |.|                 | + |     |
//        |  Y  |       | Y' | |  sin(Q)  cos(Q) |   |  Yo |
//        |_   _|       |_  _| |_               _|   |_   _|
//
// In the method below, translationVector is (Xo,Yo), and rotationAngle is Q. The method
// returns the inverse transformed location (X,Y) corresponding to the 
// transformedLocation (X',Y')
Pair inverseTransformCoords (const Pair & transformedLocation, // Location to be transformed back
                                                               // to the original coordinate system
                             const Pair & translationVector,   // Location of the new origin
                             const float & rotationAngle)      // Rotation of the original axes
{
  Pair fieldLocation;

  fieldLocation.setX (transformedLocation.getX() * COS (rotationAngle) -
                      transformedLocation.getY() * SIN (rotationAngle) + 
                      translationVector.getX());

  fieldLocation.setY (transformedLocation.getX() * SIN (rotationAngle) +
                      transformedLocation.getY() * COS (rotationAngle) + 
                      translationVector.getY());

  return fieldLocation;
}

//==========================================================================================