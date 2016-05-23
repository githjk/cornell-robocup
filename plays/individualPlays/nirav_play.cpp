#include "info/robot/robot.h"
#include "math/dist.h"
#include "nirav_play.h"
#include "../basic_actions.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/TightDefenseDefenderSkill.h"
#include "Skillz/TightDefenseBlockerSkill.h"



Play NiravPlay::playNum = -1;

NiravPlay::NiravPlay(VisionModule *vm) : BasePlay(vm)
{
  loadValues();
}

void NiravPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

  RobotIndex a,b,d,s;
  a=ROBOT1;
  b=ROBOT0;
  d=ROBOT2;
  s=ROBOT3;

  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(b);
  blockerSkill = (BlockerGainPossessionSkill*)(skills->getSkill(BlockerGainPossessionSkill::skillNum));
  blockerSkill->initialize();

  skills = rsd->getStrategyModule().getSkillSet(a);
  aggressorSkill = (CageDefenseSkill*)(skills->getSkill(CageDefenseSkill::skillNum));
  aggressorSkill->initialize();

  skills = rsd->getStrategyModule().getSkillSet(d);
  defenderSkill = (CageDefenseSkill*)(skills->getSkill(CageDefenseSkill::skillNum));
  defenderSkill->initialize();
  
  skills = rsd->getStrategyModule().getSkillSet(s);
  specialOpSkill = (SpecialOpCageDefenseSkill*)(skills->getSkill(SpecialOpCageDefenseSkill::skillNum));
  specialOpSkill->initialize();





}

ActionFunction NiravPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void NiravPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  Pair dest(1.0f, 0.0f);
  Pair dest1(-1.0f,0.0f);
  
  
  if(getLocation(ROBOT4, *vision, rsd->getSystemParams()).getX() <= dest1.getX()+0.1)
  {
    state = 0;
    
  }
  if(getLocation(ROBOT4, *vision, rsd->getSystemParams()).getX() >= dest.getX()-0.1)
  {
    state = 1;
    
  }

  if(state == 0)
  {
    rsd->getDestination(4)->setPos(dest);
    rsd->getDestination(4)->setControl(OMNI_NORMAL);
  }
  else
  {
    rsd->getDestination(4)->setPos(dest1);
    rsd->getDestination(4)->setControl(OMNI_NORMAL);
  }
/*
  blockerSkill->run();
  defenderSkill->run();
  aggressorSkill->run();
  */
  /*	
  Pair us(getLocation(rsd->getParams().TEAM, (RobotIndex)0, *vision));
  Pair next(0.9f,0.4f);

  GUI_Record.debuggingInfo.setDebugPoint(0, 0, next);
  rsd->getDestination(0)->setPos(next);
  rsd->getDestination(0)->setRotation(angleBetween(Pair(0.0f,0.0f), next));
  rsd->getDestination(0)->setControl(OMNI_NORMAL_ANY_SPEED);
  rsd->getDestination(0)->setSpeed(DEFAULT_SPEED);

  GUI_Record.debuggingInfo.addDebugMessage("hello world!");
  */
}