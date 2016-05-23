#ifndef VISION_CONSTANTS_H
#define VISION_CONSTANTS_H

#include "ai.h"
/********************************
Constants Declarations
*********************************/
#define ROBOFLAG_SYSTEM
const int NUM_BALLS			    = 1;
const int NUM_OBSTACLES		    = 1;
const int VISION_FIRST_FRAME    = 0;
const int NUM_VISION_SNAPSHOTS  = 10;	// minimum of 2
const int ROBOCUP_BALL_TEAM		= 0;
const int ROBOCUP_BALL_INDEX	= 0;
const int TIMING_TEST_FRAMES	= 200;

// Parameters for Vision Packet. Indicates number of robots per team, balls, and number of obstacles etc.
const int VISION_PACKET_NUM_OBSTACLES = 0;
const int VISION_PACKET_NUM_BALLS     = 1;
const int VISION_PACKET_NUM_ROBOTS    = 5;

//Value to indicate bad prediction error calculation
const float BAD_PRED_ERR = -32000.0f;


struct manualObjectPlacement
{
	
	float friendlyRobotX[NUM_ROBOTS], friendlyRobotY[NUM_ROBOTS], friendlyRobotRotation[NUM_ROBOTS];
	float opponentRobotX[NUM_ROBOTS], opponentRobotY[NUM_ROBOTS], opponentRobotRotation[NUM_ROBOTS];
	float ballX[NUM_TEAMS][NUM_BALLS], ballY[NUM_TEAMS][NUM_BALLS];
};

extern manualObjectPlacement GUI_Placement;


#endif