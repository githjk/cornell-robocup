/*
 *	torturetest.h
 *  Makes the robots run laps while dribbling and kicking
 */

#ifndef TORTURETEST_H
#define TORTURETEST_H

#include "../baseplay.h"
#include "../basic_actions.h"

//LapState already defined in RunLaps
enum TestState
{
  testUp,
  testRight,
  testDown,
  testLeft
};

class TortureTest : public BasePlay {
public:
 TortureTest(VisionModule *vm);
 static Play playNum;

  Play getID() const { return playNum; }
  char* getName() const { return "Torture Test"; }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

private:
  TestState testState[NUM_ROBOTS];
};

#endif //TORTURETEST_H
