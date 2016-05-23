#include "RegisterSkillz.h"
#include "SkillSetIncludes.h"

///registers each skill'z special skill number
void registerSkillz()
{
  numSkillz = 0;

  //-----
  //for every skill, set skill number
  //-----
  DribblePointSkill::skillNum= numSkillz++;
  PullBallOffCornerSkill::skillNum = numSkillz++;
  JukeAroundOpponentSkill::skillNum = numSkillz++;
  SimpleScreenSkill::skillNum = numSkillz++;
  PenaltyShotUsAimAndShootSkill::skillNum = numSkillz++;
  SupplementThreeManSkill::skillNum = numSkillz++;
  SpinAroundBallSkill::skillNum = numSkillz++;
  RotateAroundBallSkill::skillNum = numSkillz++;
  ManeuverWithBallSkill::skillNum = numSkillz++;
  TurnAndKickSkill::skillNum = numSkillz++;
  InterceptBallSkill::skillNum = numSkillz++;
  DiveSkill::skillNum = numSkillz++;
  TandemDefenderSkill::skillNum = numSkillz++;
  TightDefenseSpecialOpDSkill::skillNum = numSkillz++;
  PassBlockSkill::skillNum = numSkillz++;
  ShotBlockSkill::skillNum = numSkillz++;
  SimpleKickSkill::skillNum = numSkillz++;
  ThreeManBlockerSkill::skillNum = numSkillz++;
  ThreeManSideSkill::skillNum = numSkillz++;
  CreatorPassSkill::skillNum = numSkillz++;
  BackPassSkill::skillNum = numSkillz++;
  TestSkill::skillNum = numSkillz++;
  MoveToBallSkill::skillNum = numSkillz++;  
  PickSkill::skillNum = numSkillz++;
  MoveAroundPickSkill::skillNum = numSkillz++;
  StripBallSkill::skillNum = numSkillz++;
  LooseDSpecialOpSkill::skillNum = numSkillz++;
  TightDefenseBlockerSkill::skillNum = numSkillz++;
  SpecialOpCageDefenseSkill::skillNum = numSkillz++;
  BehindScreenOffenseSkill::skillNum = numSkillz++;
  StiffarmOffenseSkill::skillNum = numSkillz++;
  AcquirePossessionSkill::skillNum = numSkillz++;
  SpecialOpBlockSkill::skillNum = numSkillz++;
  MidReboundOffenseSkill::skillNum = numSkillz++;
  PullBallOffWallSkill::skillNum = numSkillz++;
  SimpleDefenseSkill::skillNum = numSkillz++;
  CutGoalSkill::skillNum = numSkillz++;
  TandemBlockerSkill::skillNum = numSkillz++;
  TightDefenseDefenderSkill::skillNum = numSkillz++;
  ReceivePassSkill::skillNum = numSkillz++;
  CrossCreaseCreatorSkill::skillNum = numSkillz++;
  RemoveBallSetupSkill::skillNum = numSkillz++;
  RemoveBallHelpSkill::skillNum = numSkillz++;
  FacilatePassSkill::skillNum = numSkillz++;
  CreatorLookForwardPassSkill::skillNum = numSkillz++;
  JamAndShootSkill::skillNum = numSkillz++;
  BlockerGainPossessionSkill::skillNum = numSkillz++;   
  CageDefenseSkill::skillNum = numSkillz++;
  LookForChipKickSkill::skillNum = numSkillz++;
  CrossCreasePassSkill::skillNum = numSkillz++;
  OppositeReboundOffenseSkill::skillNum = numSkillz++;  
  AdvanceBallUpfieldSkill::skillNum = numSkillz++;
  ReboundOffenseSkill::skillNum = numSkillz++;
  TouchShootSkill::skillNum = numSkillz++;
  RemoveBallSkill::skillNum = numSkillz++;
  RemoveBallScreenSkill::skillNum = numSkillz++;
  DribblePickScoreSkill::skillNum = numSkillz++;
  DoublePickBlockSkill::skillNum = numSkillz++;
  PassToCreatorSkill::skillNum = numSkillz++;
  CreatorCreateSkill::skillNum = numSkillz++;
  FollowDefenderSkill::skillNum = numSkillz++;
  OppositeCoverSkill::skillNum = numSkillz++;
  SmartRotateAroundBallSkill::skillNum = numSkillz++;
  AggressivePullBallOffWallSkill::skillNum = numSkillz++;
  AggressivePullBallOffCornerSkill::skillNum = numSkillz++;
  AggressiveJamAndShootSkill::skillNum = numSkillz++;
  StupidJamAndShootSkill::skillNum = numSkillz++;
  CarefulStripBallSkill::skillNum = numSkillz++;
  PenaltySpinSkill::skillNum = numSkillz++;
  StrafeSkill::skillNum = numSkillz++;
}

///the number of skillz registered
int numSkillz;
