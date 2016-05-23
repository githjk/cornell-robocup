#ifndef PLAYS_LOOSE_DEFENSE_H
#define PLAYS_LOOSE_DEFENSE_H

#include "../baseplay.h"
#include "../basic_actions.h"

class LooseDefense : public BasePlay 
{
public:
  LooseDefense(VisionModule *vm);
 
  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Loose Defense"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_LOOSE_DEFENSE_H

