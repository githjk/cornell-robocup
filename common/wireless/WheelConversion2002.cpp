#include "WheelConversion2002.h"
#include <math.h>


static float PI = 3.1459f;

WheelConversion2002::WheelConversion2002()
{ 
  // No initialization required
}

//Convert from global (field coordinates)
//to local (robot) coordinates
void WheelConversion2002::convertWheelVelocities( float xVel,
                             float yVel,
                             float rotVel,
                             float rotation,
                             float &xDotRobot,
                             float &yDotRobot,
                             float &phiDotRobot)
{
  // xDotRobot, yDotRobot should depend on rotation.
  xDotRobot = (float)(cos(rotation)*xVel + sin(rotation)*yVel);
  yDotRobot = (float)(-sin(rotation)*xVel + cos(rotation)*yVel);

  phiDotRobot = rotVel;

}