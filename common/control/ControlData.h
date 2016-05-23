#ifndef CONTROL_DATA_H
#define CONTROL_DATA_H

typedef enum
{
  NO_ROBOT      =   -1,
  ROBOT0,
  ROBOT1,
  ROBOT2,
  ROBOT3,
  ROBOT4,
  NUM_ROBOTS    //must equal NUM_PLAYERS_ON_TEAM (defined in field.h)
} RobotIndex;

//Don't call this if ind == NUM_ROBOTS
__inline RobotIndex operator++(RobotIndex& ind, int postfix) { //postfix operator
  RobotIndex old = ind;
  ind = static_cast<RobotIndex>(static_cast<int>(ind) + 1);
  return old;
}

//Don't call this if ind == NO_ROBOT
__inline RobotIndex operator--(RobotIndex& ind, int postfix) { //postfix operator
  RobotIndex old = ind;
  ind = static_cast<RobotIndex>(static_cast<int>(ind) - 1);
  return old;
}

//----------------------------------------------------------------------
typedef enum
{
  NO_KICK,
  KICK_SLOW, //often referred to as "Kick Suck"
  KICK_PASS,
  KICK_PASS_IMMEDIATE,
  KICK_WEAK_1,
  KICK_WEAK_2,
  KICK_WEAK_3,
  KICK_WEAK_4,
  KICK_SHOT,
  KICK_SHOT_IMMEDIATE
} KickSpeed;

//----------------------------------------------------------------------
typedef enum
{
  NO_DRIBBLE,
  SLOW_DRIBBLE,
  MID_DRIBBLE,
  FAST_DRIBBLE
} DribbleSpeed;

extern DribbleSpeed DRIBBLE_DEFAULT;

//----------------------------------------------------------------------
typedef enum
{
  NO_V_DRIBBLE,
  SLOW_V_DRIBBLE,
  MID_V_DRIBBLE,
  FAST_V_DRIBBLE
} VerticalDribbleSpeed;

extern VerticalDribbleSpeed V_DRIBBLE_DEFAULT;

//----------------------------------------------------------------------
typedef enum
{
  NO_BEEP,
  YES_BEEP
} BeepCommand;


//----------------------------------------------------------------------
class ControlData {

	friend class ControlModule;

public:

  ControlData();

  //----------------------------------------------------------------------
  float getRotVel(int robotIndex) { return rotVel[robotIndex]; }
  float getXVel (int robotIndex)  { return xVel[robotIndex]; }
  float getYVel (int robotIndex)  { return yVel[robotIndex]; }

  void setRotVel(int robotIndex, float vel) { rotVel[robotIndex] = vel; }
  void setXVel  (int robotIndex, float vel) {   xVel[robotIndex] = vel; }
  void setYVel  (int robotIndex, float vel) {   yVel[robotIndex] = vel; }

  //----------------------------------------------------------------------
  void setKick (RobotIndex robotIndex, KickSpeed kickSpeed) { kick[robotIndex] = kickSpeed; }
  KickSpeed getKick (RobotIndex robotIndex) const { return kick[robotIndex]; }

  void setDribble (RobotIndex robotIndex, DribbleSpeed d) { dribble[robotIndex] = d; }
  DribbleSpeed getDribble (RobotIndex robotIndex) const { return dribble[robotIndex]; }

  void setVerticalDribble (RobotIndex robotIndex, VerticalDribbleSpeed d) { verticalDribble[robotIndex] = d; }
  VerticalDribbleSpeed getVerticalDribble (RobotIndex robotIndex) const { return verticalDribble[robotIndex]; }
  
  void setBeep (RobotIndex robotIndex, BeepCommand b) { beep[robotIndex] = b; }
  BeepCommand getBeep (RobotIndex robotIndex) const { return beep[robotIndex]; }


  //----------------------------------------------------------------------
  // Set robot velocities manually
  void setRobotVelocity(  RobotIndex robotIndex, 
                          float xVelocity,
                          float yVelocity,
                          float rotVelocity
  );

  // Set robot commands manually
  void setRobotCommands(  RobotIndex robotIndex,
                          KickSpeed kickCommand,
                          DribbleSpeed dribbleCommand,
                          VerticalDribbleSpeed sideDribbleCommand,
                          BeepCommand beepCommand
  );

  //----------------------------------------------------------------------
  //Stops the robot...
  void stopRobot(RobotIndex robotIndex);

  //makes robot drift. Will be used when the robot is lost...
  void driftRobot(RobotIndex robotIndex,float frameRate);

  //----------------------------------------------------------------------
private:

	float xVel[NUM_ROBOTS];
	float yVel[NUM_ROBOTS];
	float rotVel[NUM_ROBOTS];

  KickSpeed             kick[NUM_ROBOTS];
  DribbleSpeed          dribble[NUM_ROBOTS];
  VerticalDribbleSpeed  verticalDribble[NUM_ROBOTS];
  BeepCommand           beep[NUM_ROBOTS];

  float tSample;//used for the drift move...
};

#endif // CONTROL_DATA_H
