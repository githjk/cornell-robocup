/*
 *	Return values from ACTION_FUNCTION calls
 *
 *	0 - ERROR_BASE:
 *		Commands to switch into a new role or to switch the roles of two players
 *
 *	> ERROR_BASE:
 *		Some error occurred
 */

#ifndef ACTION_CODES_H
#define ACTION_CODES_H

#include <math.h>


/***********************************************************************************/

//For now, allow only one switch per frame
enum SwitchCommand {
  NO_ACTION = 0,

  SW_MAIN_FORWARD,		// Switch to MainForward
	SW_PASS_FORWARD,		// Switch to PassForward
	SW_MIDFIELDER,		  // Switch to Midfielder
	SW_ACTIVE_DEFENDER,		// Switch to ActiveDefender
	SW_GOALIE,		      // Switch to Goalie

// 	SW_SINGLE_DEFENDER,		// Switch to SingleDefender
//	SW_PASSBLOCK_DEFENDER,// Switch to PassBlockDefender

  SW_FORWARDS,	    	// Switch the two forwards
  SW_DEFENDERS,	    	// Switch the two defenders
  SW_MID_MAIN,    		// Switch the Midfielder and MainForward
  SW_MID_PASS,	    	// Switch the Midfielder and PassForward
  SW_OFF_THREE,	    	// Rotate the three offenders:
											  //		PassForward --> Midfielder
										    //		MainForward --> PassForward
											  //		MidFielder  --> MainForward

  SW_ACTIVE_MIDFIELDER,	// Switch the Active Defender and Midfielder
  SW_ACTIVE_PASS,	      // Switch the Active Defender and PassForward

//  SW_MID_SINGLE,	  	// Switch the Midfielder and Single Defender
//  SW_SINGLE_ACTIVE,		// Switch the Single to Active Defender
};


/*	Error codes are offset from ERROR_BASE
 *	
 *	Unless it is a major error, the program should report it, but continue
 */

#define ACTION_ERROR_BASE		0x10000

#define ERROR_TRAJECTORY		ACTION_ERROR_BASE + 1	// Problem with TrajGen.dll



/*
 *	Constants to set robot instructions
 *	
 */
/*
#define DRIBBLE		(0x0007)
#define KICK		(0x0700)
#define GRAB		(0x0024)
*/

//Format for more velocity bits (6/10/2000) -NS and Nok
const unsigned char PACKET_DRIBBLE_2000  =  (0x06); // 00000110
const unsigned char PACKET_KICK_2000 = (0x01); // 00000001

const unsigned char PACKET_BAR_DOWN_2000 =(0x08); // 00001000
const unsigned char PACKET_BAR_UP_2000 = (0x10) ;//  00010000


//New codes for Vertical Dribbling
// BYTE 4
const unsigned char PACKET_BEEP = (0x01); // 00000001

//const unsigned char PACKET_FAST_DRIBBLE  =  (0x06); // 00000110
//const unsigned char PACKET_MID_DRIBBLE  =  (0x06);  // 00000100
//const unsigned char PACKET_SLOW_DRIBBLE  =  (0x06); // 00000010
//2001-8-5  mth13:  implemented mid and slow speeds:
const unsigned char PACKET_FAST_DRIBBLE  =  (0x06); // 00000110
const unsigned char PACKET_MID_DRIBBLE  =  (0x04);  // 00000100
const unsigned char PACKET_SLOW_DRIBBLE  =  (0x02); // 00000010

const unsigned char PACKET_KICK_SHOT = (0x18); // 00011000
const unsigned char PACKET_KICK_PASS = (0x10); // 00010000
const unsigned char PACKET_KICK_SLOW = (0x08); // 00001000

//BYTE 3
const unsigned char PACKET_FAST_V_DRIBBLE = (0xC0);  // 11000000
const unsigned char PACKET_MID_V_DRIBBLE = (0xC0);   // 10000000
const unsigned char PACKET_SLOW_V_DRIBBLE = (0xC0);  // 01000000


#endif	//ACTION_CODES_H