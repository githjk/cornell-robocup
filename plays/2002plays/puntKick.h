#ifndef PLAYS_PUNT_KICK_H
#define PLAYS_PUNT_KICK_H

#include "../baseplay.h"
#include "../basic_actions.h"

class PuntKick : public BasePlay 
{
public:
  PuntKick(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Punt Kick"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_PUNT_KICK_H

