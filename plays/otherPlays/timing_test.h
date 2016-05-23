/*
 *	timing_test.h
 */
#ifndef PLAYS_TIMING_TEST_H
#define PLAYS_TIMING_TEST_H

#define NUM_TIMING_POINTS 5

#include "../baseplay.h"
#include "../basic_actions.h"

class TimingTest : public BasePlay 
{
public:
  static Play playNum;
  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Timing Test Play"; 
  }
  TimingTest(VisionModule *vm) ;
 
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();

  float START_X,START_Y,
        END_X, END_Y,
        MID_X, MID_Y;
  float DISTANCE_FRACTION;
  float ROTATION_CHANGE;
  int state;
  Destination * dest;
  float predTime;
  float actualTime;
  Timer time;

};



#endif //PLAYS_TIMING_TEST_H

