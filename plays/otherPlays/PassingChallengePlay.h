/*
 *	PassingChallengePlay.h
 */
#ifndef PASSING_CHALLENGE_PLAY_H
#define PASSING_CHALLENGE_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/AcquirePossessionSkill.h"
#include "Skillz/SimpleKickSkill.h"
#include "Skillz/ReceivePassSkill.h"
#include "Skillz/RotateAroundBallSkill.h"

class PassingChallengePlay : public BasePlay 
{
public:
  static Play playNum;
  PassingChallengePlay(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "PassingChallengePlay"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos); 

  void uninitializeSkills();

  void loadValues();

  typedef enum {

  KICK_RECEIVE = 0,
  MOVE,
  ROTATE
  };

  typedef enum {

  LEFT = 0,
  RIGHT
  };

  int state;
  int turn;

  SkillSet* skills1;
  SkillSet* skills2;
  SimpleKickSkill* kickSkill1;
  SimpleKickSkill* kickSkill2;
  ReceivePassSkill* receiveSkill1 ;
  ReceivePassSkill* receiveSkill2;
  AcquirePossessionSkill* acquireSkill1 ;
  AcquirePossessionSkill* acquireSkill2;
  RotateAroundBallSkill* rotateSkill1;
  RotateAroundBallSkill* rotateSkill2;
  RobotIndex robot1ID,robot2ID;

  Pair dest,dest1;

  float POINT_X;
  float POINT_Y;
  float ROTATION_THRESHOLD, DISTANCE_THRESHOLD;
  float MAX_ELAPSED_TIME;
  int side0;
  int side1;

};

#endif //PASSING_CHALLENGE_PLAY_H

