#include "perpendicular.h"
#include "common/util/intersect/intersect.h"
#include "common/util/pair/pair.h"
#include "common/util/math/angle.h"

#include "GUI/GUI.h"

//=========================================
//Draws a vector from A through B. Draws
//the perpendicular through C and finds the intersection.
//Returns the distance from B to the intersection along the first
//vector, including sign. Useful for determining
//if a robot is behind/in front of an opponent with the ball.
float pOffSet(Pair A,
              Pair B,
              Pair C)
{
  //If A and B are same, return now
  if(A.getX() == B.getX() &&
     A.getY() == B.getY())
  {
    return 0.0f;
  }

  //First construct a line from A through B
  Line AB(A,B);

  //Next, using the slow of AB, calculate the slope and construct the
  //perpencular line through C
  Pair D;
  Line CD;
  CD.setA(C);
 
  //zero slope case
  if(B.getY() == A.getY())
  {
    D.set(C.getX(),
          C.getY() + 0.5f);
    CD.setB(D);
  }
  //infinity slope case
  else if(B.getX() == A.getX())
  {
    D.set(C.getX() + 0.5f,
          C.getY());
    CD.setB(D);
  }
  //standard case
  else
  {
    float ABslope = AB.getSlope();
    float CDslope = -1.0f/ABslope;
    D.set(C.getX() + 0.5f, 
          C.getY() + 0.5f*CDslope);
    CD.setB(D);
  }

  //Find AB and CD intersection
  Pair intersection;
  findLineIntersection(AB, CD, &intersection);

  //Determine intersection ofset and sign from B
  float result = intersection.distanceTo(B);

  float ABangle = angleBetween(A, B);
  float AIntersectionAngle = angleBetween(A, intersection);
  float diffAngle = angleDifference(ABangle, AIntersectionAngle);
  
  if(ABS(diffAngle) > PI/2.0f)
    result = -result;

  GUI_Record.debuggingInfo.setDebugPoint(1, 0, A);
  GUI_Record.debuggingInfo.setDebugPoint(1, 1, B);
  GUI_Record.debuggingInfo.setDebugPoint(1, 2, C);
  GUI_Record.debuggingInfo.setDebugPoint(1, 3, D);
  GUI_Record.debuggingInfo.setDebugPoint(1, 4, intersection);

  return result;
}
//==========================================
