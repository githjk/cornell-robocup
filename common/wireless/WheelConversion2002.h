#ifndef WHEEL_CONVERSION_2002_H
#define WHEEL_CONVERSION_2002_H

class WheelConversion2002 {



public:
  // Initialize velocity conversion constants
  WheelConversion2002();

  // Converts (xVel,yVel,rotVel) vector into 
  // wheel velocities for the omni wheels
  void convertWheelVelocities( float xVel,
                               float yVel,
                               float rotVel,
                               float rotation,
                               float &velocityMagnitude,
                               float &velocityDirection,
                               float &rotationalVelocity);

};

#endif