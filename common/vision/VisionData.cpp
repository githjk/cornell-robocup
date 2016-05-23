/** Implementation file for VisionData Class
**/

#include "VisionData.h"


VisionData::VisionData() { }

//------------------------------------------------------------------------
double VisionData::getStartTime (void) const { 
  return startTime; 
}
void VisionData::setStartTime (double s) { 
  startTime = s; 
}

double VisionData::getFinishTime (void) const { 
  return finishTime; 
}
void VisionData::setFinishTime (double f) { 
  finishTime = f; 
}

int VisionData::getInstruction (void) const { 
  return instruction; 
}
void VisionData::setInstruction (int i) { 
  instruction = i; 
}

//------------------------------------------------------------------------
const MovingObject* VisionData::getTeam (int team)  const { 
  return teams[team]; 
}

//------------------------------------------------------------------------

const MovingObject& VisionData::getRobot (int team, int robotIndex) const { 
  return teams[team][robotIndex]; 
}

void VisionData::setRobotFound(int team, int robotIndex, bool condition) { 
  robotsFound[team][robotIndex]=condition;
}

bool VisionData::isRobotFound (int team, int robotIndex) const { 
  return (robotsFound[team][robotIndex]); 
}

//------------------------------------------------------------------------

void VisionData::setRobotXPos(int team, int number, float pos) { 
  teams[team][number].setXPos(pos); 
}
void VisionData::setRobotYPos(int team, int number, float pos) { 
  teams[team][number].setYPos(pos); 
}
void VisionData::setRobotXVel(int team, int number, float pos) { 
  teams[team][number].setXVel(pos); 
}
void VisionData::setRobotYVel(int team, int number, float pos) { 
  teams[team][number].setYVel(pos); 
}
void VisionData::setRobotRotation(int team, int number, float pos) { 
  teams[team][number].setRotation(pos); 
}
void VisionData::setRobotRotVel(int team, int number, float pos) { 
  teams[team][number].setRotVel(pos); 
}

//------------------------------------------------------------------------

MovingObject VisionData::getBall(int team, int number) const
{ 
  return ball[team][number]; 
}

void VisionData::setBallFound(int team, int ballIndex, bool condition)
{ 
  ballsFound[team][ballIndex]=condition;
}

bool VisionData::isBallFound (int team, int ballIndex) const  
{ 
  return (ballsFound[team][ballIndex]); 
}

//------------------------------------------------------------------------

void VisionData::setBallPos(int team, int number, Pair pos) { 
  ball[team][number].setPos(pos); 
}
void VisionData::setBallXPos(int team, int number, float pos) { 
  ball[team][number].setXPos(pos); 
}
void VisionData::setBallYPos(int team, int number, float pos) { 
  ball[team][number].setYPos(pos); 
}
void VisionData::setBallXVel(int team, int number, float pos) { 
  ball[team][number].setXVel(pos); 
}
void VisionData::setBallYVel(int team, int number, float pos) { 
  ball[team][number].setYVel(pos); 
}

//------------------------------------------------------------------------


