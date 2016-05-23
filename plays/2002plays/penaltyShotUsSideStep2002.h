#ifndef PLAYS_PENALTY_SHOT_US_SIDE_STEP_2002_H
#define PLAYS_PENALTY_SHOT_US_SIDE_STEP_2002_H

#include "../baseplay.h"
#include "../basic_actions.h"

class PenaltyShotUsSideStep2002 : public BasePlay 
{
public:
  PenaltyShotUsSideStep2002(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Penalty Shot Us Side Step 2002"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_PENALTY_SHOT_US_SIDE_STEP_2002_H

