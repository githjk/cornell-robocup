#ifndef PLAYS_DOUBLE_PICK_OFFENSE_H
#define PLAYS_DOUBLE_PICK_OFFENSE_H

#include "../baseplay.h"
#include "../basic_actions.h"

class DoublePickOffense : public BasePlay 
{
public:

  DoublePickOffense(VisionModule *vm);
  
  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Double Pick Offense"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_DOUBLE_PICK_OFFENSE_H

