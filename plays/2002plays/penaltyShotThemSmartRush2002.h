#ifndef PLAYS_PENALTY_SHOT_THEM_SMART_RUSH_2002_H
#define PLAYS_PENALTY_SHOT_THEM_SMART_RUSH_2002_H

#include "../baseplay.h"
#include "../basic_actions.h"

class PenaltyShotThemSmartRush2002 : public BasePlay 
{
public:
  PenaltyShotThemSmartRush2002(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Penalty Shot Them Smart Rush 2002"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_PENALTY_SHOT_THEM_SMART_RUSH_2002_H

