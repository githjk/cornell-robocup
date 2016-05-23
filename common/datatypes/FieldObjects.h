//===============================================================================
// FieldObjects.h
//
// Defines all data structures that are used to pass data between Vision and AI
//
// Created 11/8/99 by jbp4
//===============================================================================

#ifndef __FIELDOBJECTS_H__
#define __FIELDOBJECTS_H__


#include "vision_globals.h"

#define NUM_PLAYERS_ON_TEAM	5
#define NUM_BALLS_PER_TEAM 1

//===============================================================================
// FieldObject Class Definition
//===============================================================================
struct FieldObject 
{
	float XPos;		//X position, in meters
	float YPos;		//Y position, in meters

	float XVel;		//Velocity Parameters
	float YVel;		//in meters per second

	float Rotation;	//Rotation of object, in  radians
};


struct FieldData 
{
  long		timeStamp;
  double		startTime;
  double		finishTime;
  int			instruction;				//Instruction being sent to AI
  bool		robotsFound[::NUM_TEAMS][NUM_PLAYERS_ON_TEAM];//true == robot found, false == not found
  bool		ballsFound[::NUM_TEAMS][NUM_BALLS_PER_TEAM];//true == ball found, 0 == not found

  FieldObject ball[::NUM_TEAMS][NUM_BALLS_PER_TEAM];
  char ballSource[::NUM_TEAMS][NUM_BALLS_PER_TEAM];
  FieldObject team[::NUM_TEAMS][NUM_PLAYERS_ON_TEAM];
};

struct RobotInfo 
{
	double		xVel;			//left wheel velocity
	double		yVel;		//Right wheel velocity
	double		thetaVel;		//Forward velocity
	int			parameter;				//Kicker, Dribbler, etc
};

struct AIData 
{
	int		Instruction;				//Instruction being sent to Vision
	long		TimeStamp;
	RobotInfo	robot_instructs[NUM_PLAYERS_ON_TEAM];	//Instructions for the robots
};

#endif
