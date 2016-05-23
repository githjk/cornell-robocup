/*
*	visionTester.h
*/
#ifndef PLAYS_VISIONTESTER_PLAY_H
#define PLAYS_VISIONTESTER_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"

class VisionTesterPlay : public BasePlay 
{

private:
  struct robotStep
  {
    int index;
    int boxOffsetY;

    int lostFrames;
    bool disabled;
  };
  
public:
  VisionTesterPlay(VisionModule *vm);
  static Play playNum;
  Play getID() const 
  {
    return playNum;
  }
  
  char* getName() const 
  { 
    return "Vision Tester Play"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos);
  
  robotStep steps[NUM_ROBOTS];
  bool outOfFormation;

  void getDest(int index,
               int yStep,
               RobocupStrategyData* rsd,
               Destination& result
              );


#define xRes NUM_ROBOTS
#define yRes 2
#define ySteps 10

#define maxLostFrames 5
};



#endif //PLAYS_VISIONTESTER_PLAY_H

