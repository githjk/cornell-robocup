#ifndef PLAYS_BACK_PASS_H
#define PLAYS_BACK_PASS_H

#include "../baseplay.h"
#include "../basic_actions.h"

class BackPass : public BasePlay 
{
public:
  BackPass(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Back Pass"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_BACK_PASS_H

