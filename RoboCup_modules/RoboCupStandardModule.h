#ifndef RC_STANDARD_MODULE_H
#define RC_STANDARD_MODULE_H

#include "StandardModule.h"
#include "plays/playbook.h"
#include "RoboCupStrategyModule.h"

#include "../common/network/Client.h"

/*********************
* Robocup Module     *
*********************/
class RoboCupModule :public StandardModule
{
public:
  RoboCupModule(const char* commandLineArg);
  ~RoboCupModule();

  virtual void resetFrame();

  //wrapper for StandardModule::executeFrame()
  virtual void executeFrame();

  //
  // GUI access functions
  //
  void setPlay(int play);
 
  Play getPlay(int frame=-1); 
 
  char* getPlayName();
 
  int currentPlayActive();
 
  bool playChanged();
  
  char* getPlayName(Play play);
 
  bool ToggleVision();
  
  //this funciton fills buffer with the names of all of the plays, starting after NO_PLAY
  //buffer should be a large char array (at least 20*NUM_PLAYS, maybe more).
  void getAllPlayNames(char* buffer);
 
  char* getPositionText(Position p) const;
  char* getPositionName(Position p) const;
  char* getRobotType(int i);
  void setRobotType(int i, int type);

  //-----------------------------------
  // Set Robot Position or Current Play
  //-----------------------------------
  //Get's robot ID's position (e.g. midfielder==1)
  Position GetRobotPosition(RobotIndex ID, int frame=-1);
 
  //Sets robot ID's positition
  void SetRobotPosition(int robotID, int newPosition);
 
  void Initialize2002Positions(); 
  
  void ResetPositions(); 
  
  float GetRobotDestinationX(int ID, int frame=-1);
 
  float GetRobotDestinationY(int ID, int frame=-1);
 
  float GetRobotDestinationRotation(int ID, int frame=-1);
  
  float GetRobotPassX(int ID, int frame=-1);
 
  float GetRobotPassY(int ID, int frame=-1);
 
  int GetRobotPassValue(int ID, int frame=-1);
 
  virtual void shutDown();

  void resetPlay();

  //Skill functions
  void evaluateSkillz();
  void updateSkillz();
  void loadSkillParams();
  void loadPlayParams();
  void loadWeatherParams();
  void loadTransitionParams();
  void resetSkillz();

  void reloadParameters();
  void reloadTrajectoryParameters();

  //----------------------------------
  //samples vision speed

  void sampleFrameRate();

  bool togglePositionSwitching();
  bool togglePlayTransitions();

protected:
  //Points to the same object as strategy
  RoboCupStrategyModule* RCstrategy;

  Client *client_;
};


#endif // RC_STANDARD_MODULE_H
