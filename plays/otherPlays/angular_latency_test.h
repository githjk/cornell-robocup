/*
 *	angular_latency_test.h
 */

#ifndef ANGULAR_LATENCY_TEST_H
#define ANGULAR_LATENCY_TEST_H

//how many of frames to do latency test
//const int numberOfFramesforTest = 200;

#include "../baseplay.h"
#include "../basic_actions.h"

enum AngularLatencyTestState
{
  ALT_MOVING_TO_START,
  ALT_TURNING_QUICKLY
};


class AngularLatencyTest : public BasePlay {
public:
  static Play playNum;
  AngularLatencyTest(VisionModule *vm);

  Play getID() const { return playNum; }
  char* getName() const { return "Angular Gain Latency Test"; }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

  virtual void endPlay(RobocupStrategyData* rsd);

private:

  //Index of Robot to use:
  RobotIndex index;

  //store the original parameters:
  float oldVThetaMax;
  float oldDefaultAG;
  float oldLatencyFrames;
  float stabilityThresholdTheta;

  //counts down how of frames to do latency test
  float time;

  // K is the angular gain constant (used as shorthand for "angular gain")
  float currentK;
  float highK;
  float lowK;
  
  //output message
  char out[80];


  AngularLatencyTestState state;

  void updateK(bool stable, RobocupStrategyData* rsd);
};

#endif //ANGULAR_LATENCY_TEST_H
