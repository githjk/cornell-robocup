/** Implementation file for RawVisionData Class
**/

#include "RawVisionData.h"


RawVisionData::RawVisionData() { }

//------------------------------------------------------------------------
const MovingObject* RawVisionData::getTeam (int team)  const { 
  return teams[team]; 
}

//------------------------------------------------------------------------

const MovingObject& RawVisionData::getRobot (int team, int robotIndex) const { 
  return teams[team][robotIndex]; 
}

void RawVisionData::setRobotFound(int team, int robotIndex, bool condition) { 
  robotsFound[team][robotIndex]=condition;
}

bool RawVisionData::isRobotFound (int team, int robotIndex) const { 
  return (robotsFound[team][robotIndex]); 
}

//------------------------------------------------------------------------

void RawVisionData::setRobotXPos(int team, int number, float pos) { 
  teams[team][number].setXPos(pos); 
}
void RawVisionData::setRobotYPos(int team, int number, float pos) { 
  teams[team][number].setYPos(pos); 
}
void RawVisionData::setRobotXVel(int team, int number, float pos) { 
  teams[team][number].setXVel(pos); 
}
void RawVisionData::setRobotYVel(int team, int number, float pos) { 
  teams[team][number].setYVel(pos); 
}
void RawVisionData::setRobotRotation(int team, int number, float pos) { 
  teams[team][number].setRotation(pos); 
}
void RawVisionData::setRobotRotVel(int team, int number, float pos) { 
  teams[team][number].setRotVel(pos); 
}

//------------------------------------------------------------------------

MovingObject RawVisionData::getBall(int team, int number) const
{ 
  return ball[team][number]; 
}

void RawVisionData::setBallFound(int team, int ballIndex, bool condition)
{ 
  ballsFound[team][ballIndex]=condition;
}

bool RawVisionData::isBallFound (int team, int ballIndex) const  
{ 
  return (ballsFound[team][ballIndex]); 
}

char RawVisionData::getBallSource(int team, int number) const
{
  return ballSource[team][number];
}

//------------------------------------------------------------------------


void RawVisionData::setBallSource(int team, int number, char source) {
  ballSource[team][number] = source;
}
void RawVisionData::setBallPos(int team, int number, Pair pos) { 
  ball[team][number].setPos(pos); 
}
void RawVisionData::setBallXPos(int team, int number, float pos) { 
  ball[team][number].setXPos(pos); 
}
void RawVisionData::setBallYPos(int team, int number, float pos) { 
  ball[team][number].setYPos(pos); 
}
void RawVisionData::setBallXVel(int team, int number, float pos) { 
  ball[team][number].setXVel(pos); 
}
void RawVisionData::setBallYVel(int team, int number, float pos) { 
  ball[team][number].setYVel(pos); 
}

//------------------------------------------------------------------------
float RawVisionData::getNormalizedTranslationalError(int robot)
{
  if(robot < 0 || robot >= NUM_PLAYERS_ON_TEAM)
  {
    return -1.0f;
  }
  else
  {
    return normalizedTranslationalError[robot];
  }
}
//------------------------------------------------------------------------
float RawVisionData::getNormalizedRotationalError(int robot)
{
  if(robot < 0 || robot >= NUM_PLAYERS_ON_TEAM)
  {
    return -1.0f;
  }
  else
  {
    return normalizedRotationalError[robot];
  }
}
//------------------------------------------------------------------------
bool RawVisionData::getResetTranslationalVelocity(int robot)
{
  if(robot < 0 || robot >= NUM_PLAYERS_ON_TEAM)
  {
    return false;
  }
  else
  {
    return resetTranslationalVelocity[robot];
  }
}
//------------------------------------------------------------------------
bool RawVisionData::getResetRotationalVelocity(int robot)
{
  if(robot < 0 || robot >= NUM_PLAYERS_ON_TEAM)
  {
    return false;
  }
  else
  {
    return resetRotationalVelocity[robot];
  }
}
//------------------------------------------------------------------------
float RawVisionData::getDistanceTravelled(int robot)
{
  if(robot < 0 || robot >= NUM_PLAYERS_ON_TEAM)
  {
    return false;
  }
  else
  {
    return distanceTravelled[robot];
  }
}
//------------------------------------------------------------------------
float RawVisionData::getRotationTravelled(int robot)
{
  if(robot < 0 || robot >= NUM_PLAYERS_ON_TEAM)
  {
    return false;
  }
  else
  {
    return rotationTravelled[robot];
  }
}
//------------------------------------------------------------------------
