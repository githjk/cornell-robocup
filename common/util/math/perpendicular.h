#include "base_types.h"
#include "ai.h"

#ifndef PERPENDICULAR_H
#define PERPENDICULAR_H

//=========================================
//Draws a vector from A through B. Draws
//the perpendicular of AB through C and finds the intersection.
//Returns the distance from B to the intersection along the first
//vector, including sign. Useful for determining
//if a robot is behind/in front of an opponent with the ball.
//Example:
//
//
//  C      I 
//    
//         A
//
//         B
//
//I = intersection of AB and perpendicular through C
//returned distance would be from B to I
float pOffSet(Pair A,
              Pair B,
              Pair C);
//==========================================

#endif // PERPENDICULAR_H
