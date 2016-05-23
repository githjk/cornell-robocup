#ifndef PLAYS_SINGLE_ASSIST_OFFENSE_H
#define PLAYS_SINGLE_ASSIST_OFFENSE_H

#include "../baseplay.h"
#include "../basic_actions.h"

class SingleAssistOffense : public BasePlay 
{
public:
  SingleAssistOffense(VisionModule *vm); 

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Single Assist Offense"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_SINGLE_ASSIST_OFFENSE_H

