/*
 *	LED_latency_test.h
 */

#ifndef LED_LATENCY_TEST_H
#define LED_LATENCY_TEST_H

//how many of frames to do latency test
const int numberOfFramesforTest = 200;

#include "../baseplay.h"
#include "../basic_actions.h"

class LED_LatencyTest : public BasePlay {
public:
  static Play playNum;
  LED_LatencyTest(VisionModule *vm);

  Play getID() const { return playNum; }
  char* getName() const { return "LED Latency Test"; }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

private:

  RobotIndex index;

  //store old lost frames # for ball
  int ballLostFrames;

  //counts down how of frames to do latency test
  int counter;

  //results =-)
  bool ballStatus[numberOfFramesforTest];
};

#endif //LED_LATENCY_TEST_H
