#include <iostream.h>
#include "StandardModule.h"
#include "../network/Client.h"
#include "info/ball/possession.h"

const float NOT_FOUND_X = -32000.0f;
const float NOT_FOUND_Y = -32000.0f;
const float NOT_FOUND_XVEL = -32000.0f;
const float NOT_FOUND_YVEL = -32000.0f;
const float NOT_FOUND_ROT = -32000.0f;
const float NOT_FOUND_ROTVEL = -32000.0f;

/*********************
* Standard Module    *
*********************/
	

// constructor
StandardModule::StandardModule() 
{

  currentFrame = VISION_FIRST_FRAME;
	
  //Create a new parameters object. All values are 
  //loaded by the constructor for you.
  SP = new SystemParameters();

  vision = NULL;
  strategy = NULL;
  control = NULL;

  systemStatus = "Standard Module Created!";
  //==================================
  //latency information
  minLatency = 0;
  maxLatency = 0;
  meanLatency = 0;
  latencyTestDone = false;
  //==================================

}

// destructor
StandardModule::~StandardModule() {
    StandardModule::shutDown();
	delete control;
	delete strategy;
	delete vision;
	delete SP;
}
//======================================================================
//how much time vision claims last frame took to execute
double StandardModule::visionExecuteTime()
{
  if(currentFrame != VISION_FIRST_FRAME)
    return 1000.0*(vision->getCurrentFrame()->getFinishTime() - 
                   vision->getCurrentFrame(currentFrame-1)->getFinishTime());
  else
    return 1000.0*(vision->getCurrentFrame()->getFinishTime() - 
                   vision->getCurrentFrame(MAX_LOG-1)->getFinishTime());
}
//======================================================================
//how much time it took us to get the last frame of vision data
double StandardModule::visionGetTime()
{
  return visionGetTimeVal;
}
//======================================================================
//how much time it took us to execute wireless code
double StandardModule::wirelessExecuteTime()
{
  return wirelessExecuteTimeVal;
}
//======================================================================
//how much time it took us to execute talk to simulator code
double StandardModule::simulatorExecuteTime()
{
  return simulatorExecuteTimeVal;
}
//======================================================================
//how much time it took us to execute trajectory code
double StandardModule::trajectoryExecuteTime()
{
  return trajectoryExecuteTimeVal;
}
//======================================================================
//how much time it took us to execute strategy code
double StandardModule::strategyExecuteTime()
{
  return strategyExecuteTimeVal;
}
//======================================================================
//how much time entire loop is
double StandardModule::totalExecuteTime()
{
  return totalExecuteTimeVal;
}
//======================================================================
//how fast the AI is running
double StandardModule::aiFPS()
{
  return 1000.0 / totalExecuteTimeVal;
}
//======================================================================
//how fast the vision systems claims to be running
double StandardModule::visionFPS()
{
  return 1000.0 / visionExecuteTimeVal;
}
//======================================================================

//get the current parameters
SystemParameters* StandardModule::getParameters()
{
  return SP;
}


// runs one frame of the RobocupSystem
void StandardModule::executeFrame(Client *client)
{
  //-----------------------------
  //-----------------------------
  // COMPLETE PREVIOUS FRAME
  // wait for new vision data
  //if vision enabled, give it 2500 ms
  totalTimer.markStartTime();
  timer.markStartTime();
  if( VisionStatus() )
  {
    vision->getNewData(client);
    
    //check to see if the vision coords have to be flipped...
    if (vision->areSidesFlipped() )
      vision->FlipVision();
      
  }
  //else, only wait VERY short period of time (30ms)
  else
  {
    vision->getNoVisionData();
    Sleep(30);
  }

  timer.markEndTime();
  visionGetTimeVal = timer.getInterval_ms();

  //-----------------------------
  //-----------------------------
  //Calculate how much time last frame of vision took to execute on vision process
  if(currentFrame != VISION_FIRST_FRAME)
    visionExecuteTimeVal = 1000.0*(vision->getCurrentFrame()->getFinishTime() - 
                                   vision->getCurrentFrame(currentFrame-1)->getFinishTime());
  else
    visionExecuteTimeVal = 1000.0*(vision->getCurrentFrame()->getFinishTime() - 
                                   vision->getCurrentFrame(MAX_LOG-1)->getFinishTime());

  //-----------------------------
  //-----------------------------
  // Send previous commands - wireless
  timer.markStartTime();
  control->sendRPCCommands(client);
  timer.markEndTime();
  wirelessExecuteTimeVal = timer.getInterval_ms();

  //-----------------------------
  //-----------------------------
  // Send previous commands - simulator
  timer.markStartTime();
  control->sendSimulatorCommands(client);
  timer.markEndTime();
  simulatorExecuteTimeVal = timer.getInterval_ms();

  //-----------------------------
  //-----------------------------
  // EXECUTE NEW FRAME
  updateFrame();
      
  vision->predictVision( control->getControlArray() );   

  // update raw vision data in the vision module
  vision->updateRawVision();


  //-----------------------------
  //-----------------------------
  //Execute Strategy + Trajectory
  //if the ai is running, get new desinations for robots
  if(strategy->strategyStatus())
  {
    //---------------------
    //---------------------
    //Execute strategy
    timer.markStartTime();

    //Execute AI commands
    strategy->executeStrategy();
    timer.markEndTime();
    strategyExecuteTimeVal = timer.getInterval_ms();
    //---------------------
    //---------------------
    //Execute trajectory
    timer.markStartTime();
    control->generatePaths();
    timer.markEndTime();
    trajectoryExecuteTimeVal = timer.getInterval_ms();
    //---------------------
    //---------------------
  }
  else
  {
    //---------------------
    //---------------------
    //Execute strategy
    timer.markStartTime();

    //Execute AI commands
    strategy->executeNoStrategy();
    timer.markEndTime();
    strategyExecuteTimeVal = timer.getInterval_ms();
    //---------------------
    //---------------------
    //Execute trajectory
    timer.markStartTime();
    control->stopRobots();
    timer.markEndTime();
    trajectoryExecuteTimeVal = timer.getInterval_ms();
    //---------------------
    //---------------------
  }
  //-----------------------------
  //-----------------------------
  totalTimer.markEndTime();
  totalExecuteTimeVal = totalTimer.getInterval_ms();
  //-----------------------------
  //-----------------------------
}

// increments the current frame by one
void StandardModule::updateFrame()
{
	currentFrame++;
	
	if (currentFrame >= MAX_LOG)
  {
		currentFrame = VISION_FIRST_FRAME;
	}

	vision->setFrame(currentFrame);
	strategy->setFrame(currentFrame);
	control->setFrame(currentFrame);
}


//***** ENTER USER INTERFACE FUNCTIONS HERE *****//
//=================================================
//  Display Functions: these 
//  simply get values to give to GUI
//=================================================
//Is vision running?
bool StandardModule::VisionStatus() { return vision->visionStatus(); }

//Is wireless running?
bool StandardModule::WirelessRunning() { return control->wirelessStatus(); }

//Is the AI running?
bool StandardModule::AIStatus() { return strategy->strategyStatus(); }

//--------------------------
//Returns 1 or 2, which team we are playing on 
//(which set of vision data we listen to)
int StandardModule::CurrentTeam() { return SP->general.TEAM; }

  //Is the field flipped?
bool StandardModule::SidesFlipped() { return vision->areSidesFlipped(); }


//return the systems current status
char* StandardModule::GetSystemStatus() 
{
  return systemStatus;
}

//--------------------------
//Info on OUR robots
//--------------------------
//This function returns what the robot is currently doing. This
//is a debugging feature, and allows code to display what it
//is currently up to. 
const char* StandardModule::GetRobotMessageBuffer(int ID, int frame)
{
  return strategy->getCurrentFrame(frame)->robotMessageBuffer((RobotIndex)ID);
}

void StandardModule::SetRobotMessage(RobotIndex ID, const char* text){
	strategy->getCurrentFrame(currentFrame)->setMessage(ID,text);
}

int StandardModule::GetDefaultColor()
{
	return SP->general.COLOR_SCHEME;
}

int StandardModule::GetComplexSetting()
{
	return SP->general.COMPLEX_MODE;
}

int StandardModule::GetFriendlyHasPossession(int ID)
{
	return friendlyHasPossession((RobotIndex)ID, (*SP));
}

int StandardModule::GetOpponentHasPossession(int ID)
{
	return opponentHasPossession((RobotIndex)ID, (*SP));
}

//Get's the x-coordinate of our robot ID
float StandardModule::GetOurRobotX(int ID, int frame) 
{
  return vision->getOurRobotX(ID, frame); 
}

//Get's the y-coordinate of our robot ID
float StandardModule::GetOurRobotY(int ID, int frame) 
{ 
  return vision->getOurRobotY(ID, frame);
}

//Get's the x-coordinate of our robot ID
float StandardModule::GetOurRobotRawX(int ID, int frame) 
{
  return vision->getOurRobotRawX(ID, frame); 
}

//Get's the y-coordinate of our robot ID
float StandardModule::GetOurRobotRawY(int ID, int frame) 
{ 
  return vision->getOurRobotRawY(ID, frame);
}

//Get's the x-vel of our robot ID
float StandardModule::GetOurRobotXVel(int ID, int frame)
{ 
  return vision->getOurRobotXVel(ID, frame);
}

//Get's the y-vel of our robot ID
float StandardModule::GetOurRobotYVel(int ID, int frame)
{ 
  return vision->getOurRobotY(ID, frame); 
}

//Get's the rotation of our robot ID
float StandardModule::GetOurRobotRotation(int ID, int frame) 
{
  return vision->getOurRobotRotation(ID, frame);
}

//Get's the rotation of our robot ID
float StandardModule::GetOurRobotRawRotation(int ID, int frame) 
{
  return vision->getOurRobotRawRotation(ID, frame);
}

//Get's the last sent x-velocity for our robot ID 
float StandardModule::GetOurRobotSentXVel(int ID, int frame) 
{ 
  return control->getCurrentFrame(frame)->getXVel(ID); 
}
//-----------------------------
//Get's the last sent y-velocity for our robot ID
float StandardModule::GetOurRobotSentYVel(int ID, int frame) 
{ 
  return control->getCurrentFrame(frame)->getYVel(ID); 
}
//-----------------------------
//Get's the last sent rotation for our robot ID
float StandardModule::GetOurRobotSentRotation(int ID, int frame)
{ 
  return control->getCurrentFrame(frame)->getRotVel(ID);
}
//-----------------------------
float StandardModule::GetOurRobotDestX(int ID, int frame)
{
  return strategy->getCurrentFrame(frame)->getDestination(ID)->getXPos();
}
//-----------------------------
float StandardModule::GetOurRobotDestY(int ID, int frame)
{
  return strategy->getCurrentFrame(frame)->getDestination(ID)->getYPos();
}
//-----------------------------
float StandardModule::GetOurRobotDestRot(int ID, int frame)
{
  return strategy->getCurrentFrame(frame)->getDestination(ID)->getRotation();
}
//-----------------------------
float StandardModule::getNormalizedTranslationalError(int robot, int frame)
{
  return vision->getRawFrame(frame)->getNormalizedTranslationalError(robot);
}
//-----------------------------
float StandardModule::getNormalizedRotationalError(int robot, int frame)
{
  return vision->getRawFrame(frame)->getNormalizedRotationalError(robot);
}
//-----------------------------
bool StandardModule::getResetTranslationalVelocity(int robot, int frame)
{
  return vision->getRawFrame(frame)->getResetTranslationalVelocity(robot);
}
//-----------------------------
bool StandardModule::getResetRotationalVelocity(int robot, int frame)
{
  return vision->getRawFrame(frame)->getResetRotationalVelocity(robot);
}
//-----------------------------
float StandardModule::getDistanceTravelled(int robot, int frame)
{
  return vision->getRawFrame(frame)->getDistanceTravelled(robot);
}
//-----------------------------
float StandardModule::getRotationTravelled(int robot, int frame) 
{
  return vision->getRawFrame(frame)->getRotationTravelled(robot);
}
//-----------------------------

//--------------------------
//Info on THEIR robots
//--------------------------
//Get's the x-coordinate of their robot ID
float StandardModule::GetTheirRobotX(int ID, int frame) 
{ 
  return vision->getTheirRobotX(ID, frame); 
}

//Get's the y-coordinate of their robot ID
float StandardModule::GetTheirRobotY(int ID, int frame) 
{ 
  return vision->getTheirRobotY(ID, frame); 
}

//Get's the x-coordinate of their robot ID
float StandardModule::GetTheirRobotRawX(int ID, int frame) 
{ 
  return vision->getTheirRobotRawX(ID, frame); 
}

//Get's the y-coordinate of their robot ID
float StandardModule::GetTheirRobotRawY(int ID, int frame) 
{ 
  return vision->getTheirRobotRawY(ID, frame); 
}

//Get's the x-vel of their robot ID
float StandardModule::GetTheirRobotXVel(int ID, int frame) 
{ 
  return vision->getTheirRobotXVel(ID, frame); 
}

//Get's the y-vel of their robot ID
float StandardModule::GetTheirRobotYVel(int ID, int frame) 
{ 
  return vision->getTheirRobotYVel(ID, frame);
}

//--------------------------
//Info on BALL
//--------------------------

//Get's the x-coordinate of ball
float StandardModule::GetBallX(int frame) 
{ 
  return vision->getBallX(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX, frame);
}

//Get's the y-coordinate of ball
float StandardModule::GetBallY(int frame) 
{ 
  return vision->getBallY(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX, frame);
}

//Get's the raw x-coordinate of ball
float StandardModule::GetBallRawX(int frame) 
{ 
  return vision->getBallRawX(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX, frame);
}

//Get's the raw y-coordinate of ball
float StandardModule::GetBallRawY(int frame) 
{ 
  return vision->getBallRawY(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX, frame);
}

//Get's the x-vel of ball
float StandardModule::GetBallXVel(int frame) 
{ 
  return vision->getBallXVel(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX, frame);
}

//Get's the y-vel of ball
float StandardModule::GetBallYVel(int frame) 
{
  return vision->getBallYVel(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX, frame);
}

//--------------------------
//Returns current frame
//--------------------------
int StandardModule::ReturnFrame() { return currentFrame; }

//Is our robot ID found?
bool StandardModule::OurRobotFound(int robotID, int frame) 
{ 
  return vision->isRobotFound(SP->general.TEAM, robotID, frame); 
}
//Is their robot ID found?
bool StandardModule::TheirRobotFound(int robotID, int frame) 
{ 
  return vision->isRobotFound(SP->general.OTHER_TEAM, robotID, frame); 
}

//Is the ball found?
bool StandardModule::RoboCupBallFound(int frame) 
{ 
  return vision->isBallFound(0,0, frame); 
}

//Ball Source?
char StandardModule::RoboCupBallSource(int frame) 
{ 
  return vision->getBallSource(0, 0, frame);
}


//is robot ID kick armed?
bool StandardModule::RobotKick(int robotID, int frame) 
{ 
  return (control->getCurrentFrame(frame)->getKick((RobotIndex) robotID) != NO_KICK);
}

//is robot ID dribbling?
bool StandardModule::RobotDribble(int robotID, int frame)
{
  return (control->getCurrentFrame(frame)->getDribble((RobotIndex) robotID) != NO_DRIBBLE);
}

//does friendly robot ID have possession?
bool StandardModule::FriendlyHasPossession(int robotID, int frame)
{
  return (strategy->getCurrentFrame(frame)->getFriendlyPossession((RobotIndex)robotID));
}

//does friendly robot ID have possession?
bool StandardModule::OpponentHasPossession(int robotID, int frame)
{
  return (strategy->getCurrentFrame(frame)->getOpponentPossession((RobotIndex)robotID));
}


//=================================================
//  Set Functions: these 
//  take values from the GUI and set them in the
//  AI like the current play for wireless 
//  running boolean
//=================================================

//---------------------------
// Toggle Vision/Wireless/AI
//---------------------------
//Toggles vision status, returns new value
bool StandardModule::ToggleVision() { return vision->toggleVisionStatus(); }

//Toggles wireless status, returns new value
bool StandardModule::ToggleWireless() { return control->toggleWirelessStatus(); }

//Toggles AI status, returns new value
bool StandardModule::ToggleAI() { return strategy->toggleStrategyStatus(); }
//---------------------------
// Set Vision/Wireless/AI
//---------------------------
//Set vision status, returns new value
bool StandardModule::SetVision(bool newVisionStatus) 
{  
  return vision->setVisionStatus(newVisionStatus);
}

//Set wireless status, returns new value
bool StandardModule::SetWireless(bool newWirelessStatus)
{
  return control->setWirelessStatus(newWirelessStatus);
}

//Set AI status, returns new value
bool StandardModule::SetAI(bool newAIStatus)
{
  return strategy->setStrategyStatus(newAIStatus);
}
//---------------------------
// Toggle System Variables
//---------------------------
//Toggles current team # between 1 and 2, returns toggled value
int StandardModule::ToggleTeamNumber()
{
  int SAVE = SP->general.TEAM;
  SP->general.TEAM = SP->general.OTHER_TEAM;
  SP->general.OTHER_TEAM = SAVE;
  return SP->general.TEAM;
}

//Toggles playing side, returns new value
bool StandardModule::TogglePlayingSide()
{
  return vision->flipSides();
}

void StandardModule::reloadParameters()
{
  //always turn off wireless in case you have to change teams etc...
  control->setWirelessStatus(false);
  
  SP->general.loadValues();
  resetParametersInModules();
}

void StandardModule::resetParametersInModules()
{ 
  vision->reloadParameters();
  control->updateParameters();
}

void StandardModule::resetFrame(){
  currentFrame = VISION_FIRST_FRAME;
}

  //=================================================

	//***** END USER INTERFACE FUNCTIONS *****//

void StandardModule::shutDown()
{
}

RobotTypeParams * StandardModule::getRobotTrajParams(RobotIndex robotID){
  switch (control->wireless->getRobotType(robotID)){
  case ROBOT_2001:
    return &SP->control.Omni2001;
    break;
  case ROBOT_2002:
  default:
    return &SP->control.Omni2002;
    break;
  }
}




