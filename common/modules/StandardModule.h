#ifndef STANDARD_MODULE_H
#define STANDARD_MODULE_H

#include "ai.h"

#include "VisionModule.h"
#include "StrategyModule.h"
#include "ControlModule.h"
#include "hires_timer.h"
#include "ControlParams.h"

/*********************
* Standard Module    *
*********************/
class StandardModule
{
protected:
  //StandardModules may not be created directly.
  //You must use a subclass constructor to vill in the modules.
  StandardModule();
	
public:
	// destructor
  ~StandardModule();

  // initialization of StandardModule -- called from constructor
  void initializeModule();

  //get the current parameters
  SystemParameters* getParameters();

	//***** ENTER USER INTERFACE FUNCTIONS HERE *****//
  //=================================================
  //  Display Functions: these 
  //  simply get values to give to GUI
  //=================================================
  //Is vision running?
  bool VisionStatus(); 

  //Is wireless running?
  bool WirelessRunning();
   
  //Is the AI running?
  bool AIStatus();
  //--------------------------
  //Returns 1 or 2, which team we are playing on 
  //(which set of vision data we listen to)
  int CurrentTeam();

  //Is the field flipped?
  bool SidesFlipped();

  //return the systems current status
  char* GetSystemStatus(); 

  //--------------------------
  //Info on OUR robots
  //--------------------------

  //This function returns what the robot is currently doing. This
  //is a debugging feature, and allows code to display what it
  //is currently up to. 
  const char* GetRobotMessageBuffer(int ID, int frame=-1); 

  void SetRobotMessage(RobotIndex ID, const char* text);

  int StandardModule::GetDefaultColor();
  int StandardModule::GetComplexSetting();

  int GetFriendlyHasPossession(int ID);
  int GetOpponentHasPossession(int ID);

  //Get's the x-coordinate of our robot ID
  float GetOurRobotX(int ID, int frame=-1);

  //Get's the y-coordinate of our robot ID
  float GetOurRobotY(int ID, int frame=-1);

  //Get's the x-coordinate of our robot ID
  float GetOurRobotRawX(int ID, int frame=-1);

  //Get's the y-coordinate of our robot ID
  float GetOurRobotRawY(int ID, int frame=-1);

  //Get's the x-vel of our robot ID
  float GetOurRobotXVel(int ID, int frame=-1);

  //Get's the y-vel of our robot ID
  float GetOurRobotYVel(int ID, int frame=-1);

  //Get's the rotation of our robot ID
  float GetOurRobotRotation(int ID, int frame=-1);
  
  //Get's the rotation of our robot ID
  float GetOurRobotRawRotation(int ID, int frame=-1);

  //Get's the last sent x-velocity for our robot ID
  float GetOurRobotSentXVel(int ID, int frame=-1);

  //Get's the last sent y-velocity for our robot ID
  float GetOurRobotSentYVel(int ID, int frame=-1);

  //Get's the last sent rotation for our robot ID
  float GetOurRobotSentRotation(int ID, int frame=-1);

  //Get's our robots destination xposition
  float GetOurRobotDestX(int ID, int frame=-1);

  //Get's our robtos destination yposition
  float GetOurRobotDestY(int ID, int frame=-1);

  //Get's our robtos destination rotation
  float GetOurRobotDestRot(int ID, int frame=-1);
 
  float getNormalizedTranslationalError(int robot, int frame=-1);
  float getNormalizedRotationalError(int robot, int frame=-1);
  bool getResetTranslationalVelocity(int robot, int frame=-1);
  bool getResetRotationalVelocity(int robot, int frame=-1);
  float getDistanceTravelled(int robot, int frame=-1);
  float getRotationTravelled(int robot, int frame=-1);  

  //--------------------------
  //Info on THEIR robots
  //--------------------------
  //Get's the x-coordinate of their robot ID
  float GetTheirRobotX(int ID, int frame=-1);

  //Get's the y-coordinate of their robot ID
  float GetTheirRobotY(int ID, int frame=-1);

  //Get's the x-coordinate of their robot ID
  float GetTheirRobotRawX(int ID, int frame=-1);

  //Get's the y-coordinate of their robot ID
  float GetTheirRobotRawY(int ID, int frame=-1);

  //Get's the x-vel of their robot ID
  float GetTheirRobotXVel(int ID, int frame=-1);

  //Get's the y-vel of their robot ID
  float GetTheirRobotYVel(int ID, int frame=-1);

  //--------------------------
  //Info on BALL
  //--------------------------

  //Get's the x-coordinate of ball
  float GetBallX(int frame=-1);

  //Get's the y-coordinate of ball
  float GetBallY(int frame=-1);

  //Get's the x-coordinate of ball
  float GetBallRawX(int frame=-1);

  //Get's the y-coordinate of ball
  float GetBallRawY(int frame=-1);

  //Get's the x-vel of ball
  float GetBallXVel(int frame=-1);

  //Get's the y-vel of ball
  float GetBallYVel(int frame=-1);

  //gets the xcoord of ball in prediction error data for frame
  float GetBallPredErrX(int frame=-1);

  //gets the ycoord of ball in prediction error data for frame
  float GetBallPredErrY(int frame=-1);
//-----------------------------

	//Get's the x-coordinate of ball
	float GetOurBallX(int num, int frame=-1);

	//Get's the y-coordinate of ball
	float GetOurBallY(int num, int frame=-1) ;

	//Get's the x-vel of ball
	float GetOurBallXVel(int num, int frame=-1);

	//Get's the y-vel of ball
	float GetOurBallYVel(int num, int frame=-1) ;

	//OPPONENT BALLS
	//Get's the x-coordinate of ball
	float GetOpponentBallX(int num, int frame=-1);

	//Get's the y-coordinate of ball
	float GetOpponentBallY(int num, int frame=-1) ;

	//Get's the x-vel of ball
	float GetOpponentBallXVel(int num, int frame=-1);

	//Get's the y-vel of ball
	float GetOpponentBallYVel(int num, int frame=-1) ;

  //--------------------------
  //Returns current frame
  //--------------------------
  int ReturnFrame();

  //Is our robot ID found?
  bool OurRobotFound(int robotID, int frame=-1);
  //Is their robot ID found?
  bool TheirRobotFound(int robotID, int frame=-1);
  //Is the ball found?
  bool RoboCupBallFound(int frame=-1);
  //Source of ball information (which camera)
  char RoboCupBallSource(int frame=-1) ;


  //is robot ID kick armed?
  bool RobotKick(int robotID, int frame=-1);
  //is robot ID dribbling?
  bool RobotDribble(int robotID, int frame=-1);

  //does friendly robot ID have possession?
  bool FriendlyHasPossession(int robotID, int frame=-1);
  bool OpponentHasPossession(int robotID, int frame=-1);

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
  bool ToggleVision();

  //Toggles wireless status, returns new value
  bool ToggleWireless();

  //Toggles AI status, returns new value
  bool ToggleAI();
  
  //---------------------------
  // Set Vision/Wireless/AI
  //---------------------------
  //Set vision status, returns new value
  bool SetVision(bool newVisionStatus);

  //Set wireless status, returns new value
  bool SetWireless(bool newWirelessStatus);

  //Set AI status, returns new value
  bool SetAI(bool newAIStatus);
  //---------------------------
  // Toggle System Variables
  //---------------------------
  //Toggles current team # between 1 and 2, returns toggled value
  int ToggleTeamNumber();

  //Toggles playing side, returns new value (1 = regular, 2 = flipped)
  bool TogglePlayingSide();
  
  //Sets the current frame back to zero
  virtual void resetFrame();

  //--------------------------------
  //Latency info getters and setters
  //--------------------------------
  bool getLatencyTestDone()
  { 
    //if not done, return false
    if(!latencyTestDone)
      return false;

    //if done, set to false, then return true
    //this way window is only opened once
    latencyTestDone = false;
    return true;
  }
  
  void setLatencyTestDone(bool newValue)
  { 
    latencyTestDone = newValue;
  }

  float getMinLatency()
  { 
    return minLatency; 
  }
  
  void setMinLatency(float newMinLatency)
  { 
    minLatency = newMinLatency; 
  }

  float getMaxLatency()
  { 
    return maxLatency; 
  }
  
  void setMaxLatency(float newMaxLatency)
  { 
    maxLatency = newMaxLatency; 
  }

  float getMeanLatency()
  { 
    return meanLatency; 
  }
  
  void setMeanLatency(float newMeanLatency)
  { 
    meanLatency = newMeanLatency; 
  }
  
  //=================================================
	//***** END USER INTERFACE FUNCTIONS *****//

	// runs one frame of the RobocupSystem
	virtual void executeFrame(Client *client);


  virtual void reloadParameters();
  virtual void resetParametersInModules();

  int getTeam() { return SP->general.TEAM; }

  int getTeamNumber() {return SP->general.TEAM;};

  RobotTypeParams * getRobotTrajParams(RobotIndex robotID);


  virtual void shutDown();

protected:

	//gives a small idea what the system
	//has done (like init rpc and transmitting)
	char* systemStatus;
	
	VisionModule*   vision;
	StrategyModule* strategy;
	ControlModule*  control;

    //params object
    SystemParameters* SP;

	int currentFrame;

	// increments the current frame by one
	void updateFrame();

  //latency information
  float minLatency;
  float maxLatency;
  float meanLatency;
  bool latencyTestDone;

  Timer timer;
  Timer totalTimer;
public:

  //how much time vision claims last frame took to execute
  double visionExecuteTime();

  //how much time it took us to get the last frame of vision data
  double visionGetTime();

  //how much time it took us to execute wireless code
  double wirelessExecuteTime();

  //how much time it took us to execute talk to simulator code
  double simulatorExecuteTime();

  //how much time it took us to execute trajectory code
  double trajectoryExecuteTime();

  //how much time it took us to execute strategy code
  double strategyExecuteTime();

  //how much time entire loop is
  double totalExecuteTime();

  //how fast the AI is running
  double aiFPS();

  //how fast the vision systems claims to be running
  double visionFPS();
  
  

  double elapsedTime_start;
  double elapsedTime_network;
  double elapsedTime_wireless;
  double elapsedTime_prediction;
  double elapsedTime_strategy;
  double elapsedTime_trajectory;
private:
  //used to calculate execute times
  double start, stop;
  double overallStartTime, 
         visionGetTimeVal,
         visionExecuteTimeVal,
         wirelessExecuteTimeVal,
         simulatorExecuteTimeVal,
         strategyExecuteTimeVal,
         trajectoryExecuteTimeVal,
         totalExecuteTimeVal;       

//  const static int FRAMES_TO_AVERAGE = 10;
  #define FRAMES_TO_AVERAGE 10
};


#endif // STANDARD_MODULE_H