#ifndef PLAYS_CAGE_DEFENSE_H
#define PLAYS_CAGE_DEFENSE_H

#include "../baseplay.h"
#include "../basic_actions.h"

class CageDefense : public BasePlay 
{
public:
  //Constructor.
  CageDefense(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Cage Defense"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_CAGE_DEFENSE_H

