/*
 *	nirav_play.h
 */
#ifndef PLAYS_NIRAV_PLAY_H
#define PLAYS_NIRAV_PLAY_H

#include "../baseplay.h"
#include "../basic_actions.h"
#include "Skillz/SkillSet.h"

#include "Skillz/TightDefenseDefenderSkill.h"

#include "Skillz/TightDefenseBlockerSkill.h"
#include "Skillz/CageDefenseSkill.h"

#include "Skillz/SpecialOpCageDefenseSkill.h"
#include "Skillz/BlockerGainPossessionSkill.h"

class NiravPlay : public BasePlay 
{
public:
  static Play playNum;
  NiravPlay(VisionModule *vm);
  Play getID() const 
  {
	  return playNum;
  }
  
  char* getName() const 
  { 
	  return "Nirav Play"; 
  }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);
  
  virtual ActionFunction getActionFunction(Position pos);

  //Index of robot that will be spinning
  RobotIndex ID;

  RobotIndex defenderID;

  RobotIndex specialOpDefenderID;

  //Handle on skillset for the spinning robot
  SkillSet* skills;

  SkillSet* skills1;


  //Handle on spinning skill
  //TightDefenseBlockerSkill* blockerSkill;

  //TightDefenseDefenderSkill* defenderSkill;
  SpecialOpCageDefenseSkill*  specialOpSkill;
  BlockerGainPossessionSkill* blockerSkill;

  CageDefenseSkill* aggressorSkill;

  CageDefenseSkill* defenderSkill;
  int state ;


};



#endif //PLAYS_NIRAV_PLAY_H

