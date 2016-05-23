#ifndef STRATEGY_MODULE_H
#define STRATEGY_MODULE_H

class StrategyModule;
class StandardModule;

#include "ai.h"
#include "StandardModule.h"
#include "VisionModule.h"
#include "common/datatypes/SystemParameters.h"

#define MAX_MESSAGE_LENGTH 80

/*********************
* Strategy Data      *
*********************/

class StrategyData
{
	//friend class StrategyModule;
	
public:
  
  // constructors 
	StrategyData();
	
  Destination* getDestination(int ID) { return &destinations[ID]; }

  //This returns a const pointer to prevent people writing directly into the buffers.
  // If people do write directly to the buffers, they may overflow them.
	const char* robotMessageBuffer(RobotIndex ID) { return robotMessages[ID]; }
	void setMessage(RobotIndex ID, const char* message) 
  {
    if(ID >= ROBOT0 && ID < NUM_ROBOTS)
    {
      strncpy(robotMessages[ID], message, MAX_MESSAGE_LENGTH); 
    }
  }

  //set if a robot has possession
  void setFriendlyPossession(RobotIndex i, bool value);
  void setOpponentPossession(RobotIndex i, bool value);

  //get if a robot has possession
  bool getFriendlyPossession(RobotIndex i);
  bool getOpponentPossession(RobotIndex i);

protected:
	Destination destinations[NUM_ROBOTS];

	//this array stores a message for each of
	//our robots. we can save debugging
	//messages for any robot during the game
	//to quickly determine what each robot is doing
	//by looking at the console
	char robotMessages[NUM_ROBOTS][MAX_MESSAGE_LENGTH+1];  
  // (Add one to the message length for the terminating null character.
  //  strncpy will not add a null to the end of long strings.)

  //possesion holders
  bool friendlyPossession[NUM_ROBOTS];
  bool opponentPossession[NUM_ROBOTS];

};


/*********************
* Strategy Module    *
*********************/
class StrategyModule
{
public:
  Pair getLastUnpredictedBall();
	// constructors 
	StrategyModule( SystemParameters* SP,
                   VisionModule* vision)
  {
		this->SP		= SP;
		this->vision	= vision;

		currentFrame = VISION_FIRST_FRAME;

		strategyData = new StrategyData[MAX_LOG];

		// Inherited classes must initialize the array of StrategyData
    StrategyRunning = false;
//hack: remove
	hack_Formation=0;
//
}

  virtual ~StrategyModule() {
		if (strategyData != NULL) {
			delete strategyData;
			strategyData = NULL;
		}
	}

  //------------------------------------------------------
  //Read/Set Strategy Status
  //------------------------------------------------------  
  //Returns strategy status (on/off)
  bool strategyStatus() { return StrategyRunning; }

  //Toggles strategy status (on/off)
  bool toggleStrategyStatus()
  {
    StrategyRunning = !StrategyRunning;
    return StrategyRunning;
  }

  //Sets strategy status (on/off)
  bool setStrategyStatus(bool newStrategyStatus) 
  { 
    StrategyRunning = newStrategyStatus; 
    return StrategyRunning;
  }
  //------------------------------------------------------
  void setStandardModule(StandardModule* standardModule);
  //------------------------------------------------------

	// retrieves current frame
	virtual StrategyData* getCurrentFrame(int frame=-1) 
  { 
    //if frame # unspecified, take current frame
    if(frame==-1)  
      return &strategyData[currentFrame];
    //else, select specified frame
    else
      return &strategyData[frame];
  }

	// sets currentFrame (assume it is a valid frame)
	void setFrame(int newFrame) {currentFrame = newFrame;}

	// executes one frame of strategy
	virtual void executeStrategy();
    virtual void executeNoStrategy();

//---hack to change formation!!  Remove once the Scientific American people leave!!!!
  void setFormation(int num) { hack_Formation=num;}

  RobotTypeParams * getRobotTrajParams(RobotIndex robotID);

  VisionModule* getVisionModule() { return vision; }

protected:
  StandardModule* standardModule;

	SystemParameters*		SP;

	VisionModule*	vision;

	bool            StrategyRunning;

	int             currentFrame;
	StrategyData*	strategyData;

	//subclasses that do their own initialization use this constructor
	StrategyModule() { hack_Formation=0; }
	

	//hack: remove
	int hack_Formation;
//-------------------------

};



#endif // STRATEGY_MODULE_H
