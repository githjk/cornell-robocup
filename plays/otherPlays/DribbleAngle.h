/*
 *	dribbleAngle.h
 */
#ifndef PLAYS_DRIBBLE_ANGLE_TEST_H
#define PLAYS_DRIBBLE_ANGLE_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "skillz/SkillSet.h"
#include "skillz/SpinSkill.h"
#include "skillz/MoveToBallSkill.h"

class DribbleAngleTest : public BasePlay 
{
public:
  static Play playNum;
 DribbleAngleTest(VisionModule *vm);
 
 Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Dribble Angle Play"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos);

  bool started; 
  int state;

  float highAngle;
  float lowAngle;
  float angle;

  char outMessage[40];
  RobotIndex robot;
  Pair startPos;
  Pair endPos;

  SkillSet *skills;
  MoveToBallSkill * moveSkill;
  SpinAroundBallSkill * spinSkill;
  Timer timer;

};



#endif //PLAYS_DRIBBLE_ANGLE_TEST_H

