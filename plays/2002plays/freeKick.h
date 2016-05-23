#ifndef PLAYS_FREE_KICK_H
#define PLAYS_FREE_KICK_H

#include "../baseplay.h"
#include "../basic_actions.h"

class FreeKick : public BasePlay 
{
public:
  FreeKick(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Free Kick"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_FREE_KICK_H

