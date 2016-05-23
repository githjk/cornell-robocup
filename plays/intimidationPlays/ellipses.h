/*
*	ellipses.h
*/
#ifndef PLAYS_ELLIPSES_PLAY_H
#define PLAYS_ELLIPSES_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class EllipsesPlay : public BasePlay 
{
private:
  struct translation
  {
    float Radius;
    float Frequency;
    float Phase;
  };
  
  struct rotation
  {
    float Frequency;
    float Phase;
  };

public:
  EllipsesPlay(VisionModule *vm);
  static Play playNum;
  Play getID() const 
  {
    return playNum;
  }
  
  char* getName() const 
  { 
    return "Ellipses Play"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos);
  
  void PosCalculate(translation Translate,
                    rotation Rotate,
                    float Time,
                    Destination* desiredPosition);
  
  void VelCalculate(translation Translate,
                    rotation Rotate,
                    float Time,
                    float VelFactor,
                    Destination* desiredPosition);
  
  translation Translations[NUM_ROBOTS];
  rotation Rotations[NUM_ROBOTS];
  
  float MAJOR;
  float MINOR;
  
  bool outOfFormation; 
};



#endif //PLAYS_ELLIPSES_PLAY_H

