/*
 *	radar_play.h
 */
#ifndef PLAYS_RADAR_PLAY_H
#define PLAYS_RADAR_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class RadarPlay : public BasePlay 
{
public:
  RadarPlay(VisionModule *vm);
  static Play playNum;
  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Radar"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos);

  float angle;
  float lookAngleOffset1[NUM_ROBOTS];
  float lookAngleOffset2[NUM_ROBOTS];
  float offsets[NUM_ROBOTS];
};



#endif //PLAYS_RADAR_PLAY_H

