// VisionPacket.cpp: implementation of the VisionPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "VisionPacket.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VisionPacket::VisionPacket(int robotNum, int ballNum, int obstacleNum)
: packet(NULL)
{
	this->robotNum = robotNum;
	this->ballNum = ballNum;
  this->obstacleNum = obstacleNum;

	// calculating size of the packet
	totalSize = 
    // initial time stamp
    sizeof(long) + 
    // start & finish time
    sizeof(double)*2 +
    // instruction
    sizeof(int) +
    // team1 & team2 robots
    2*packetRobotSize*robotNum +
    // balls
    packetBallSize*ballNum +
    // obstacles
    packetObstacleSize*obstacleNum;

	// allocating the array
	packet = new char[totalSize];

	// Offset calculation for easy manipulation
	timeStampOffSet = 0;
	startTimeOffSet = timeStampOffSet + sizeof(long);
	finishTimeOffSet= startTimeOffSet + sizeof(double);
	instructionOffSet = finishTimeOffSet + sizeof(double);
	friendlyRobotOffSet = instructionOffSet + sizeof(int);
	opponentRobotOffSet = friendlyRobotOffSet + packetRobotSize*robotNum;
	ballOffSet = opponentRobotOffSet+ packetRobotSize*robotNum;
  obstacleOffSet = ballOffSet+ packetBallSize*ballNum;
	foundOffSet = 0;
	xPosOffSet  = foundOffSet + sizeof(bool);
	yPosOffSet  = xPosOffSet + sizeof(float);
  sourceOffSet = yPosOffSet + sizeof(float);
	rotationOffSet = yPosOffSet + sizeof(float);
}

void VisionPacket::setTimeStamp(long timeStamp)
{
	*((long *)(packet+timeStampOffSet)) = timeStamp;
}

long VisionPacket::getTimeStamp()
{
	return *((long *)(packet+timeStampOffSet));
}


void VisionPacket::setStartTime(double startTime)
{
	*((double *)(packet+startTimeOffSet)) = startTime;
}

double VisionPacket::getStartTime()
{
	return *((double *)(packet+startTimeOffSet));
}

void VisionPacket::setFinishTime(double finishTime)
{
	*((double *)(packet+finishTimeOffSet)) = finishTime;
}

double VisionPacket::getFinishTime()
{
	return *((double *)(packet+finishTimeOffSet));
}

void VisionPacket::setInstruction(int instruction)
{
	*((int *)(packet+instructionOffSet)) = instruction;
}

int VisionPacket::getInstruction()
{
	return *((int *)(packet+instructionOffSet));
}


void VisionPacket::setFriendlyRobot(int index, bool found, float xpos, float ypos, float rotation)
{
	*( (bool *)(packet+friendlyRobotOffSet+packetRobotSize*index+foundOffSet)) = found;
	*((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+xPosOffSet))  = xpos;
	*((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+yPosOffSet))  = ypos;
	*((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+rotationOffSet)) = rotation;
}
	
bool VisionPacket::getFriendlyRobot(int index, bool &found , float &xpos, float &ypos, float &rotation)
{
	xpos =     *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+xPosOffSet));
	ypos =     *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+yPosOffSet));
	rotation = *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+rotationOffSet));
  found =     *((bool *)(packet+friendlyRobotOffSet+packetRobotSize*index+foundOffSet));
	return      *((bool *)(packet+friendlyRobotOffSet+packetRobotSize*index+foundOffSet));
}


bool VisionPacket::isFriendlyRobotFound(int index)
{
  return *((bool *)(packet+friendlyRobotOffSet+packetRobotSize*index+foundOffSet));
}

VisionPacket::setFriendlyRobotFound(int index, bool found)
{
  *((bool *)(packet+friendlyRobotOffSet+packetRobotSize*index+foundOffSet)) = found;
}

float VisionPacket::getFriendlyRobotXPos(int index)
{
  return *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+xPosOffSet));
}

VisionPacket::setFriendlyRobotXPos(int index, float xpos)
{
  *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+xPosOffSet)) = xpos;
}

float VisionPacket::getFriendlyRobotYPos(int index)
{
  return *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+yPosOffSet));
}

VisionPacket::setFriendlyRobotYPos(int index, float ypos)
{
  *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+yPosOffSet)) = ypos;
}

float VisionPacket::getFriendlyRobotRotation(int index)
{
  return *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+rotationOffSet));
}

VisionPacket::setFriendlyRobotRotation(int index, float rotation)
{
  *((float *)(packet+friendlyRobotOffSet+packetRobotSize*index+rotationOffSet))=rotation;
}

	
void VisionPacket::setOpponentRobot(int index, bool found, float xpos, float ypos, float rotation)
{
	*( (bool *)(packet+opponentRobotOffSet+packetRobotSize*index+foundOffSet)) = found;
	*((float *)(packet+opponentRobotOffSet+packetRobotSize*index+xPosOffSet)) = xpos;
	*((float *)(packet+opponentRobotOffSet+packetRobotSize*index+yPosOffSet)) = ypos;
	*((float *)(packet+opponentRobotOffSet+packetRobotSize*index+rotationOffSet)) = rotation;
}

bool VisionPacket::getOpponentRobot(int index,bool &found, float &xpos, float &ypos, float &rotation)
{
	xpos =     *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+xPosOffSet));
	ypos =     *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+yPosOffSet));
	rotation = *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+rotationOffSet));
  found =     *((bool *)(packet+opponentRobotOffSet+packetRobotSize*index+foundOffSet));
	return      *((bool *)(packet+opponentRobotOffSet+packetRobotSize*index+foundOffSet));
}

bool VisionPacket::isOpponentRobotFound(int index)
{
  return *((bool *)(packet+opponentRobotOffSet+packetRobotSize*index+foundOffSet));
}

VisionPacket::setOpponentRobotFound(int index, bool found)
{
  *((bool *)(packet+opponentRobotOffSet+packetRobotSize*index+foundOffSet)) = found;
}

float VisionPacket::getOpponentRobotXPos(int index)
{
  return *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+xPosOffSet));
}

VisionPacket::setOpponentRobotXPos(int index, float xpos)
{
  *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+xPosOffSet)) = xpos;
}

float VisionPacket::getOpponentRobotYPos(int index)
{
  return *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+yPosOffSet));
}


VisionPacket::setOpponentRobotYPos(int index, float ypos)
{
  *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+yPosOffSet)) = ypos;
}

float VisionPacket::getOpponentRobotRotation(int index)
{
  return *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+rotationOffSet));
}

VisionPacket::setOpponentRobotRotation(int index, float rotation)
{
  *((float *)(packet+opponentRobotOffSet+packetRobotSize*index+rotationOffSet)) = rotation;
}

void VisionPacket::setObstacle(int index, bool found, float xpos, float ypos, float rotation)
{
	*( (bool *)(packet+obstacleOffSet+packetObstacleSize*index+foundOffSet)) = found;
	*((float *)(packet+obstacleOffSet+packetObstacleSize*index+xPosOffSet)) = xpos;
	*((float *)(packet+obstacleOffSet+packetObstacleSize*index+yPosOffSet)) = ypos;
	*((float *)(packet+obstacleOffSet+packetObstacleSize*index+rotationOffSet)) = rotation;
}

bool VisionPacket::getObstacle(int index, bool &found, float &xpos, float &ypos, float &rotation)
{
	xpos =     *((float *)(packet+obstacleOffSet+packetObstacleSize*index+xPosOffSet));
	ypos =     *((float *)(packet+obstacleOffSet+packetObstacleSize*index+yPosOffSet));
  found =     *((bool *)(packet+obstacleOffSet+packetObstacleSize*index+foundOffSet));
	rotation = *((float *)(packet+obstacleOffSet+packetObstacleSize*index+rotationOffSet));
	return      *((bool *)(packet+obstacleOffSet+packetObstacleSize*index+foundOffSet));
}

bool VisionPacket::isObstacleFound(int index)
{
  return *((bool *)(packet+obstacleOffSet+packetObstacleSize*index+foundOffSet));
}

VisionPacket::setObstacleFound(int index, bool found)
{
  *((bool *)(packet+obstacleOffSet+packetObstacleSize*index+foundOffSet)) = found;
}

float VisionPacket::getObstacleXPos(int index)
{
  return *((float *)(packet+obstacleOffSet+packetObstacleSize*index+xPosOffSet));
}

VisionPacket::setObstacleXPos(int index, float xpos)
{
  *((float *)(packet+obstacleOffSet+packetObstacleSize*index+xPosOffSet)) = xpos;
}

float VisionPacket::getObstacleYPos(int index)
{
  return *((float *)(packet+obstacleOffSet+packetObstacleSize*index+yPosOffSet));
}

VisionPacket::setObstacleYPos(int index, float ypos)
{
  *((float *)(packet+obstacleOffSet+packetObstacleSize*index+yPosOffSet)) = ypos;
}

float VisionPacket::getObstacleRotation(int index)
{
  return *((float *)(packet+obstacleOffSet+packetObstacleSize*index+rotationOffSet));
}

VisionPacket::setObstacleRotation(int index, float rotation)
{
  *((float *)(packet+obstacleOffSet+packetObstacleSize*index+rotationOffSet))= rotation;
}


bool VisionPacket::getBall(int index, bool &found, float &xpos, float &ypos,char &source)
{
	xpos = *((float *)(packet+ballOffSet+packetBallSize*index+xPosOffSet));
	ypos = *((float *)(packet+ballOffSet+packetBallSize*index+yPosOffSet));
  found = *((bool *)(packet+ballOffSet+packetBallSize*index+foundOffSet));
  source = *((char *)(packet+ballOffSet+packetBallSize*index+sourceOffSet));
	return  *((bool *)(packet+ballOffSet+packetBallSize*index+foundOffSet));
}

/// returns ball is found, and outputs parameters in pointers that is being passed
bool VisionPacket::getBall(int index, bool &found, float &xpos, float &ypos)
{
  char source;
  return getBall(index,found, xpos, ypos,source);
}


bool VisionPacket::isBallFound(int index)
{
  return *((bool *)(packet+ballOffSet+packetBallSize*index+foundOffSet));
}

float VisionPacket::getBallXPos(int index)
{
  return *((float *)(packet+ballOffSet+packetBallSize*index+xPosOffSet));
}

float VisionPacket::getBallYPos(int index)
{
  return *((float *)(packet+ballOffSet+packetBallSize*index+yPosOffSet));
}

char VisionPacket::getBallSource(int index)
{
  return *((char *)(packet+ballOffSet+packetBallSize*index+sourceOffSet));
}

void VisionPacket::setBall(int index, bool found, float xpos, float ypos,char source)
{
	*((float *)(packet+ballOffSet+packetBallSize*index+xPosOffSet))  =  xpos;
	*((float *)(packet+ballOffSet+packetBallSize*index+yPosOffSet))  =  ypos;;
	*((bool *)(packet+ballOffSet+packetBallSize*index+foundOffSet)) = found;
  *((char *)(packet+ballOffSet+packetBallSize*index+sourceOffSet))  = source;
}


VisionPacket::setBallXPos(int index, float xpos)
{
  *((float *)(packet+ballOffSet+packetBallSize*index+xPosOffSet)) = xpos;
}

VisionPacket::setBallYPos(int index, float ypos)
{
  *((float *)(packet+ballOffSet+packetBallSize*index+yPosOffSet)) = ypos;
}

VisionPacket::setBallSource(int index, char source)
{
  *((char *)(packet+ballOffSet+packetBallSize*index+sourceOffSet)) = source;
}


int VisionPacket::getTotalSize()
{
	return totalSize;
}

char* VisionPacket::getPacket()
{
	return packet;
}

void VisionPacket::setPacket(char* packetPointer)
{
  memcpy(packet,packetPointer,totalSize);
}

VisionPacket::~VisionPacket()
{
	if (packet!=NULL)
  {
    delete[] packet;
    packet = NULL;
  }
}


