/*

 *	simple_defense_skill_test.h

 */

#ifndef PLAYS_SIMPLE_DEFENSE_SKILL_TEST_H

#define PLAYS_SIMPLE_DEFENSE_SKILL_TEST_H



#include "../baseplay.h"

#include "../basic_actions.h"

#include "Skillz/SkillSet.h"

#include "Skillz/SimpleDefenseSkill.h"



class SimpleDefenseSkillTest : public BasePlay 

{

public:

  static Play playNum;

  SimpleDefenseSkillTest(VisionModule *vm) : BasePlay(vm)
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

	  return "Simple Defense Skill"; 

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



  //Handle on skillset for the screening robot

  SkillSet* skills;



  //Handle on simple defense skill

  SimpleDefenseSkill* defenseSkill;

};



#endif //PLAYS_SIMPLE_DEFENSE_SKILL_TEST_H



