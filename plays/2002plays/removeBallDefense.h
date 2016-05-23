#ifndef PLAYS_REMOVE_BALL_DEFENSE_H
#define PLAYS_REMOVE_BALL_DEFENSE_H

#include "../baseplay.h"
#include "../basic_actions.h"

class RemoveBallDefense : public BasePlay 
{
public:
  RemoveBallDefense(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Remove Ball Defense"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_REMOVE_BALL_DEFENSE_H

