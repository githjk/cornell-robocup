/*
 *	random_play.h
 */

#ifndef PLAYS_RANDOM_PLAY_H
#define PLAYS_RANDOM_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class RandomPlay : public BasePlay {
public:
  static Play playNum;
  RandomPlay(VisionModule *vm);

  
  Play getID() const {return playNum;}
  char* getName() const { return "Random Play"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  
//  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);
};

#endif //PLAYS_RANDOM_PLAY_H

