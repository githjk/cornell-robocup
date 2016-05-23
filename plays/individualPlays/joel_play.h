/*
 *	joel_play.h
 */

#ifndef PLAYS_JOEL_PLAY_H
#define PLAYS_JOEL_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class JoelPlay : public BasePlay {
public:
  static Play playNum;
  JoelPlay(VisionModule *vm) ;

  Play getID() const {return playNum;}
  char* getName() const { return "Joel Play"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);


};

#endif //PLAYS_JOEL_PLAY_H

