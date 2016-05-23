/*
 *	cones_play.h
 */
#ifndef PLAYS_CONES_PLAY_H
#define PLAYS_CONES_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class ConesPlay : public BasePlay 
{
public:
  ConesPlay(VisionModule *vm);
  static Play playNum;
  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Cones"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos);

  float separationDistance;
  #define pushBackDistance 0.15f
  int activeRobot;
  Pair Offsets[NUM_ROBOTS];
  Pair striveFor[16];
  int goingTo;
};

#endif //PLAYS_CONES_PLAY_H

