#ifndef PLAYS_TOUCH_SHOOT_H
#define PLAYS_TOUCH_SHOOT_H

#include "../baseplay.h"
#include "../basic_actions.h"

class TouchShoot : public BasePlay 
{
public:
  TouchShoot(VisionModule *vm);

  static Play playNum;
  
  Play getID() const {return playNum;}

  char* getName() const { return "Touch Shoot"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  void loadValues();
};

#endif //PLAYS_TOUCH_SHOOT_H

