#ifndef PLAYS_KICK_UPFIELD_H
#define PLAYS_KICK_UPFIELD_H

#include "../baseplay.h"
#include "../basic_actions.h"

class KickUpfield : public BasePlay 
{
public:
  KickUpfield(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Kick Upfield"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_KICK_UPFIELD_H

