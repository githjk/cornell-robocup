/**
 * coordinate_transform.h
 */

#ifndef COORDINATE_TRANSFORM_H
#define COORDINATE_TRANSFORM_H

#include "base_types.h"

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
                      const float & rotationAngle);   // Rotation of original axes 


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
Pair inverseTransformCoords (const Pair & transformedLocation, // Location to be transformed back
                                                               // to the original coordinate system
                             const Pair & translationVector,   // Location of the new origin
                             const float & rotationAngle);     // Rotation of the original axes

//====================================================================================

#endif // COORDINATE_TRANSFORM_H
