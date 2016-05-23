/*
 *	runlaps.h
 *  Makes the robots run laps
 */

#ifndef RUNLAPS_H
#define RUNLAPS_H

#include "../baseplay.h"
#include "../basic_actions.h"

enum LapState
{
  runningUp,
  runningRight,
  runningDown,
  runningLeft
};

class RunLaps : public BasePlay {
public:
  RunLaps(VisionModule *vm);

  static Play playNum;

  Play getID() const { return playNum; }
  char* getName() const { return "Run Laps"; }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

private:
  LapState lapState[NUM_ROBOTS];
};

#endif //RUNLAPS_H
