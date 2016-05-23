#include "RoboCupStandardModule.h"
#include "RoboCupStrategyModule.h"

#include "../common/network/Client.h"
#include "../Skillz/RegisterSkillz.h"
#include "../plays/RegisterPlays.h"
#include "Skillz/SkillSet.h"
#include "plays/noplay.h"
#include "common/wireless/WirelessSender.h"

RoboCupModule::RoboCupModule(const char* commandLineArg) : StandardModule()
{
  cout.flush();

	// Initialize Main Modules and their connections
	vision      = new VisionModule(SP);
  //Register all the skills BEFORE we create a strategy module.
  registerSkillz();
  registerPlays();
	RCstrategy  = new RoboCupStrategyModule(SP, vision);
  strategy    = RCstrategy;  //RCstrategy is a local pointer so we do fewer casts
  strategy->setStandardModule(this);
	control     = new ControlModule(SP, vision, strategy);



  const char* hostIP;
  if (commandLineArg == NULL) 
  {
    cout << "\nNo vision server specified.  Using the default: " << SP->general.SERVER_ADDR << "\n" << endl;
    hostIP = SP->general.SERVER_ADDR;
  }
  else
  {
    hostIP = commandLineArg;
  }



  cout << "attempting to connect to: " << hostIP << " on port: " << SP->general.VISION_PORT << "\n";
  client_ = new Client(SEND_RECEIVE,hostIP, SP->general.VISION_PORT);
  client_->openConnection();

  reloadParameters();

}
	

RoboCupModule::~RoboCupModule()
{
  RoboCupModule::shutDown();
}

void RoboCupModule::executeFrame()
{
  if (client_ == NULL)
  {
    cout << "Error!  Not connected to the vision!" << endl;
    return;
  }
	///do all robot actions for this frame
  StandardModule::executeFrame(client_);
	///now let robot all robot skillz update and evaluate their performance
	evaluateSkillz();
	updateSkillz();
  resetSkillz();
}

char* RoboCupModule::getRobotType(int i)
{
  switch(control->wireless->getRobotType(i))
  { 
    case ROBOT_2000:
      return "2000";
    case ROBOT_2001:
      return "2001";
    case ROBOT_2002:
      return "2002";
    default:
      return "????";
  }
}

void RoboCupModule::setRobotType(int i, int type)
{
  switch(type)
  {
  case 2000:
    control->wireless->setRobotType(i, ROBOT_2000);
    break;
  case 2001:
    control->wireless->setRobotType(i, ROBOT_2001);
    break;
  case 2002:
    control->wireless->setRobotType(i, ROBOT_2002);
    break;
  }
}

char* RoboCupModule::getPositionText(Position p) const {
    switch (p){
      case NO_POSITION: return "No position";
      //-------------------------------------
      //2002 Positions
      case BLOCKER: return "Blocker";
      case DEFENDER: return "Defender";
      case AGGRESSOR: return "Aggressor";
      case CREATOR: return "Creator";
      case SPECIAL_OP_DEFENDER: return "Special Op Defender";
      case SPECIAL_OP_AGGRESSOR: return "Special Op Aggressor";
      case SPECIAL_OP_CREATOR: return "Special Op Creator";
      //-------------------------------------
      default: return "Error: bad value for position";
    }
}

char* RoboCupModule::getPositionName(Position p) const {
  switch (p){
      case NO_POSITION: return "Lice";
      default: return "Error: bad value for position";
    }
}

//----------------------------------
//samples vision speed
void RoboCupModule::sampleFrameRate()
{
  vision->SampleVisionSpeed(client_);
}

//turn position switching on/off
bool RoboCupModule::togglePositionSwitching()
{
  RCstrategy->checkPositions = !RCstrategy->checkPositions;
  return RCstrategy->checkPositions;
}

//turn play transitions on/off
bool RoboCupModule::togglePlayTransitions()
{
  RCstrategy->checkTransitions = !RCstrategy->checkTransitions;
  return RCstrategy->checkTransitions;
}

//reset frame
void RoboCupModule::resetFrame(){
  RCstrategy->copyIntoLastFrame();
  currentFrame = VISION_FIRST_FRAME;
}

//called by the destructor
void RoboCupModule::shutDown(void) 

{
  if(client_ != NULL)
  {
    client_->close();
  }

  StandardModule::shutDown();  
}

void RoboCupModule::resetPlay()
{
  RCstrategy->playbook->getPlay(RCstrategy->getCurrentRoboCupFrame()->getCurrentPlay())->initializePlay
    (
      *vision->getCurrentFrame(),   
      RCstrategy->getCurrentRoboCupFrame()
    );
}


  void RoboCupModule::setPlay(int play)
  {
    RCstrategy->getCurrentRoboCupFrame()->setNewPlay(static_cast<Play>(play));
    for(RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++)
    {
      RCstrategy->getSkillSet(i)->unitializeSkillz();
    } 
  }

  Play RoboCupModule::getPlay(int frame) 
  {
    //if specifying a frame
    if(frame !=-1)
      return RCstrategy->getCurrentRoboCupFrame(frame)->getCurrentPlay();
    //else, return current frame
    else
    {
      //if the play has not been changed, just return it
      if(!RCstrategy->getCurrentRoboCupFrame()->playChanged())
        return RCstrategy->getCurrentRoboCupFrame()->getCurrentPlay();
      //else, return the play that has been set, since the AI must be off right now
      else
        return RCstrategy->getCurrentRoboCupFrame()->getNextPlay();

    }
  }

  bool RoboCupModule::playChanged()
  {
    return RCstrategy->getCurrentRoboCupFrame()->playChanged();
  }

  char* RoboCupModule::getPlayName()
  { 
    return this->getPlayName(getPlay()); 
  }

  int RoboCupModule::currentPlayActive()
  {
     if(
        ((getPlay() > START_2002_PLAYS) && 
         (getPlay() < END_2002_PLAYS))
       )
     {
        if(playChanged())
          return 2;
        else
          return 1;
     }
     else
     {
        if(playChanged())
          return 4;
        else
          return 3;

     }
  }

  char* RoboCupModule::getPlayName(Play play)
  { 
    return RCstrategy->playbook->getPlayName(play); 
  }

  bool RoboCupModule::ToggleVision()
  { 
    bool visStat = vision->toggleVisionStatus();
    if (visStat) {
      client_->openConnection();
    } else {
      client_->close();
    }
    return visStat;
  }

  //this funciton fills buffer with the names of all of the plays, starting after NO_PLAY
  //buffer should be a large char array (at least 20*NUM_PLAYS, maybe more).
  void RoboCupModule::getAllPlayNames(char* buffer)
  { 
    RCstrategy->playbook->enumPlayNames(buffer); 
  }

  //-----------------------------------
  // Set Robot Position or Current Play
  //-----------------------------------
  //Get's robot ID's position (e.g. midfielder==1)
  Position RoboCupModule::GetRobotPosition(RobotIndex ID, int frame)
  { 
    //if frame specified, used it
    if(frame != -1)
      return RCstrategy->getCurrentRoboCupFrame(frame)->getPositionOfRobot(ID); 
    //else use current frame
    else
      return RCstrategy->getCurrentRoboCupFrame()->getPositionOfRobot(ID); 
  
  }

  //Sets robot ID's positition
  void RoboCupModule::SetRobotPosition(int robotID, int newPosition)
  { 
    RCstrategy->getCurrentRoboCupFrame()->setRobotAtPosition( (RobotIndex)robotID, (Position)newPosition ); 
    RCstrategy->getSkillSet((RobotIndex) robotID)->resetSkillz();
    resetPlay();
  }

  void RoboCupModule::Initialize2002Positions()
  { 
    RCstrategy->Initialize2002Positions(); 
  }
  
  void RoboCupModule::ResetPositions() 
  { 
    RCstrategy->ResetPositions(); 
  }
  
  float RoboCupModule::GetRobotDestinationX(int ID, int frame)
  { 
    return strategy->getCurrentFrame(frame)->getDestination(ID)->getPos().getX(); 
  }

  float RoboCupModule::GetRobotDestinationY(int ID, int frame)
  { 
    return strategy->getCurrentFrame(frame)->getDestination(ID)->getPos().getY(); 
  }

  float RoboCupModule::GetRobotDestinationRotation(int ID, int frame)
  { 
    return strategy->getCurrentFrame(frame)->getDestination(ID)->getRotation(); 
  }
  
  float RoboCupModule::GetRobotPassX(int ID, int frame)
  { 
    return RCstrategy->getCurrentRoboCupFrame(frame)->getPassDest((RobotIndex)ID).getX(); 
  }

  float RoboCupModule::GetRobotPassY(int ID, int frame)
  { 
    return RCstrategy->getCurrentRoboCupFrame(frame)->getPassDest((RobotIndex)ID).getY(); 
  }

  int RoboCupModule::GetRobotPassValue(int ID, int frame)
  { 
    return RCstrategy->getCurrentRoboCupFrame(frame)->getPassValue((RobotIndex)ID); 
  }

///Evaluates all skills for all the robots
void RoboCupModule::evaluateSkillz() 
{
  for(RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++) 
  {
    RCstrategy->getSkillSet(i)->updateVisionAndStrategyPointers();
    RCstrategy->getSkillSet(i)->evaluateSkillz();
  }
}


///Updates all skills for all the robots
void RoboCupModule::updateSkillz() {
  for(RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++) {
    RCstrategy->getSkillSet(i)->updateSkillz();
  }
}

///Reloads all parameters for all skills in all skillsets
void RoboCupModule::loadSkillParams() 
{
  for(RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++) 
  {
    RCstrategy->getSkillSet(i)->loadSkillParams();
  }
}

///Reloads all parameters for all plays
void RoboCupModule::loadPlayParams() 
{
  for(int i = 0; i<NUM_PLAYS; i++)
  {
     if(i != INVALID_PLAY)
     {
       BasePlay* play = RCstrategy->playbook->getPlay(i);
       if(play != NULL)
       {
         play->loadValues();
       }
     }
  }
}

void RoboCupModule::loadWeatherParams(){
  RCstrategy->weather->loadWeatherParams();
}
///Resets the hasBeenRun variable for all skillz, so they can run again next frame
void RoboCupModule::resetSkillz() {
  for(RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++) {
    RCstrategy->getSkillSet(i)->resetSkillz();
  }
}
void RoboCupModule::loadTransitionParams(){
    RCstrategy->pt->loadTransitionParams();
}


///Reloads all parameters, both normal params and skill params
void RoboCupModule::reloadParameters()
{
  //always turn off wireless in case you have to change teams etc...
  control->setWirelessStatus(false);
    
  //load regular params
  SP->loadValues();

  //load skill params
  loadSkillParams();

  //load play params
  loadPlayParams();

  loadWeatherParams();

  resetParametersInModules();

  //reinit the play
  resetPlay();
}

///Reloads trajectory parameters
void RoboCupModule::reloadTrajectoryParameters()
{
  //always turn off wireless in case you have to change teams etc...
  control->setWirelessStatus(false);
    
  //load regular params
  SP->loadValues();

  resetParametersInModules();
}