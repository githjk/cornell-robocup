#ifndef WHEEL_CONVERSION_2001_H
#define WHEEL_CONVERSION_2001_H

class WheelConversion2001 {

public:
  // Initialize velocity conversion constants
  WheelConversion2001();

  // Converts (xVel,yVel,rotVel) vector into 
  // wheel velocities for the omni wheels
  void convertWheelVelocities( float xVel,
                               float yVel,
                               float rotVel,
                               float rotation,
                               float &wheelOne,
                               float &wheelTwo,
                               float &wheelThree);

};

#endif