/*
=========================================================================================

	OPENLOOP_CONSOLE.CPP

=========================================================================================

*/
#ifndef NO_VIS_CONSOLE

#include <windows.h>
#include <stdio.h>

#include "OpenLoopConsole.h"
#include "../modules/WirelessModule.h"

HANDLE gAIConsole;

// Measured blank lines
#define BL_10				    "          "
#define BL_20				    BL_10 BL_10
#define BL_40			    	BL_20 BL_20
#define BL_55			    	BL_40 BL_10 "     "
#define BL_75			    	BL_20 BL_55
#define BL_ENTIRE_LINE	BL_40 BL_40


// Console Size
const int BUFSIZE_X		= 80;
const int BUFSIZE_Y		= 32;

// Colors
#define BACKGROUND_WHITE	(BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN)
#define FOREGROUND_WHITE	(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN)

#define STATUS_COLOR		FOREGROUND_BLUE | BACKGROUND_GREEN
#define BOX_COLOR			FOREGROUND_WHITE
#define ALERT_COLOR			BACKGROUND_RED | FOREGROUND_WHITE

// Location of status line (Y)
#define STATUS_LINE			(BUFSIZE_Y - 2)

//Y-coords of certain blocks of lines
const int SELECTION_BLOCK_BEGIN = 4;
const int SPEED_BLOCK_BEGIN = 10;
const int JOYSTICK_BEGIN= 15;
const int ROBOT_BEGIN = 21;
const int PACKETS_BEGIN = 27;

static char buffer[BUFSIZE_X+1];
static char tempBuffer[BUFSIZE_X+1];


/* 
 * conprintf()
 * -----------
 * Print the specified string to a location on the console, with the specified text attributes
 */
void conprintf(char	*lpbuffer,		// string to output
			   int	x,				// starting x-position of text
			   int	y,				// starting y-position of text
			   WORD	wAttributes)	// attributes of output string
{
	static COORD cursorPos;
	unsigned long temp;

	cursorPos.X = x;
	cursorPos.Y = y;
	SetConsoleCursorPosition(gAIConsole, cursorPos);
	SetConsoleTextAttribute(gAIConsole, wAttributes);
	WriteConsole(gAIConsole, lpbuffer, strlen(lpbuffer), &temp, NULL);
}



void conprintNumJoysticks(int numJoyAttached){
	sprintf(buffer,"Joysticks found:        %6d", numJoyAttached);
	conprintf(buffer, 45, SELECTION_BLOCK_BEGIN, BOX_COLOR);
}

void conprintJoystick(int j_index){
	sprintf(buffer,"Selected Joystick:      %6d",j_index);
	conprintf(buffer, 45, SELECTION_BLOCK_BEGIN+1, BOX_COLOR);
}

void conprintJoyAcc(float joyAcc){
	sprintf(buffer, "Acceleration:       %7.2f m/s/s", joyAcc);
	conprintf(buffer, 45, SPEED_BLOCK_BEGIN, BOX_COLOR);
}

void conprintJoyV(float JoyV){
	sprintf(buffer,"Maximum Velocity:    %6.2f m/s", JoyV);
	conprintf(buffer, 45, SPEED_BLOCK_BEGIN+1, BOX_COLOR);
}

void conprintJoyW(float JoyW){
	sprintf(buffer,"Rotational Velocity: %6.2f rad/s", JoyW);
	conprintf(buffer, 45, SPEED_BLOCK_BEGIN+2, BOX_COLOR);
}

void conprintRotation(float angle){
	sprintf(buffer,"Robot Rotation:      %6.2f rad",angle);
	conprintf(buffer, 45, SPEED_BLOCK_BEGIN+3, BOX_COLOR);
}


void conprintJoySelected(int joystick, int robot){
	sprintf(buffer,"Joystick %d: Robot %d",joystick, robot);
	conprintf(buffer, 4, JOYSTICK_BEGIN+joystick, BOX_COLOR);
}

void conprintJoyCommands(int j, JOYINFOEX jiex){
	sprintf(buffer,"(%5d, %5d) %3x      ",
		jiex.dwXpos,
		jiex.dwYpos,
		jiex.dwButtons
	);
	conprintf(buffer, 25, JOYSTICK_BEGIN + j, BOX_COLOR);
}

void conprintJoyError(int j, MMRESULT error) {
 	conprintf(BL_55, 25, JOYSTICK_BEGIN + j, BOX_COLOR);

  switch(error)
  {
  case(MMSYSERR_NODRIVER):
  	conprintf("<<< Error: No driver found. >>>", 25, JOYSTICK_BEGIN + j, BOX_COLOR);
    break;
  case(MMSYSERR_INVALPARAM):
  	conprintf("<<< Internal error: bad parameter to JoyGetPos >>>", 25, JOYSTICK_BEGIN + j, BOX_COLOR);
    break;
  case(MMSYSERR_BADDEVICEID):
  	conprintf("<<< Internal error: bad deviceID. >>>", 25, JOYSTICK_BEGIN + j, BOX_COLOR);
    break;
  case(JOYERR_UNPLUGGED):
  	conprintf("<<< Error: joystick disconnected. >>>", 25, JOYSTICK_BEGIN + j, BOX_COLOR);
    break;
  default:
    sprintf(buffer, "<<< Unknown error code: 0x%x >>>", error);
  	conprintf(buffer, 25, JOYSTICK_BEGIN + j, BOX_COLOR);
    break;
  }
}


void conprintYDisabled(bool disabled){
  if(disabled)
	  conprintf("Disabled ", 42, SELECTION_BLOCK_BEGIN+3, BOX_COLOR);
  else
	  conprintf("Enabled  ", 42, SELECTION_BLOCK_BEGIN+3, BOX_COLOR);
}

void conprintCommand(const RobotInfo* command, RobotIndex robot, bool yDisabled){
  if(yDisabled)
  {
	  sprintf(buffer, "Robot %d: (%7.2f,    ---,%7.2f) %3x",
	  	robot,
	  	command[robot].xVel,
	  	command[robot].thetaVel,
	  	command[robot].parameter
	  );
  }
  else
  {
	  sprintf(buffer, "Robot %d: (%7.2f,%7.2f,%7.2f) %3x",
	  	robot,
	  	command[robot].xVel,
	  	command[robot].yVel,
	  	command[robot].thetaVel,
	  	command[robot].parameter
	  );
  }
	conprintf(buffer, 4, ROBOT_BEGIN + robot, BOX_COLOR);
}

void conprintWheelCommands(const WirelessModule& wireless){ 
  for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++)
  {
    const short* command = wireless.getWheelCommands(robot);
	  sprintf(buffer, "(%9d,%9d,%9d)",
	  	command[0],
	  	command[1],
	  	command[2]
	  );
	  conprintf(buffer, 47, ROBOT_BEGIN + robot, BOX_COLOR);
  }
}



/*
 * initOpenLoopConsole()
 * ---------------
 * Initialize the OpenLoop Console: set window size, clear boxes, set titles, reset the
 * cursor to the status bar.
 */
void initOpenLoopConsole(int use2000robots) {
	gAIConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT winRect;
	COORD bufSize;

	// Set window and buffer size
	bufSize.X = BUFSIZE_X;
	bufSize.Y = BUFSIZE_Y;

	winRect.Left = 0;
	winRect.Top = 0;
	winRect.Right = bufSize.X - 1;
	winRect.Bottom = bufSize.Y - 1;

	SetConsoleTitle("OMNI Open Loop Test Console");

	if(!SetConsoleScreenBufferSize(gAIConsole, bufSize)) return;
	if(!SetConsoleWindowInfo(gAIConsole, TRUE, &winRect)) return;


	// Print main box
	clearBox();

	// Make status line blank
	conprintf(BL_ENTIRE_LINE, 0, STATUS_LINE, STATUS_COLOR); 

	// Print window header
  clearHeaderBox(use2000robots );

	// Move cursor back to status line
//	ResetToStatus();
}





void drawTick(int x, int y)
{
	conprintf(" ", x+0, y+0, BACKGROUND_RED);
}

void drawZero(int x, int y)
{
  conprintf(" ", x+0, y+0, BACKGROUND_RED);
  conprintf(" ", x+1, y+0, BACKGROUND_RED);
  conprintf(" ", x+2, y+0, BACKGROUND_RED);
  conprintf(" ", x+0, y+1, BACKGROUND_RED);
  conprintf(" ", x+2, y+1, BACKGROUND_RED);
  conprintf(" ", x+0, y+2, BACKGROUND_RED);
  conprintf(" ", x+1, y+2, BACKGROUND_RED);
  conprintf(" ", x+2, y+2, BACKGROUND_RED);
}

void drawOne(int x, int y)
{
  conprintf(" ", x+0, y+0, BACKGROUND_RED);
  conprintf(" ", x+1, y+0, BACKGROUND_RED);
  conprintf(" ", x+1, y+1, BACKGROUND_RED);
  conprintf(" ", x+0, y+2, BACKGROUND_RED);
  conprintf(" ", x+1, y+2, BACKGROUND_RED);
  conprintf(" ", x+2, y+2, BACKGROUND_RED);
}

/*
 * Clear the title header
 */
void clearHeaderBox(int use2000robots) {

	for(int i=0; i<=2; i++)		
		conprintf(BL_ENTIRE_LINE, 0, i, BACKGROUND_GREEN);

	conprintf("OMNI OPEN LOOP TEST CONSOLE", 30, 1, BACKGROUND_GREEN);


  drawTick(8, 0);
  drawZero(10, 0);

  drawTick(BUFSIZE_X-17, 0);
  drawZero(BUFSIZE_X-15, 0);

  if(use2000robots != 0)
  {
    drawZero(14, 0);
    drawZero(BUFSIZE_X-11, 0);
  }
  else
  {
    drawOne(14, 0);
    drawOne(BUFSIZE_X-11, 0);
  }

}

/*
 * Clear the title header
 */
void clearBox() {

	for(int i=4; i<BUFSIZE_Y; i++)		
		conprintf(BL_ENTIRE_LINE, 0, i, BOX_COLOR);

	sprintf(buffer, " 1  2  3  4  P| 1  2  3  4  P| 1  2  3  4  P| 1  2  3  4  P| 1  2  3  4  P ");
	conprintf(buffer, 3, PACKETS_BEGIN, BOX_COLOR | BACKGROUND_BLUE);

//	conprintf("HIT RETURN TO START USING RPC",			4,  4, BOX_COLOR);

	conprintf("    0-4:   Select Robot 0-4",			4,	SELECTION_BLOCK_BEGIN, BOX_COLOR);
	conprintf("q,w,e,r:   Select Joystick 0,1,2,3",	4,	SELECTION_BLOCK_BEGIN + 1, BOX_COLOR);
	conprintf("      t:   Toggle year (2000/2001)",	4,	SELECTION_BLOCK_BEGIN + 2, BOX_COLOR);
  conprintf("      y:   Toggle sideways movement:",	4,	SELECTION_BLOCK_BEGIN + 3, BOX_COLOR);

	conprintf("  (a)/(z)     +/- Acceleration",			4,	SPEED_BLOCK_BEGIN, BOX_COLOR);
	conprintf("(k,+)/(m,-)   +/- Max. Velocity",			4,	SPEED_BLOCK_BEGIN+1, BOX_COLOR);
	conprintf("  (j)/(n)     +/- Rotational",			4,	SPEED_BLOCK_BEGIN+2, BOX_COLOR);
	conprintf("  ([)/(])     Rotate Left/Right",		4,	SPEED_BLOCK_BEGIN+3, BOX_COLOR);

  conprintf("Velocities:     x:      y:   theta:",		2,	ROBOT_BEGIN-1, BOX_COLOR);
  conprintf("  Wheel 1:  Wheel 2:  Wheel 3:",		47,	ROBOT_BEGIN-1, BOX_COLOR);


}


void conprintData(const unsigned char* data_packet){

	sprintf(buffer, "");

	//data_packet = Get_Data();
	for(int i=0; i < 25; i++)
	{
		sprintf(tempBuffer, "%2x ",data_packet[i]);
		strcat(buffer,tempBuffer);
	}
	conprintf(buffer, 3, PACKETS_BEGIN+1, BOX_COLOR);
}




// Direct all debugging output to the status box
void printStatus(char* message)
{
	conprintf(BL_ENTIRE_LINE, 0, STATUS_LINE, STATUS_COLOR); 
  conprintf(message, 2, STATUS_LINE, STATUS_COLOR); 
}

void printError(char* message)
{
	conprintf(BL_ENTIRE_LINE, 0, STATUS_LINE, ALERT_COLOR); 
  conprintf(message, 2, STATUS_LINE, ALERT_COLOR); 
}

void printLoopTime(double loopTime, int msPerLoop)
{
  if(loopTime > msPerLoop)
  {
    //The trip through the loop took too long.  This isn't the end of the world,
    // but if accurate acceleration is improtant you should decrease
    // NUM_ITERATIONS_PER_SECOND or make sure no other programs are running.
    //
    // Long loops can also be caused by bad joysticks.  Look for error messages from the joysticks.
    sprintf(buffer, "Warning: the loop took %2.1lf ms instead of the expected %d ms.   ", loopTime, msPerLoop);
    conprintf(buffer, 2, STATUS_LINE+1, BOX_COLOR); 
  }
  else
  {
    conprintf(BL_75, 0, STATUS_LINE+1, BOX_COLOR);  //if you print 80 spaces, the line will wrap
  }
}


#endif  //NO_VIS_CONSOLE
