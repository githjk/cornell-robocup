/*
 *	cut_goal_skill_test.h
 */
#ifndef PLAYS_LOOSE_DEFENSE_SKILL_TEST_H
#define PLAYS_LOOSE_DEFENSE_SKILL_TEST_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"
#include "Skillz/TandemDefenderSkill.h"
#include "Skillz/TandemBlockerSkill.h"

class LooseDefenseSkillTest : public BasePlay 
{
public:
  static Play playNum;
  LooseDefenseSkillTest(VisionModule *vm) : BasePlay(vm)
  {
  }

  //====================================
  Play getID() const 
  {
	  return playNum;
  }
  //====================================
  char* getName() const 
  { 
	  return "Loose Defense Skill"; 
  }
  //====================================
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  //====================================
  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  //====================================
  virtual ActionFunction getActionFunction(Position pos); 
  //====================================
private:
  //Index of robot that will be screening
  RobotIndex ID;

  //Index of the defender robot
  RobotIndex defenderID;

  //Handle on skills
  SkillSet* skillsdefender;
  SkillSet* skillsblocker;

  //Handle on skills
  TandemDefenderSkill* tandemDefenderSkill;
  TandemBlockerSkill* tandemBlockerSkill;
};

#endif //PLAYS_LOOSE_DEFENSE_SKILL_TEST_H