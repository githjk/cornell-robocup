/*********************************************************
 *                                                       *
 *  Open Loop Testing Code                               *
 *  ----------------------                               *
 *                                                       *
 * Uses the WirelessModule to control the 2000           *
 * or 2001 robots. Reads in a text file with velocities  *
 * and dibble/kick commands. The text file has the       *
 * following format:                                     *
 *                                                       *
 * xVelocity  yVelocity  rotVelocity  Dribble Kick       *
 *                                                       *
 * The velocities are in m/s, and the dribble/kick       *
 * flags are just one or zero. The x-axis is in the      *
 * direction that the robot is initially facing. All     *
 * values are floats                                     *
 *                                                       *
 * Example:                                              *
 * .9 .4 0 1 0                                           *
 * This moves the robot forward at .9 m/s and left at    *
 * .4 m/s without turning. The dribbler is on, but the   *
 * kick is not armed.                                    *
 *                                                       *
 * June 19, 2001 -- Kent Cseh                            *
 *********************************************************/

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "../modules/WirelessModule.h"
//#include "OpenLoopConsole.h"

#define INPUT_FILE "openLoopCommands.txt"
#define FRAME_LIMIT 500
const int NUM_ITERATIONS_PER_SECOND = 60; //16 ms per iteration
#define USE_DRIBBLE_COMMAND true
#define USE_KICK_COMMAND true
#define USE_VERTICAL_DRIBBLE_COMMAND true




#define LOW		1000
#define	MID		32768
#define HIGH	60000

//Send 15 kick signals for each pulse event...
#define NUM_KICK_SIGNALS	15

const int DEFAULT_PARALLEL_PORT_BASE_ADDR = 0x378;


class CommandFrame {
public:
  CommandFrame() {
    xVel=yVel=rotVel=0.0f;
    dribble=kick=vDribble=0.0f;
  }

  float xVel,yVel,rotVel;
  float dribble,kick,vDribble;

};

class OpenLoop {
public:
  OpenLoop() { numCommands = 0; };
  void run(SystemParameters* pseudoParams);

protected:
  CommandFrame commands[FRAME_LIMIT];
  int readCommands();
  void readFrame(CommandFrame* comLine);

  ifstream file;    //input filename is specified in INPUT_FILE
  int numCommands;

};