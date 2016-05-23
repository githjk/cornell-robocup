#include "WheelConversion2000.h"
#include <math.h>

static float PI = 3.1459f;

static float wheelX[3];
static float wheelY[3];
static float wheelAngle[3];

static float wheelCos[3];
static float wheelSin[3];

WheelConversion2000::WheelConversion2000()
{

  wheelX[0] =  0.0350f;
  wheelX[1] =  0.0350f;
  wheelX[2] = -0.0510f;

  wheelY[0] = -0.0530f;
  wheelY[1] =  0.0530f;
  wheelY[2] = -0.0000f;

  wheelAngle[0] =  (200.0f * PI)/180.0f;
  wheelAngle[1] =  (-20.0f * PI)/180.0f;
  wheelAngle[2] =  ( 90.0f * PI)/180.0f;

  for (int i = 0; i < 3; i++)
  {
    wheelCos[i] = (float)cos( wheelAngle[i] );
    wheelSin[i] = (float)sin( wheelAngle[i] );
  }


}


//  italy00_normal.setAllWheelsX    ( 0.035,  0.035, -0.051 );
//  italy00_normal.setAllWheelsY    (-0.053,  0.053,  0.0);
//  italy00_normal.setAllWheelsTheta(200*PI/180,  -20*PI/180, 90*PI/180);


static float xWheelVel [3];
static float yWheelVel [3];
static float linearWheelVel[3];

static float cosRot, sinRot;


void WheelConversion2000::convertWheelVelocities( float xVel,
                             float yVel,
                             float rotVel,
                             float rotation,
                             float &wheelOne,
                             float &wheelTwo,
                             float &wheelThree)
{

  cosRot = (float)cos(rotation);
  sinRot = (float)sin(rotation);


  // Find the x and y velocities at each wheel location
  for(int j=0; j<3; j++)
  {
    xWheelVel [j] = (  cosRot * xVel) + ( sinRot * yVel) - ( rotVel * wheelY[j] );
    yWheelVel [j] = (- sinRot * xVel) + ( cosRot * yVel) + ( rotVel * wheelX[j] );
  }

  // Find the component of velocity in the controlled direction
  for(j=0; j<3; j++)
  {
    linearWheelVel[j] = ( xWheelVel[j] * wheelCos[j] ) 
                      + ( yWheelVel[j] * wheelSin[j] );
  }

  // Return results
  wheelOne    = linearWheelVel[0];
  wheelTwo    = linearWheelVel[1];
  wheelThree  = linearWheelVel[2];

}


//    wlocdot [j][0] =  cosRot  * command->getFirstXVel(robotIndex)
//                + sinRot  * command->getFirstYVel(robotIndex)
//                - command->getFirstRotVel(robotIndex) * wheelCfg.y_wheelpos(j);

//    wlocdot [j][1] =- sinRot  * command->getFirstXVel(robotIndex)
//                    + cosRot  * command->getFirstYVel(robotIndex)
//                    + command->getFirstRotVel(robotIndex) * wheelCfg.x_wheelpos(j);

//    linspeed[j] = wheelCfg.velocityScalingFactor * 
//                  (wlocdot[j][0] * wheelCfg.getOrient(j, WHEEL_COS) +
//                   wlocdot[j][1] * wheelCfg.getOrient(j, WHEEL_SIN) );
