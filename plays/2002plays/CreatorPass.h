#ifndef PLAYS_CREATOR_PASS_H
#define PLAYS_CREATOR_PASS_H

#include "../baseplay.h"
#include "../basic_actions.h"

class CreatorPass : public BasePlay 
{
public:
  CreatorPass(VisionModule *vm);
 
  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Creator Pass"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_CREATOR_PASS_H

