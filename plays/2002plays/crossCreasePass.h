#ifndef PLAYS_CROSS_CREASE_PASS_H
#define PLAYS_CROSS_CREASE_PASS_H

#include "../baseplay.h"
#include "../basic_actions.h"

class CrossCreasePass : public BasePlay 
{
public:
  CrossCreasePass(VisionModule *vm);
 
  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Cross Crease Pass"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_CROSS_CREASE_PASS_H

