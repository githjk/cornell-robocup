#include "RegisterPlays.h"
#include "PlayIncludes.h"
#include "playbook.h"

///////////////////////////////////////////////////////////////////////////
/// To add a play to the system:                                        ///
/// - add include to playincludes.h                                     ///
/// - add playNum initialization to registerplays() (in register.cpp)   ///
/// - add constructor to playbook.cpp                                   ///
///////////////////////////////////////////////////////////////////////////

void registerPlays()
{
  numPlays=0;

  INVALID_PLAY = -1;
  NoPlay::playNum=numPlays++;//sets all destinations to current positions.
  //=========================================
  //2002 PLAYS
  //=========================================
  START_2002_PLAYS=numPlays++;
  SingleAssistOffense::playNum=numPlays++;
  ThreeManDefense::playNum=numPlays++;
  UpfieldDefense::playNum=numPlays++;
  LooseDefense::playNum=numPlays++;
  TightDefense::playNum=numPlays++;
  CageDefense::playNum=numPlays++;
  RemoveBallDefense::playNum=numPlays++;
  DoublePickOffense::playNum=numPlays++;
  CreatorAssistOffense::playNum=numPlays++;
  CrossCreasePass::playNum=numPlays++;
  CreatorPass::playNum=numPlays++;
  TouchShoot::playNum=numPlays++;
  JamAndShoot::playNum=numPlays++;
  BackPass::playNum=numPlays++;
  PuntKick::playNum=numPlays++;
  KickUpfield::playNum=numPlays++;
  PongDefense::playNum=numPlays++;
  END_2002_PLAYS=numPlays++;
  //=========================================
  //2002 PENALTY PLAYS
  //=========================================
  START_2002_PENALTY_PLAYS=numPlays++;
  FreeKick::playNum=numPlays++;
  PenaltyShotUsPivot2002::playNum=numPlays++;
  PenaltyShotUsOpenLoop2002::playNum=numPlays++;
  PenaltyShotUsSideStep2002::playNum=numPlays++;
  PenaltyShotThemRush2002::playNum=numPlays++;
  PenaltyShotThemSmartRush2002::playNum=numPlays++;
  PenaltyShotThemDiveLeft2002::playNum=numPlays++;
  PenaltyShotThemDiveRight2002::playNum=numPlays++;
  END_2002_PENALTY_PLAYS=numPlays++;
  //=========================================
  //FORMATION PLAYS
  //=========================================
  START_FORMATION_PLAYS=numPlays++;
  FormationTester::playNum=numPlays++;
  WallFormation::playNum=numPlays++;
  KickoffUs::playNum=numPlays++;
  KickoffThem::playNum=numPlays++;
  KickFormation::playNum=numPlays++;
  DribbleFormation::playNum=numPlays++;
  PenaltyShotUs::playNum=numPlays++;
  PenaltyShotThem::playNum=numPlays++;
  END_FORMATION_PLAYS=numPlays++;
  //=========================================
  //SKILL TEST PLAYS
  //=========================================
  //Evan Kuhn's Skillz
  START_EVAN_KUHN_SKILL_TEST_PLAYS=numPlays++;
  TestSkillPlay::playNum=numPlays++;
  MoveToBallPlay::playNum=numPlays++;
  PickSkillPlay::playNum=numPlays++;
  MoveAroundPickSkillPlay::playNum=numPlays++;
  LooseDSpecOpTest::playNum=numPlays++;
  TightDefenseBlockerSkillTest::playNum=numPlays++;
  SpecialOpCageDefenseSkillTest::playNum=numPlays++;
  END_EVAN_KUHN_SKILL_TEST_PLAYS=numPlays++;
  //----
  //Evan Malone's Skillz
  START_EVAN_MALONE_SKILL_TEST_PLAYS=numPlays++;
  PullBallOffWallSkillTest::playNum=numPlays++;
  END_EVAN_MALONE_SKILL_TEST_PLAYS=numPlays++;
  //----
  //Joel Chestnutt's Skillz
  START_JOEL_SKILL_TEST_PLAYS=numPlays++;
  BlockerGainPossessionSkillTest::playNum=numPlays++;
  SpinAroundBallSkillTest::playNum=numPlays++;
  ManeuverWithBallSkillTest::playNum=numPlays++;
  InterceptBallSkillTest::playNum=numPlays++;
  TurnAndKickSkillTest::playNum=numPlays++;
  TightDefenseSpecialOpDSkillTest::playNum=numPlays++;
  END_JOEL_SKILL_TEST_PLAYS=numPlays++;
  //----
  //Nirav Shah's Skillz
  START_NIRAV_SKILL_TEST_PLAYS=numPlays++;
  SimpleDefenseSkillTest::playNum=numPlays++;
  CutGoalSkillTest::playNum=numPlays++;
  LooseDefenseSkillTest::playNum=numPlays++;
  TightDefenseDefenderSkillTest::playNum=numPlays++;
  ReceivePassSkillTest::playNum=numPlays++;
  END_NIRAV_SKILL_TEST_PLAYS=numPlays++;
  //----
  //Will's Skillz
  START_WILL_SKILL_TEST_PLAYS=numPlays++;
  SimpleScreenSkillTest::playNum=numPlays++;
  JukeAroundOpponentSkillTest::playNum=numPlays++;
  PullBallOffCornerSkillTest::playNum=numPlays++;
  DribblePointSkillTest::playNum=numPlays++;
  END_WILL_SKILL_TEST_PLAYS=numPlays++;
  //----
  //Unassigned Skill Plays
  CreatorLookForwardPassSkillTest::playNum=numPlays++;
  JamAndShootSkillTest::playNum=numPlays++;
  AcquirePossessionSkillTest::playNum=numPlays++;
  DefenderCageDefenseSkillTest::playNum=numPlays++;
  AggressorCageDefenseSkillTest::playNum=numPlays++;
  LookForChipKickSkillTest::playNum=numPlays++;
  CrossCreasePassSkillTest::playNum=numPlays++;
  OppositeReboundOffenseSkillTest::playNum=numPlays++;
  MidReboundOffenseSkillTest::playNum=numPlays++;
  AdvanceBallUpfieldSkillTest::playNum=numPlays++;
  StiffarmOffenseSkillTest::playNum=numPlays++;
  //=========================================
  //INDIVIDUAL PLAYS
  //=========================================
  START_INDIVIDUAL_PLAYS=numPlays++;
  WillPlay::playNum=numPlays++;
  JoelPlay::playNum=numPlays++;
  NiravPlay::playNum=numPlays++;
  TamasPlay::playNum=numPlays++;
  HollywoodPlay::playNum=numPlays++;
  END_INDIVIDUAL_PLAYS=numPlays++;
  //=========================================
  //INTIMIDATION PLAYS
  //=========================================
  START_INTIMIDATION_PLAYS=numPlays++;
  RadarPlay::playNum=numPlays++;
  ConesPlay::playNum=numPlays++;
  FigureEightPlay::playNum=numPlays++;
  EllipsesPlay::playNum=numPlays++;
  FiveManStar::playNum=numPlays++;
  RunLaps::playNum=numPlays++;
  END_INTIMIDATION_PLAYS=numPlays++;
  //=========================================
  //OTHER PLAYS
  //=========================================
  START_OTHER_PLAYS=numPlays++;
  TimingTest::playNum=numPlays++;
  VisionTesterPlay::playNum=numPlays++;
  DribbleAngleTest::playNum=numPlays++;
  LED_LatencyTest::playNum=numPlays++;
  AngularLatencyTest::playNum=numPlays++;
  RandomPlay::playNum=numPlays++;
	PredictionTestPlay::playNum=numPlays++;
	TortureTest::playNum=numPlays++;
  SimpleKickTest::playNum=numPlays++;
  SimpleDribbleTest::playNum=numPlays++;
  SnowflakeTester::playNum=numPlays++;
  KickingChallengePlay::playNum=numPlays++;
  PassingChallengePlay::playNum=numPlays++;
  END_OTHER_PLAYS=numPlays++;
  //=========================================
  NUM_PLAYS=numPlays++;
}

///the number of plays registered
int numPlays;