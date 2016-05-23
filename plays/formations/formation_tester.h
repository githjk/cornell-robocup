#ifndef PLAYS_FORMATION_TESTER_H
#define PLAYS_FORMATION_TESTER_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "math/angle.h"

class FormationTester : public BasePlay {
public:
  static Play playNum;

  FormationTester(VisionModule *vm) : BasePlay(vm) { }
 
  Play getID()  const{ return playNum; }

  char* getName()  const{ return "Formation Tester"; }

  void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

  void loadValues();

private:
  Pair location[NUM_ROBOTS];
  float rotation[NUM_ROBOTS];
  bool found[NUM_ROBOTS];

  int robotID;
  int mode;
  bool moveOn;
};

#endif //PLAYS_FORMATION_TESTER_H
