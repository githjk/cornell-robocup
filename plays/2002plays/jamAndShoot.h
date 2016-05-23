#ifndef PLAYS_JAM_AND_SHOOT_H
#define PLAYS_JAM_AND_SHOOT_H

#include "../baseplay.h"
#include "../basic_actions.h"

class JamAndShoot : public BasePlay 
{
public:

  JamAndShoot(VisionModule *vm);
	
  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Jam And Shoot"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_JAM_AND_SHOOT_H

