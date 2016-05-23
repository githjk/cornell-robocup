/*
*	figure_eight.h
*/
#ifndef PLAYS_FIGURE_EIGHT_PLAY_H
#define PLAYS_FIGURE_EIGHT_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class FigureEightPlay : public BasePlay
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
  static Play playNum;
  Play getID() const 
  {
    return playNum;
  }
  
  char* getName() const 
  { 
    return "Figure Eight Play"; 
  }
  
  FigureEightPlay(VisionModule *vm);
  
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
  
	void loadValues();

  float FREQUENCY;
  float MAJOR;
  float MINOR;
  
  bool outOfFormation;  
};



#endif //PLAYS_FIGURE_EIGHT_PLAY_H

