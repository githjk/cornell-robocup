/*
 *	will_play.h
 */
#ifndef PLAYS_WILL_PLAY_H
#define PLAYS_WILL_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

#define MAX_KEPT 1000
struct vision_and_commands
{
  Pair rawPosition;
  float rawRotation;

  Pair predictedPosition;
  float predictedRotation;

  Pair lastPositionCommand;
  float lastRotationCommand;
};

class WillPlay : public BasePlay 
{
public:
  float t;
  float Vx;
  float lastX;
  int frame1, frame2, counter, index;
  float x1, x2;
  bool accel;
  bool decel;
  bool stop;
  float Amax, Vmax;
  static Play playNum;
 WillPlay(VisionModule *vm);
 Play getID() const 
  {
	  return playNum;
  }
  
 void loadValues();
  char* getName() const 
  { 
	  return "Will Play"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  virtual ActionFunction getActionFunction(Position pos);
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  Skill* skillHandle;

  vision_and_commands* log;

  VisionModule *vm;

};





#endif //PLAYS_WILL_PLAY_H

