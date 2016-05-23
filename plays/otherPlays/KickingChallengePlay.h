/*
 *	KickingChallengePlay.h
 */
#ifndef KICKING_CHALLENGE_PLAY_H
#define KICKING_CHALLENGE_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/AcquirePossessionSkill.h"
#include "Skillz/AdvanceBallUpfieldSkill.h"
#include "Skillz/JamAndShootSkill.h"


class KickingChallengePlay : public BasePlay 
{
public:
  static Play playNum;
  KickingChallengePlay(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Kicking Challenge"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  virtual ActionFunction getActionFunction(Position pos) { return stopRobot; }
  
  void doSingleAssist(RobocupStrategyData* rsd);
  void doUpfieldDefense(RobocupStrategyData* rsd);
  void doJamAndShoot(RobocupStrategyData* rsd);
  void uninitializeSkills();
  void loadValues();

  typedef enum {

  SINGLE_ASSIST = 0,
  JAM_AND_SHOOT,
  UPFIELD_DEFENSE
  };

  int state;
  
  SkillSet* skills;
  AcquirePossessionSkill* acquireSkill;
  AdvanceBallUpfieldSkill* advanceSkill;
  JamAndShootSkill* jamSkill;
  RobotIndex currentID;
  float DISTANCE_GOAL;

};

#endif //KICKING_CHALLENGE_PLAY_H

