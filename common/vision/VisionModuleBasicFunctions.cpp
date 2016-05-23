////////////////////////////////////////////////////////////////////
//  VisionModule.CPP
//
//  This file contains all the functions necessary for processing
//  raw vision data 
//
////////////////////////////////////////////////////////////////////

#include "VisionModule.h"

Pair VisionModule::getLastUnpredictedBall()
{
  return lastUnpredictedBall;
}

//==================================================================
//==================================================================
//==================================================================
bool VisionModule::visionStatus() 
{ 
  return visionRunning; 
}

bool VisionModule::toggleVisionStatus()
{
  visionRunning = !visionRunning; 
  return visionRunning; 
}

bool VisionModule::setVisionStatus(bool newVisionStatus)
{
  
  visionRunning = newVisionStatus;
  return visionRunning;
}

bool VisionModule::areSidesFlipped() 
{ 
  return sidesFlipped; 
}

//==================================================================
//==================================================================
//==================================================================
VisionData* VisionModule::getCurrentFrame(int frame)
{
  //if frame # unspecified, take current frame
  if(frame==-1)
    return &visionData[currentFrame];
  //else, select specified frame
  else
    return &visionData[frame];
}

void VisionModule::setFrame(int newFrame) 
{
  currentFrame = newFrame;
}
//==================================================================
//==================================================================
//==================================================================
/// retrieves raw frame
RawVisionData* VisionModule::getRawFrame(int frame)
{
  //if frame # unspecified, take current frame
  if(frame==-1)
    return &rawVisionData[currentFrame];
  //else, select specified frame
  else
    return &rawVisionData[frame];
}
//==================================================================
//==================================================================
//==================================================================

bool VisionModule::isRobotFound(int team,int num, int frame)  
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].isRobotFound(team,num);
  //else, use current frame
  else
    return visionData[currentFrame].isRobotFound(team,num);
}

float VisionModule::getOurRobotX(int id, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.TEAM,id).getXPos();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.TEAM,id).getXPos();
}

float VisionModule::getOurRobotY(int id, int frame) 
{
  //if frame specified use specified frame  
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.TEAM,id).getYPos();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.TEAM,id).getYPos();
}

float VisionModule::getOurRobotRawX(int id, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return rawVisionData[frame].getRobot(sp->general.TEAM,id).getXPos();
  //else, use current frame
  else
    return rawVisionData[currentFrame].getRobot(sp->general.TEAM,id).getXPos();
}

float VisionModule::getOurRobotRawY(int id, int frame) 
{
  //if frame specified use specified frame  
  if(frame != -1)
    return rawVisionData[frame].getRobot(sp->general.TEAM,id).getYPos();
  //else, use current frame
  else
    return rawVisionData[currentFrame].getRobot(sp->general.TEAM,id).getYPos();
}
float VisionModule::getOurRobotXVel(int id, int frame)
{
  
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.TEAM,id).getXVel();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.TEAM,id).getXVel();
}

float VisionModule::getOurRobotYVel(int id, int frame)  
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.TEAM,id).getYVel();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.TEAM,id).getYVel();  
}


float VisionModule::getOurRobotRotation(int id, int frame)  
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.TEAM,id).getRotation();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.TEAM,id).getRotation();  
}

float VisionModule::getOurRobotRawRotation(int id, int frame)  
{
  //if frame specified use specified frame
  if(frame != -1)
    return rawVisionData[frame].getRobot(sp->general.TEAM,id).getRotation();
  //else, use current frame
  else
    return rawVisionData[currentFrame].getRobot(sp->general.TEAM,id).getRotation();  
}

float VisionModule::getOurRobotRotationVel(int id, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.TEAM,id).getRotVel();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.TEAM,id).getRotVel();  
}

//==================================================================
//==================================================================
//==================================================================

float VisionModule::getTheirRobotX(int id, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.OTHER_TEAM,id).getXPos();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,id).getXPos();    
}

float VisionModule::getTheirRobotY(int id, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.OTHER_TEAM,id).getYPos();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,id).getYPos();  
}

float VisionModule::getTheirRobotRawX(int id, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return rawVisionData[frame].getRobot(sp->general.OTHER_TEAM,id).getXPos();
  //else, use current frame
  else
    return rawVisionData[currentFrame].getRobot(sp->general.OTHER_TEAM,id).getXPos();    
}

float VisionModule::getTheirRobotRawY(int id, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return rawVisionData[frame].getRobot(sp->general.OTHER_TEAM,id).getYPos();
  //else, use current frame
  else
    return rawVisionData[currentFrame].getRobot(sp->general.OTHER_TEAM,id).getYPos();  
}
float VisionModule::getTheirRobotXVel(int id, int frame)  
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.OTHER_TEAM,id).getXVel();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,id).getXVel();    
}

float VisionModule::getTheirRobotYVel(int id, int frame)  
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getRobot(sp->general.OTHER_TEAM,id).getYVel();
  //else, use current frame
  else
    return visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,id).getYVel();
}


//==================================================================
//==================================================================
//==================================================================

bool VisionModule::isBallFound(int team,int num, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].isBallFound(team,num);
  //else, use current frame
  else
    return visionData[currentFrame].isBallFound(team,num);  
}

float VisionModule::getBallX(int team,int num, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getBall(team, num).getXPos();
  //else, use current frame
  else
    return visionData[currentFrame].getBall(team, num).getXPos(); 
}

float VisionModule::getBallY(int team,int num, int frame)
{
  //if frame specified use specified frame  
  if(frame != -1)
    return visionData[frame].getBall(team, num).getYPos();
  //else, use current frame
  else
    return visionData[currentFrame].getBall(team, num).getYPos();
}

float VisionModule::getBallRawX(int team,int num, int frame) 
{
  //if frame specified use specified frame
  if(frame != -1)
    return rawVisionData[frame].getBall(team, num).getXPos();
  //else, use current frame
  else
    return rawVisionData[currentFrame].getBall(team, num).getXPos(); 
}

float VisionModule::getBallRawY(int team,int num, int frame)
{
  //if frame specified use specified frame  
  if(frame != -1)
    return rawVisionData[frame].getBall(team, num).getYPos();
  //else, use current frame
  else
    return rawVisionData[currentFrame].getBall(team, num).getYPos();
}

float VisionModule::getBallXVel(int team,int num, int frame)  
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getBall(team, num).getXVel();
  //else, use current frame
  else
    return visionData[currentFrame].getBall(team, num).getXVel();
}

float VisionModule::getBallYVel(int team,int num, int frame)  
{
  //if frame specified use specified frame
  if(frame != -1)
    return visionData[frame].getBall(team, num).getYVel();
  //else, use current frame
  else
    return visionData[currentFrame].getBall(team, num).getYVel();
}

char VisionModule::getBallSource(int team, int num, int frame)
{
  //if frame specified use specified frame
  if(frame != -1)
    return rawVisionData[frame].getBallSource(team, num);
  //else, use current frame
  else
    return rawVisionData[currentFrame].getBallSource(team, num); 
  
}

//--------------------------------------------------------



