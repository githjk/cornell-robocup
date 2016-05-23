/*
=========================================================================================

	OPENLOOP_CONSOLE.H

=========================================================================================
*/

#ifndef OPENLOOP_CONSOLE_H_
#define OPENLOOP_CONSOLE_H_

#include <stdio.h>
//#include "AI_Types.h"
#include "../datatypes/ai.h"
#include "../modules/WirelessModule.h"

void initOpenLoopConsole(int use2000robots);

void clearHeaderBox(int use2000robots);
//void ResetToStatus();
//void clearStatusBox();
void clearBox();

void conprintNumJoysticks(int numJoyAttached);
void conprintJoyAcc(float joyAcc);
void conprintJoyV(float JoyV);
void conprintJoyW(float JoyW);
void conprintData(const unsigned char* data_packet);
void conprintRotation(float angle);
void conprintCommand(const RobotInfo* command, RobotIndex robot, bool yDisabled);
void conprintWheelCommands(const WirelessModule& wireless); 
void conprintYDisabled(bool disabled);

void conprintJoystick(int j_index);
void conprintJoySelected(int joystick, int robot);
void conprintJoyCommands(int j, JOYINFOEX jiex);
void conprintJoyError(int j, MMRESULT status);

// Direct all debugging output to the status box
void printStatus(char* message);
void printError(char* message);
void printLoopTime(double loopTime, int msPerLoop);

void drawTick(int x);
void drawZero(int x);
void drawOne(int x);


#endif	// OPENLOOP_CONSOLE_H_

