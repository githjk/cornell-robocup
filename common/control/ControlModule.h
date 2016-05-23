#ifndef CONTROL_MODULE_H
#define CONTROL_MODULE_H

#include "ai.h"
#include "ControlData.h"
#include "Trajectory.h"
#include "VisionModule.h"
#include "StrategyModule.h"

#include "../network/Client.h"
#include "../wireless/WirelessSender.h"

/*********************
* Control Module     *
*********************/
class ControlModule
{
 
public:

  //------------------------------------------------------
  /* constructor */
  ControlModule (SystemParameters* sp, 
                 VisionModule* vision, 
                 StrategyModule* strategy);

  void updateParameters();

  //------------------------------------------------------
  //Returns wireless status (on/off)
  bool wirelessStatus();

  //Toggles wireless status (on/off)
  bool toggleWirelessStatus();

  //Sets wireless status (on/off)
  bool setWirelessStatus(bool newWirelessStatus);

  //------------------------------------------------------
  // retrieves current frame
  ControlData* getCurrentFrame(int frame=-1);

  // sets currentFrame (assume it is a valid frame)
  void setFrame(int newFrame);

  // sets currentFrame (assume it is a valid frame)
  ControlData* getControlArray(void);
  
  //------------------------------------------------------
  // sends commands to Robots
  void sendRPCCommands(Client* client);

  // sends commands to Simulator
  void sendSimulatorCommands(Client* client);

  //------------------------------------------------------
  //sets all robots to stop moving!
  void stopRobots();

  // Builds paths for each robot
  virtual void generatePaths();

  //------------------------------------------------------

  WirelessSender*   wireless;

protected:
  SystemParameters*   SP;
  VisionModule*       vision;
  StrategyModule*     strategy;

  bool                WirelessRunning;
  int                 currentFrame;
  ControlData         controlData[MAX_LOG];

  int kickTimer[NUM_ROBOTS];
  int delayKicker[NUM_ROBOTS];

  void parseKickSpeed(int inputSpeed, int robotID);
  void parseDribbleSpeed(int inputSpeed, int robotID);
  void parseVerticalDribbleSpeed(int inputSpeed, int robotID);

  int kickSpeed[NUM_ROBOTS];
  int dribbleSpeed[NUM_ROBOTS];
  int verticalDribbleSpeed[NUM_ROBOTS];

  ObjectPath robotPath[NUM_ROBOTS];

};

#endif // CONTROL_MODULE_H
