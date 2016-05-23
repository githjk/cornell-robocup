#ifndef ROBOT_COMMAND
#define ROBOT_COMMAND

class RobotCommand {
  float xVel;
  float yVel;
  float rotVel;

  int kick;
  int dribble;
  int vDribble;

  bool beep;
};


#endif