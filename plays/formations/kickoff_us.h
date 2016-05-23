/*
 *	kickoff_us.h
 */

#ifndef PLAYS_KICKOFF_US_H
#define PLAYS_KICKOFF_US_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "math/angle.h"

class KickoffUs : public BasePlay {
public:
  static Play playNum;

 KickoffUs(VisionModule *vm) : BasePlay(vm)
 {
 }

 Play getID()  const{ return playNum; }
  char* getName()  const{ return "Kickoff Us Formation"; }

  void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  
  void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

  void loadValues();
private:

  RobotIndex index;
  //----
  float BLOCKER_X;
  float BLOCKER_Y;
  float BLOCKER_ROTATION;
  float DEFENDER_X;
  float DEFENDER_Y;
  float DEFENDER_ROTATION;
  float AGGRESSOR_X;
  float AGGRESSOR_Y;
  float AGGRESSOR_ROTATION;
  float CREATOR_X;
  float CREATOR_Y;
  float CREATOR_ROTATION;
  float SPECIAL_OP_X;
  float SPECIAL_OP_Y;
  float SPECIAL_OP_ROTATION;
  //----
};

#endif //PLAYS_KICKOFF_US_H