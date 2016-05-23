/*
 *	PullBallOffWallSkill_Test.h
 */

#ifndef PLAYS_PULL_BALL_OFF_WALL_SKILL_TEST_H
#define PLAYS_PULL_BALL_OFF_WALL_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/Skillset.h"
#include "Skillz/PullBallOffWallSkill.h"

class PullBallOffWallSkillTest : public BasePlay {
public:
  static Play playNum;

  PullBallOffWallSkillTest(VisionModule *vm) : BasePlay(vm)
  {
  }

  Play getID() const {return playNum;}
  char* getName() const { return "Pull Ball Off Wall Skill"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

	virtual void executePlay(VisionData* vision, RobocupStrategyData *rsd);

  virtual ActionFunction getActionFunction(Position pos);

private:
	//Index of the robot that will be trying to get the ball
	RobotIndex ID;

	//Handle to its skillset
	SkillSet* skills;

	//Handle to the PullBall... skill
	PullBallOffWallSkill* pullBallOffWallSkill;

};

#endif //PLAYS_PULL_BALL_OFF_WALL_SKILL_TEST_H

