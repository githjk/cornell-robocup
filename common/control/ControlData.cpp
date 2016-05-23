#include "ControlData.h"
#include "math/cmath.h"

DribbleSpeed DRIBBLE_DEFAULT;
VerticalDribbleSpeed V_DRIBBLE_DEFAULT;

// Constructor
ControlData::ControlData() { 
  tSample=0.0f;
  for (RobotIndex i=ROBOT0; i < NUM_ROBOTS; i++)
    stopRobot(i);
}


// Set robot velocities manually
void ControlData::setRobotVelocity( RobotIndex robotIndex, 
                                    float xVelocity,
                                    float yVelocity,
                                    float rotVelocity
) 
{
	xVel[robotIndex]    = xVelocity;
	yVel[robotIndex]    = yVelocity;
  rotVel[robotIndex]  = rotVelocity;
}

// Set robot commands manually
void ControlData::setRobotCommands( RobotIndex robotIndex,
                                    KickSpeed kickCommand,
                                    DribbleSpeed dribbleCommand,
                                    VerticalDribbleSpeed verticalDribbleCommand,
                                    BeepCommand beepCommand
)
{
  kick[robotIndex]            = kickCommand;
  dribble[robotIndex]         = dribbleCommand;
  verticalDribble[robotIndex] = verticalDribbleCommand;
  beep[robotIndex]            = beepCommand;
}


//Stops the robot...
void ControlData::stopRobot(RobotIndex robotIndex) 
{
	xVel[robotIndex]    = 0.0f;
	yVel[robotIndex]    = 0.0f;
  rotVel[robotIndex]  = 0.0f;

  kick[robotIndex]            = NO_KICK;
  dribble[robotIndex]         = NO_DRIBBLE;
  verticalDribble[robotIndex] = NO_V_DRIBBLE;
  beep[robotIndex]            = NO_BEEP;
}


//makes robot drift. Will be used when the robot is lost...
void ControlData::driftRobot(RobotIndex robotIndex,float frameRate) {
  tSample=tSample+(1/frameRate);
  float k = 0.5f;
  float omega = 1.0f;
  float amplitude = 0.005f;//cm

	xVel[robotIndex]    = amplitude*omega*COS(amplitude*omega);
	yVel[robotIndex]    = amplitude*omega*SIN(amplitude*omega);
  rotVel[robotIndex]  = k;

  kick[robotIndex]            = NO_KICK;
  dribble[robotIndex]         = NO_DRIBBLE;
  verticalDribble[robotIndex] = NO_V_DRIBBLE;
  beep[robotIndex]            = NO_BEEP;

  if (tSample>2*PI) { tSample=0; }
}
