#include "receive_pass.h"
#include "info/ball/ball.h"
#include "math/cmath.h"

#define BALL_VELOCITY_THRESHOLD .01f

bool interceptPass(const Pair& ballPos,
                 const Pair& ballVel,
                 const Pair& passTarget,
                 Pair* interceptionPoint,
                 const SystemParameters& params) {

  // If ball is not moving, then the interception point will be the ball's position
  //   but the pass won't actually be completed as planned
  if (ABS(ballVel.getX())<BALL_VELOCITY_THRESHOLD && ABS(ballVel.getY())<BALL_VELOCITY_THRESHOLD) {
    interceptionPoint->setX(ballPos.getX());
    interceptionPoint->setY(ballPos.getY());
    return false;
  }

  float timeToIntersection = (ballVel.getX()*(passTarget.getX()-ballPos.getX()) +
            ballVel.getY()*(passTarget.getY()-ballPos.getY()))/(ballVel.getX()*ballVel.getX() +
            ballVel.getY()*ballVel.getY());
  interceptionPoint->setX(ballPos.getX() +ballVel.getX()*timeToIntersection);
  interceptionPoint->setY(ballPos.getY() +ballVel.getY()*timeToIntersection);

  // DOES offField take into account robot radius??
  if (offField(params,*interceptionPoint)) {
    //find intersection with wall and set that point as destination
    return false;
  }

  if (timeToIntersection < 0) {
    return false;
  }

  return true;
}