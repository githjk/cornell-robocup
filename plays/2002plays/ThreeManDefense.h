#ifndef PLAYS_THREE_MAN_DEFENSE_H
#define PLAYS_THREE_MAN_DEFENSE_H

#include "../baseplay.h"
#include "../basic_actions.h"

class ThreeManDefense : public BasePlay 
{
public:
  ThreeManDefense(VisionModule *vm);
 
  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Three Man Defense"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_THREE_MAN_DEFENSE_H

