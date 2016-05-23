#include "pass.h"
#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "shoot/shoot.h"
#include "math/dist.h"
#include "math/angle.h"
#include "lane/lane.h"

//-------------------------------------------------
static float xDiff, yDiff;
static Pair midpoint;
static float vectorSlope;
static float dx, dy;
static float m, b;
static float xA, yA;
static float xBot, yBot;
//-------------------------------------------------
//used to find a point that is behind two enemy bots 
//(aka splitted defenders, useful for passing)
//
void extendVectorBallMidpoint(const Pair& ball,
                              const Pair& pointA,
                              const Pair& pointB,
                              const float& extensionDistance,
                              //-----------------------------
                              // Extended point from midpoint
                              //-----------------------------
                              Pair* destination)
{
  xDiff = ( pointA.getX() - pointB.getX() );
  yDiff = ( pointA.getY() - pointB.getY() );

  midpoint.setX( (pointA.getX() + pointB.getX()) / 2 );
  midpoint.setY( (pointA.getY() + pointB.getY()) / 2 );

  //Slope = (Y2-Y1)/(X2-X1)
  vectorSlope = (midpoint.getY() - ball.getY()) / 
                (midpoint.getX() - ball.getX());

  //If my math is right, which I'll write below, this is a very simple calculation.
  //
  //   o |   *
  //     |y
  //   --*
  //   x
  //  *
  //x^2 + y^2 = C^2 where x is the change in xposition from the point we are looking for
  //and the midpoint of the vector,same for y, and C is the distance we want to move back.
  //
  //slope = y/x
  //y = slope * x
  //so x^2 + y^2 = C^2
  //x^2 + (slope * x)^2 = C^2
  //
  //x^2 + x^2slope^2 = c^2
  //x^2(1 + slope^2) = c^2
  //x^2 = c^2 / (1 + slope^2)
  //x = c / sqrt(1 + slope^2)
  //y = x * slope.
  //so as you can see, we can do the whole thing with the last to lines of code,
  //and the only complicated instruction is one square root operation =-)
  //dx = sqrt((extension*extension)/(1 + (pslope * pslope)));

  dx = SQRT(
            (extensionDistance * extensionDistance) /
            (1 + (vectorSlope * vectorSlope))
           );

  dy = (dx*vectorSlope);

  destination->setX( midpoint.getX() - (0.4f*dx) );
  destination->setY( midpoint.getY() - (0.4f*dy) );
}


//Best pass for goalie, never returns a position that is above a third of the field away
//returns true if there is a passlane, else if not. postion with the best passlane is 
//passed by reference in receiver
bool goalieBestPass(const VisionData& field,
                    RobocupStrategyData& sd, 
			        RobotIndex goalieID,	//position of passing robot
			        RobotIndex* receiver)//position of best robot to receive
{
	
	RobotIndex best=NO_ROBOT;
	Pair front;
	float score;
	float bestScore = 10000;
	float optimalDistanceSquared = sd.getSystemParams().strategy2002.GOALIE_OPTIMAL_PASS_DISTANCE
								* sd.getSystemParams().strategy2002.GOALIE_OPTIMAL_PASS_DISTANCE;


	for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
	{
		//If friendly guy is not me, and he is found, and he is ready to receive a pass!
		//and he is far enough back
		if(
		   (goalieID !=i) && 
		   (robotFound(i, field, sd.getSystemParams())) &&
		   (sd.getPassValue(i) != 0) &&
		   (sd.getPassDest(i).getX() <= sd.getSystemParams().field.OUR_GOAL_LINE/6)
		  )
		{
			//find distance from my front to where the guy is going to receive the pass
			front = frontOfRobot(goalieID, field, sd.getSystemParams());
			score = float(fabs(front.squareDistanceTo(sd.getPassDest(i)) - optimalDistanceSquared));
			if (score < bestScore){
				bestScore = score;
				best = i;
			}
		}
	}
	
	*receiver = best;
	return (best != NO_ROBOT);
}

//---------------------------------------------------------------------------------------
// Finds the angle in which the robot should be facing to properly redirect a moving ball.
//  The angle phi is used, where phi=0 when the robot is facing in the positive y-direction.
//  The angle returned is using the standard angle theta. Raff did the math using phi, though,
//  so we didn't change it.

#define INTERVAL_SIZE .1f

float reboundAngle(const Pair& ballVel,
                  const Pair& interceptionPoint,
                  const Pair& ballDest,           // target to be shot/passed at
                  const SystemParameters& p)
{
  float leftPoint,rightPoint;
  float phi;  // phi=0 is facing the y-axis, not the x-axis
  float X,Y;
  float lValue,rValue;  //Function values corresponding to the angles leftPoint,rightPoint

  //find endpoints for bisection interval -- start from -PI and go to PI
  leftPoint = -PI;
  phi = leftPoint;
  X = ballDest.getX() - interceptionPoint.getX();  //outgoing ball vector (x-component)
  Y = ballDest.getY() - interceptionPoint.getY();  //outgoing ball vector (y-component)
  lValue = (float)(p.strategy2002.KICK_VELOCITY*(X*cos(phi)+Y*sin(phi)) + X*ballVel.getY()*sin(phi)*sin(phi)
            - Y*ballVel.getX()*cos(phi)*cos(phi) + cos(phi)*sin(phi)*(X*ballVel.getX()-Y*ballVel.getY()));
  rValue = lValue;
  rightPoint = leftPoint;
  while (lValue*rValue > 0 && rightPoint<=PI) {
    leftPoint = rightPoint;
    rightPoint += INTERVAL_SIZE;  // keep incrementing rightPoint until we get a zero-crossing
    phi = rightPoint;
    rValue = (float)(p.strategy2002.KICK_VELOCITY*(X*cos(phi)+Y*sin(phi)) + X*ballVel.getY()*sin(phi)*sin(phi)
            - Y*ballVel.getX()*cos(phi)*cos(phi) + cos(phi)*sin(phi)*(X*ballVel.getX()-Y*ballVel.getY()));
  }
  phi = leftPoint;
  lValue = (float)(p.strategy2002.KICK_VELOCITY*(X*cos(phi)+Y*sin(phi)) + X*ballVel.getY()*sin(phi)*sin(phi)
            - Y*ballVel.getX()*cos(phi)*cos(phi) + cos(phi)*sin(phi)*(X*ballVel.getX()-Y*ballVel.getY()));
  float firstRoot = bisection(ballVel,X,Y,p,leftPoint,rightPoint,lValue,rValue);

  // take the dot product of the robot's tangent vector and the outgoing & (negative) incoming
  //   ball vectors. They should be opposite signs if the root is the correct root
  //   If so, return the angle, else look for the second root (there are only 2)
  //   We want to make sure that if the ball hits the robot from the left side,
  //   it rebounds on the right side.

  float robotTangentVectorX = (float)(-p.general.PLAYER_RADIUS*sin(firstRoot - PI/2));
  float robotTangentVectorY = (float)(p.general.PLAYER_RADIUS*cos(firstRoot - PI/2));
  float dot1 = -ballVel.getX()*robotTangentVectorX - ballVel.getY()*robotTangentVectorY;
  float dot2 = (float)((ballDest.getX() - (interceptionPoint.getX() - p.general.PLAYER_RADIUS*sin(phi)))*robotTangentVectorX
               + (ballDest.getY() - (interceptionPoint.getY() + p.general.PLAYER_RADIUS*cos(phi)))*robotTangentVectorY);
  if (dot1*dot2 < 0) {
    return (normalizeAngle(firstRoot + PI/2)); // need to convert from phi to theta, so add pi/2
  }

  // repeat above calculation for the second root
  lValue = rValue;
  while (lValue*rValue > 0 && rightPoint<=PI) {
    leftPoint = rightPoint;
    rightPoint += INTERVAL_SIZE;  // keep incrementing rightPoint until we get a zero-crossing
    phi = rightPoint;
    rValue = (float)(p.strategy2002.KICK_VELOCITY*(X*cos(phi)+Y*sin(phi)) + X*ballVel.getY()*sin(phi)*sin(phi)
            - Y*ballVel.getX()*cos(phi)*cos(phi) + cos(phi)*sin(phi)*(X*ballVel.getX()-Y*ballVel.getY()));
  }
  phi = leftPoint;
  lValue = (float)(p.strategy2002.KICK_VELOCITY*(X*cos(phi)+Y*sin(phi)) + X*ballVel.getY()*sin(phi)*sin(phi)
            - Y*ballVel.getX()*cos(phi)*cos(phi) + cos(phi)*sin(phi)*(X*ballVel.getX()-Y*ballVel.getY()));
  float secondRoot = bisection(ballVel,X,Y,p,leftPoint,rightPoint,lValue,rValue);
  return (normalizeAngle(secondRoot + PI/2)); // need to convert from phi to theta, so add pi/2
  

}

// Used by reboundAngle to numerically solve for the correct angle
//  Uses the bisection method to solve for the root of an equation
float bisection(const Pair& ballVel,
				float X,
				float Y,
				const SystemParameters& p,
                float leftPoint,
				float rightPoint,
				float lValue,
				float rValue) 
{
  float midPoint,mValue,phi;

  float error = MIN(ABS(lValue),ABS(rValue));
  while (error > .01) {
    midPoint = (leftPoint + rightPoint)/2;
    phi = midPoint;
    mValue = (float)(p.strategy2002.KICK_VELOCITY*(X*cos(phi)+Y*sin(phi)) + X*ballVel.getY()*sin(phi)*sin(phi)
             - Y*ballVel.getX()*cos(phi)*cos(phi) + cos(phi)*sin(phi)*(X*ballVel.getX()-Y*ballVel.getY()));
    if (mValue*lValue > 0) {
      leftPoint = midPoint;
      lValue = mValue;
      error = MIN(ABS(rValue),ABS(mValue));
    } else {
      rightPoint = midPoint;
      rValue = mValue;
      error = MIN(ABS(lValue),ABS(mValue));
    }
  }
  return (error==ABS(lValue))?leftPoint:rightPoint;


}


//==================================================================================

