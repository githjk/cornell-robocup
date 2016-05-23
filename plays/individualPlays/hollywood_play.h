/*
 *	hollywood_play.h
 */

#ifndef PLAYS_HOLLYWOOD_PLAY_H
#define PLAYS_HOLLYWOOD_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

#include "time.h"

//-------------------------------------------------

class HollywoodPlay : public BasePlay {
public:
  static Play playNum;
  HollywoodPlay(VisionModule *vm) ;

  Play getID() const {return playNum;}
  char* getName() const { return "Hollywood Play"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  virtual ActionFunction getActionFunction(Position pos);

private:

};





#endif //PLAYS_HOLLYWOOD_PLAY_H

