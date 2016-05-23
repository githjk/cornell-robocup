#ifndef ROBOFLAG_WIRELESS_PACKET_H
#define ROBOFLAG_WIRELESS_PACKET_H

#include "RCSerialPortInterface.h"


//#define TOTAL_PACKET_SIZE (1*PACKET_SIZE)
#define TOTAL_PACKET_SIZE (2*PACKET_SIZE)  //changed for robot testing
//#define NUM_AGENTS 6
#define NUM_AGENTS 12

typedef enum
{
  ROBOT_2000 = 0,
  ROBOT_2001,
  ROBOT_2002,
  NUM_ROBOT_TYPES
} RobotType;

// Data Structure to hold Robot Commands
class RobotCommand {

public:
  float xVel;
  float yVel;
  float rotVel;

  int kick;
  int dribble;
  int vDribble;

  bool dive;
};


// Class for sending wireless packets to the robots
class WirelessSender {

public:

  WirelessSender(char* portName);
  ~WirelessSender();

	void setRobotCommand (int index, RobotCommand* command);
	void setRobotVelocity(int index, float xVel, float yVel, float rotVel);
	void setRobotActions (int index, int kick, int dribble, int vDribble);

  void setRobotRotation(int index, float rotation);
  void setRobotType(int index, RobotType newRobotType);
  RobotType getRobotType(int index);

  void getWheelCommands(int index, RobotCommand* command);
  void getWheelCommands(int index, 
                        float &xVel, 
                        float &yVel, 
                        float &rotVel);
  void buildPacket();
	int sendCommands(bool secondPacket = false);

	void getPacket(char packet[TOTAL_PACKET_SIZE]);

  void setBoard(int type);
private:

  void stopRobot(int robot, int offsetByte);
  void buildRobot2002(int robot, int offsetByte);
  void buildRobot2001(int robot, int offsetByte);
  void buildRobot2000(int robot, int offsetByte);

  // Serial Port Interface
  RCSerialPortInterface RCSPort;

  // Data packet
  unsigned char dataPacket[TOTAL_PACKET_SIZE];

  // Sets whether this is an old 2000 robot or a new 2001 robot
  RobotType robotType[NUM_AGENTS];

  // Robot Rotations
  float rotations[NUM_AGENTS];

  // Robot Commands
  RobotCommand commands[TOTAL_PACKET_SIZE];

  // Wheel Commands
  RobotCommand wheels[TOTAL_PACKET_SIZE];

  //Which wireless board are we using
  //1 = JinWoo Board
  //2 = Michael Jordan Board
  int wirelessBoard;

};


#endif WIRELESS_MODULE_H