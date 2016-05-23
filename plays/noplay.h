/*
 *	noplay.h
 */

#ifndef PLAYS_NOPLAY_H
#define PLAYS_NOPLAY_H

#include "baseplay.h"
#include "basic_actions.h"

class NoPlay : public BasePlay {
public:
  static Play playNum;
  NoPlay(VisionModule *vm) : BasePlay(vm)
  {
  }
  Play getID()  const{return playNum;}
  char* getName()  const{ return "No Play"; }

  void initializePlay(const VisionData& vision, RobocupStrategyData* rsd){}
  void executePlay(VisionData* vision, RobocupStrategyData* rsd)
  {
    //Nothing to do, the destinations have already been initialized to the current position with 0 velocity
    for (RobotIndex index = ROBOT0; index < NUM_ROBOTS; index++)
      stopRobot(index, this, *vision, rsd);

  }

  virtual ActionFunction getActionFunction(Position pos) {return stopRobot;}

};

#endif //PLAYS_NOPLAY_H

