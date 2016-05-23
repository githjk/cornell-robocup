/*
 *	Snowflake Tester
 */

#ifndef SNOWFLAKE_TESTER_H
#define SNOWFLAKE_TESTER_H

#include "../baseplay.h"
#include "../basic_actions.h"

class SnowflakeTester : public BasePlay {
public:
  static Play playNum;
  SnowflakeTester(VisionModule *vm);
 
  ~SnowflakeTester();

  Play getID() const { return playNum; }

  char* getName() const { return "Snowflake Tester"; }

  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }

private:
  int step;
  int mode;
  int maxSteps;
  Pair* dests;
  Pair origin;
};

#endif //SNOWFLAKE_TESTER_H
