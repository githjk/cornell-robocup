#ifndef VISION_MODULE_H
#define VISION_MODULE_H


// Libraries Utilized

#include "common/datatypes/SystemParameters.h"
#include "TrajectoryAndControl.h"

#include "VisionData.h"
#include "RawVisionData.h"

#include "VisionPacket.h"
#include "VisionConstants.h"

#include "FieldObjects.h"
#include "FilteredObject.h"

#include "../network/Client.h"


/**
Class VisionModule : Captures the vision packet from the network, and sends it to the rest
of the AI class.
@author Chee Yong Lee (Rewritten from original 2001 source code
@version 1.0
June 2002
Changes include : (1) Usage of VisionPacket instead of FieldObjects
(2) Proper grouping of header and actual implementation functions in .h and .cpp file
(3) Revision of filtering code
**/


class VisionModule
{
public:
  
  //=================================================================
  // VisionModuleMainFunctions.cpp
  //=================================================================

  // constructors
  VisionModule(SystemParameters* sp);
  /// Destructor
  ~VisionModule();

  /// Flips data if we switch sides
  void FlipVision();

  /// informs the VisionModule that the parameters have changed.
  virtual void reloadParameters();
  
  /// Samples the vision to see what speed it is running at, sets FRAME_RATE accordingly
  void SampleVisionSpeed(Client* client);

  /// @receives the newest data from vision system
  virtual void getNewData(Client* client);

  /// @If vision disabled, clear the incoming vision buffer (30ms)
  void getNoVisionData();
  

  //=================================================================
  // VisionModulePrediction.cpp
  //=================================================================

  /** Predicts robot and ball positions forward to account for latency
  Updates the new vision frame, and calls predictFriendlyRobots,predictOpponentRobots
  and predictBalls.
  **/
  virtual void predictVision( ControlData command[MAX_LOG] );

  //=================================================================
  // VisionModulePredictionError.cpp
  //=================================================================

  void updateRawVision();

  //=================================================================
  // VisionModuleBasicFunctions.cpp
  //=================================================================

  Pair getLastUnpredictedBall();

  // TOGGLE FUNCTIONS
  //-----------------------------------------------------------------

  /// @return vision status (on/off)
  bool visionStatus(); 
  /// Toggles vision status (on/off)
  bool toggleVisionStatus();
  /// Sets vision status (on/off)
  bool setVisionStatus(bool newVisionStatus);
  /// @return sidesFlipped status (yes/no)

  bool areSidesFlipped();
  /// Toggles sidesFlipped status (yes/no)
  bool flipSides();

  /// retrieves frame
  VisionData* getCurrentFrame(int frame=-1);
  
  /// retrieves raw frame
  RawVisionData* getRawFrame(int frame=-1);

  /// sets currentFrame (assume it is a valid frame)
  void setFrame(int newFrame);

  

  // PLAYER ROBOT FUNCTIONS
  //-----------------------------------------------------------------

  /// @return whether the robot is found
  bool isRobotFound(int team,int num, int frame=-1);

  /// @return our robot x position
  float getOurRobotX(int id, int frame=-1);
  /// @return our robot x position
  float getOurRobotY(int id, int frame=-1);
  /// @return our robot x position
  float getOurRobotRawX(int id, int frame=-1);
  /// @return our robot x position
  float getOurRobotRawY(int id, int frame=-1);
  /// @return our robot x velocity
  float getOurRobotXVel(int id, int frame=-1);
  /// @return our robot y velocity
  float getOurRobotYVel(int id, int frame=-1);
  /// @return our robot rotation
  float getOurRobotRotation(int id, int frame=-1);
  /// @return our robot rotation
  float getOurRobotRawRotation(int id, int frame=-1);
  /// @return our robot rotation velocity 
  float getOurRobotRotationVel(int id, int frame=-1);

  // OPPONENT ROBOT FUNCTIONS
  //-----------------------------------------------------------------

  /// @return their robot x position
  float getTheirRobotX(int id, int frame=-1); 
  /// @return their robot y position
  float getTheirRobotY(int id, int frame=-1);
  /// @return their robot x position
  float getTheirRobotRawX(int id, int frame=-1); 
  /// @return their robot y position
  float getTheirRobotRawY(int id, int frame=-1);
  /// @return their robot x velocity
  float getTheirRobotXVel(int id, int frame=-1);
  /// @return their robot y velocity
  float getTheirRobotYVel(int id, int frame=-1);

  // BALL FUNCTIONS
  //-----------------------------------------------------------------

  ///@return whether a ball is found
  bool isBallFound(int team,int num, int frame=-1);

  /// @return the ball x position
  float getBallX(int team,int num, int frame=-1);
  /// @return the ball y position
  float getBallY(int team,int num, int frame=-1);
  /// @return the ball x position
  float getBallRawX(int team,int num, int frame=-1);
  /// @return the ball y position
  float getBallRawY(int team,int num, int frame=-1);
  /// @return the ball x velocity
  float getBallXVel(int team,int num, int frame=-1);
  /// @return the ball y velocity
  float getBallYVel(int team,int num, int frame=-1);

  // BALL SOURCE FUNCTIONS
  //-----------------------------------------------------------------

  /// @return the source of the source as specified in Vision Packet. MAIN_SOURCE =1, AUX_SOURCE = 2
  char getBallSource(int team, int num, int frame=-1);
 

  
protected:

  //-----------------------------------------------------------------
  // PROTECTED VARIABLES
  //-----------------------------------------------------------------

  //buffers last seen raw ball
  Pair lastUnpredictedBall;

  /// SystemParameters of AI
  SystemParameters*   sp;

  /// Whether vision is running
  bool      visionRunning;

  /// Have we flipped the sides of the field?
  bool      sidesFlipped;

  /// The current active frame number
  int       currentFrame;

  /// A log of past vision data
  VisionData        visionData[MAX_LOG];
  RawVisionData  rawVisionData[MAX_LOG];

  /// The data structure for incoming Vision
  FieldData incomingVision;

  /// The data structure for incoming packet
  VisionPacket* incomingPacket;
  
  /// general variables for positions and velocity
  float XPos, YPos; 
  float XVel, YVel;
  
  // Filter Array
  VisionData filter[NUM_VISION_SNAPSHOTS];
  
  // Filter Ball Array
  FilteredObject filteredBalls[NUM_TEAMS][NUM_BALLS];
  // Filter Friendly Robot Array
  FilteredObject filteredOpponents[NUM_PLAYERS_ON_TEAM];
  
  // Time variables
  int num_latency_frames,other_num_latency_frames;
  float latency_frame,  squared_latency_frame;
  float latency_fraction, squared_latency_fraction,other_latency_fraction,other_squared_latency_fraction;
  
  // Prediction Error variables
  float translationalPathLength[NUM_PLAYERS_ON_TEAM];
  float rotationalPathLength[NUM_PLAYERS_ON_TEAM];
  int translationalErrorCount[NUM_PLAYERS_ON_TEAM];
  int rotationalErrorCount[NUM_PLAYERS_ON_TEAM];

  // Track vision status
  int robotLost[NUM_PLAYERS_ON_TEAM];
  int opponentLost[NUM_PLAYERS_ON_TEAM];
  int ballLost[NUM_TEAMS][NUM_BALLS];
  
  //=================================================================
  // VisionModuleMainFunctions.cpp
  //=================================================================

  /// receive data from the network
  bool receiveVisionData(Client* client);

  //=================================================================
  // VisionModulePrediction.cpp
  //=================================================================

  // MAIN PREDICTION FUNCTIONS
  //-----------------------------------------------------------------

  /// Predicts friendly robots using previous control data
  void predictFriendlyRobots(int robot,ControlData command[MAX_LOG]);

  /// Predicts opponent robots using filtering (and possibly heuristics)
  void predictOpponentRobots(int robot);

  /// Predicts ball movements using filtering
  void predictBalls(int team,int ball);

  // PREDICTION UTILITY FUNCTIONS
  //-----------------------------------------------------------------

  /// initialize the variables used for prediction
  void InitializePrediction();
  
  /** Predict the ball forward motion
  * get the position of the ball from vision data(which is lagging by the
  * latency number of frames from the robot frame of reference)%%
  **/
  void predictBallForward(int team, int ball);

  /// apply kalman filter in the FilteredObject class to get predicted position and velocity for opponent robots
  void filterOpponentLocation(int robotID,float posx, float posy);
  
  /// apply kalman filter in the FilteredObject class to get predicted position and velocity for balls
  void filterBallLocation(int team, int ball, float posx, float posy);
  
  /// calculate the step X velocity to be sent to the robot
  void Velocity_StepXPos(ControlData command[MAX_LOG], int team, int robot, float timeStep,int frame);
  /// calculate the step Y velocity to be sent to the robot
  void Velocity_StepYPos(ControlData command[MAX_LOG],int team,int robot,float timeStep,int frame);
  /// calculate the step rotation for the velocity to be sent to the robot
  void Velocity_StepRotation(ControlData command[MAX_LOG],int team,int robot,float timeStep, int frame);


  //=================================================================
  // VisionModulePredictionError.cpp
  //=================================================================

  /// loads the incoming vision into the current frame
  void loadRawVision();

  /// calculates our prediction error compared to the raw vision data
  void calculatePredictionError();

  /// evaluates our prediction error to determine if our robot is stuck
  void evaluatePredictionError();


  //=================================================================
  // VisionModuleBasicFunctions.cpp
  //=================================================================

  
};

#endif // VISION_MODULE_H



