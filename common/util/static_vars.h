/*
 *	STATIC_VARS.CPP
 */

#ifndef STATIC_VARS_H
#define STATIC_VARS_H

static float m, b;
static float xBot, yBot;
static float thetaBot;
static float xHold, yHold;
static float xTemp, yTemp;
static float ignoreX, ignoreY;
static float buf;
static float interceptBuffer;
static float closestDist;
static float xOther, yOther;
static float thetaOther;
static float xValue, yValue;
static float deltaX;

static float xBall, yBall;
static float xPass, yPass;
static float thetaPass;
static float xReceive, yReceive;
static float thetaReceive;

static bool isPath;
static int index;
static float score[4];
static int robotNumber[4];	
// static float xPos, yPos;
static Pair pos;
static float xDiff, yDiff;
static float tempScore;
static int tempID; 
		
static int theirGoalie;
static float xTarget;
static float yRightTarget;
static float yTarget;
static float yLeftTarget;
static bool openShot;

static int obstacles;
static int ID;

static float time1;
static float time2;

#define SEARCH_ARRAY_SIZE 200
static Pair recoverPoints[SEARCH_ARRAY_SIZE];
static int recoverPointIndex[NUM_ROBOTS];
static float ballReachTime[SEARCH_ARRAY_SIZE];



#endif // STATIC_VARS.H




