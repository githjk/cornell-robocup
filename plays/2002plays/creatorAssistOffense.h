#ifndef PLAYS_CREATOR_ASSIST_DEFENSE_H
#define PLAYS_CREATOR_ASSIST_DEFENSE_H

#include "../baseplay.h"
#include "../basic_actions.h"

class CreatorAssistOffense : public BasePlay 
{
public:

  CreatorAssistOffense(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Creator Assist Offense"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_CREATOR_ASSIST_DEFENSE_H

