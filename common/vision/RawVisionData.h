#ifndef RAW_VISION_DATA_H
#define RAW_VISION_DATA_H

#include "VisionConstants.h"
#include "FieldObjects.h"
#include "ai.h"
class VisionModule;

class RawVisionData {
  
public:
  
  friend class VisionModule;
  RawVisionData();
  
  //------------------------------------------------------------------------
  /// @return the team data
  const MovingObject* getTeam (int team) const;

  //------------------------------------------------------------------------
  /// @return a specified robot from a particular team
  const MovingObject& getRobot (int team, int robotIndex)  const;
  /// Set whether a specified robot from a particular team is found 
  void setRobotFound(int team, int robotIndex, bool condition);
  /// @return whether a specified robot from a particular team is found
  bool isRobotFound (int team, int robotIndex) const;

  /// Set a specified Robot X position from a particular team
  void setRobotXPos(int team, int number, float pos);
  /// Set a specified Robot Y position from a particular team
  void setRobotYPos(int team, int number, float pos);
  /// Set a specified Robot X velocity from a particular team
  void setRobotXVel(int team, int number, float pos);
  /// Set a specified Robot Y velocity from a particular team
  void setRobotYVel(int team, int number, float pos);
  /// Set a specified Robot rotation from a particular team
  void setRobotRotation(int team, int number, float pos);
  /// Set a specified Robot rotational velocity from a particular team
  void setRobotRotVel(int team, int number, float pos);

  //------------------------------------------------------------------------
  /// @return a specified ball from a particular team
  MovingObject getBall(int team, int number) const;
  /// Set whether a specified ball from a particular team is found
  void setBallFound(int team, int ballIndex, bool condition);
  /// @return whether a specified ball from a particular team is found
  bool isBallFound (int team, int ballIndex) const;


  /// Set the source of a ball
  void setBallSource(int team, int ballIndex, char source);
  /// @return the source of a specified ball from a particular team
  char getBallSource(int team, int number) const;

  /// Sets a specified ball position from a particular team
  void setBallPos(int team, int number, Pair pos);
  /// Set a specified ball X position from a particular team
  void setBallXPos(int team, int number, float pos);
  /// Set a specified ball Y position from a particular team
  void setBallYPos(int team, int number, float pos);
  /// Set a specified ball X velocity from a particular team
  void setBallXVel(int team, int number, float pos);
  /// Set a specified ball Y position from a particular team
  void setBallYVel(int team, int number, float pos);
  //------------------------------------------------------------------------
  float getNormalizedTranslationalError(int robot);
  float getNormalizedRotationalError(int robot);
  bool getResetTranslationalVelocity(int robot);
  bool getResetRotationalVelocity(int robot);
  float getDistanceTravelled(int robot);
  float getRotationTravelled(int robot);
  //------------------------------------------------------------------------


private:

  ///true if robot is found
  bool robotsFound[NUM_TEAMS][NUM_PLAYERS_ON_TEAM];
  /// 2D array of robots
  MovingObject teams[NUM_TEAMS][NUM_PLAYERS_ON_TEAM];

  ///true if ball is found
  bool ballsFound[NUM_TEAMS][NUM_BALLS];
  ///where the ball information is obtained from
  char ballSource[NUM_TEAMS][NUM_BALLS];
  ///2D array of balls
  MovingObject ball[NUM_TEAMS][NUM_BALLS];

  /// the distances covered by this robot between frames
  float distanceTravelled[NUM_PLAYERS_ON_TEAM];
  float rotationTravelled[NUM_PLAYERS_ON_TEAM];

  /// prediction error for our robots
  float predictedTranslationalError[NUM_PLAYERS_ON_TEAM];
  float predictedRotationalError[NUM_PLAYERS_ON_TEAM];

  /// normalized error for our robots
  float normalizedTranslationalError[NUM_PLAYERS_ON_TEAM];
  float normalizedRotationalError[NUM_PLAYERS_ON_TEAM];

  // set these booleans true to ignore the robot's commanded velocities for this frame
  bool resetTranslationalVelocity[NUM_PLAYERS_ON_TEAM];
  bool resetRotationalVelocity[NUM_PLAYERS_ON_TEAM];

  bool validData;

};


#endif
