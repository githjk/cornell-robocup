/*
 *	prediction_test_play.h
 *  Makes the robots run laps, stop at corners, face various directions
 */

#ifndef PREDICTION_TEST_PLAY_H
#define PREDICTION_TEST_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

//to store what leg of the lap we are on
enum TestLapState
{
  runningUpLong,
  runningRightCross,
  runningDownLong,
  runningLeftCross
};

//to store whether we are paused at corner or are running
enum TestWaitState
{
	waiting,
	running
};


class PredictionTestPlay : public BasePlay {
public:
  static Play playNum;
  PredictionTestPlay(VisionModule *vm) ;

  Play getID() const { return playNum; }
  char* getName() const { return "Prediction Test Play"; }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

private:
  TestLapState testLapState[NUM_ROBOTS];
	TestWaitState testWaitState[NUM_ROBOTS];
	//rot permits incrementing of the rotation of the robot on each lap
	float rot;
	//waitCounter is a cheap timer for the waiting at corners
	int waitCounter;
};

#endif //PREDICTION_TEST_PLAY_H
