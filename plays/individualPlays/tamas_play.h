/*
 *	tamas_play.h
 */

#ifndef PLAYS_TAMAS_PLAY_H
#define PLAYS_TAMAS_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class TamasPlay : public BasePlay {
public:
  static Play playNum;
  TamasPlay(VisionModule *vm);

  Play getID() const {return playNum;}
  char* getName() const { return "Tamas Play"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);


};

#endif //PLAYS_TAMAS_PLAY_H

