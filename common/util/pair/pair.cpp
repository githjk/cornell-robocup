#include "pair.h"
#include "common/util/info/ball/ball.h"
#include "common/util/info/robot/robot.h"
#include "common/util/intersect/intersect.h"
//========================================================================= 
//returns true if the ball is between two Pairs
bool ballIsBetween(Pair A, 
                   Pair C, 
                   VisionData& field,
                   const SystemParameters& params,
                   float maxAwayDist,
                   float maxSideDist)
{
  //if robot not found then clearly not between =-)
  if(!isRobocupBallFound(field))
    return false;

  //B is robot
  Pair B(getBallLocation(field));

  float BDslope;

  if(C.getX() - A.getX() != 0)
  {
    //slope between 2 input points
    float ACslope = (C.getY() - A.getY()) / (C.getX() - A.getX());

    //BD is perpendicular to AC
    BDslope = -1/ACslope;
  }
  else
    BDslope = 0;

  //actually find point B
  Pair D(B.getX() + 10.0f, B.getY() + 10.0f*BDslope);

  //find intersection
  Line AC(A,C);
  Line BD(B,D);
  Pair E;
  findLineIntersection(AC,BD,&E);

  //calculate alpha (away distance) and beta (off to side distance)
  float alpha = A.distanceTo(E);
  float beta = E.distanceTo(B);

  //if beta is not too big (robot not too far off to the side)
  //and maxAwayDist == -32000 OR alpha is less than maxAwayDist, return true!
  if(
      //not to far to the side
      ((maxSideDist == -32000.f &&
         beta < 1.5f*params.general.PLAYER_RADIUS
       ) ||
       (maxSideDist != -32000.f &&
         beta < maxSideDist
       )
      ) &&
      //not to far away
      ((maxAwayDist == -32000.f &&
        E.distanceTo(C) < A.distanceTo(C)) ||
       (alpha < maxAwayDist &&
        E.distanceTo(C) < A.distanceTo(C))
      )
    )
    return true;
  else
    return false;
}
//========================================================================= 
//returns true if a given robot is between two Pairs
bool isBetween(Pair A, 
               Pair C, 
               int team, 
               RobotIndex ID, 
               VisionData& field,
               const SystemParameters& params,
               float maxAwayDist,
               float maxSideDist)
{
  //if robot not found then clearly not between =-)
  if(!robotFound(team, ID, field, params))
    return false;

  //B is robot
  Pair B(getLocation(team,ID,field));

  float BDslope;

  if(C.getX() - A.getX() != 0)
  {
    //slope between 2 input points
    float ACslope = (C.getY() - A.getY()) / (C.getX() - A.getX());

    //BD is perpendicular to AC
    BDslope = -1/ACslope;
  }
  else
    BDslope = 0;

  //actually find point B
  Pair D(B.getX() + 10.0f, B.getY() + 10.0f*BDslope);

  //find intersection
  Line AC(A,C);
  Line BD(B,D);
  Pair E;
  findLineIntersection(AC,BD,&E);

  //calculate alpha (away distance) and beta (off to side distance)
  float alpha = A.distanceTo(E);
  float beta = E.distanceTo(B);

  //if beta is not too big (robot not too far off to the side)
  //and maxAwayDist == -32000 OR alpha is less than maxAwayDist, return true!
  if(
      //not to far to the side
      ((maxSideDist == -32000.f &&
         beta < 4*params.general.PLAYER_RADIUS
       ) ||
       (maxSideDist != -32000.f &&
         beta < maxSideDist
       )
      ) &&
      //not to far away
      ((maxAwayDist == -32000.f &&
        E.distanceTo(C) < A.distanceTo(C)) ||
       (alpha < maxAwayDist &&
        E.distanceTo(C) < A.distanceTo(C))
      )
    )
    return true;
  else
    return false;
}//========================================================================= 
//given points A and B, extends line past B from A by extensionDistance and
//returns that point C
void extendPoint(Pair A,
                 Pair B,
                 float extensionDistance,
                 Pair& C)
{
  float ABdist = A.distanceTo(B);
  float ratio = (ABdist+extensionDistance) / ABdist;
  C.setX((ratio*(B.getX() - A.getX())) + A.getX());
  C.setY((ratio*(B.getY() - A.getY())) + A.getY());
}
//========================================================================= 
//rotates a point about an arbitrary origin by an angle (radians)
void rotateAboutPoint(Pair originalPoint,
                      Pair origin,   
                      float rotateAngle,
                      Pair& result)
{
  //method:
  //1.) shift origin to be (0,0)
  //2.) rotate point
  //3.) unshift result

  //shift
  originalPoint.setX(originalPoint.getX() - origin.getX());
  originalPoint.setY(originalPoint.getY() - origin.getY());

  //rotate
  float C = SQRT(originalPoint.getX()*originalPoint.getX() + 
                 originalPoint.getY()*originalPoint.getY());
  float beta = ASIN(originalPoint.getY()/C);  
  
  if(originalPoint.getX() < 0)
    beta = PI - beta;

  result.setX(C*COS(normalizeAngle(rotateAngle + beta)));
  result.setY(C*SIN(normalizeAngle(rotateAngle + beta)));

  //unshift
  result.setX(result.getX() + origin.getX());
  result.setY(result.getY() + origin.getY());
}
//========================================================================= 
//given points A and B, extrapolates/interpolates to find the point on the
// line AB that matches the given y value and returns that point C
void extrapolateForX(Pair A,
                 Pair B,
                 float y,
                 Pair& C)
{
  if(B.getY()==A.getY()) C.setX(B.getX());
  else C.setX((B.getX()-A.getX())/(B.getY()-A.getY()) * (y-A.getY()) + A.getX());
  C.setY(y);
}
//========================================================================= 
//given points A and B, extrapolates/interpolates to find the point on the
// line AB that matches the given x value and returns that point C
void extrapolateForY(Pair A,
                 Pair B,
                 float x,
                 Pair& C)
{
  if(B.getX()==A.getX()) C.setY(B.getY());
  else C.setY((B.getY()-A.getY())/(B.getX()-A.getX()) * (x-A.getX()) + A.getY());
  C.setX(x);
}
//========================================================================= 
