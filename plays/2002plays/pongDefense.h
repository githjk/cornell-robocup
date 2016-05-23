#ifndef PLAYS_PONG_DEFENSE_H
#define PLAYS_PONG_DEFENSE_H

#include "../baseplay.h"
#include "../basic_actions.h"

class PongDefense : public BasePlay {
public:
  static Play playNum;

   PongDefense(VisionModule *vm) : BasePlay(vm)
   {
   }

  Play getID()  const{ return playNum; }
  char* getName()  const{ return "Pong Defense"; }

  void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

  void loadValues();

private:
  //----
  RobotIndex index;
  //----
};

#endif //PLAYS_PONG_DEFENSE_H
