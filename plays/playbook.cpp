/*
*   playbook.cpp
*/
#include "playbook.h"
#include "PlayIncludes.h"

///////////////////////////////////////////////////////////////////////////
/// To add a play to the system:                                        ///
/// - add include to playincludes.h                                     ///
/// - add playNum initialization to registerplays() (in register.cpp)   ///
/// - add constructor to playbook.cpp                                   ///
///////////////////////////////////////////////////////////////////////////

//constructor
Playbook::Playbook(VisionModule* vision) 
{
  playbook=new BasePlay * [NUM_PLAYS];

  for (int i = 0; i < NUM_PLAYS; i++)
  {
    playbook[i] = NULL;
  }

  playbook[NoPlay::playNum] = new NoPlay(vision);

  //##################
  //### 2002 Plays ###
  //##################
  playbook[RemoveBallDefense::playNum] = new RemoveBallDefense(vision);
  playbook[CageDefense::playNum] = new CageDefense(vision);
  playbook[TightDefense::playNum] = new TightDefense(vision);
  playbook[LooseDefense::playNum] = new LooseDefense(vision);
  playbook[UpfieldDefense::playNum] = new UpfieldDefense(vision);
  playbook[SingleAssistOffense::playNum] = new SingleAssistOffense(vision);
  playbook[DoublePickOffense::playNum] = new DoublePickOffense(vision);
  playbook[CreatorAssistOffense::playNum] = new CreatorAssistOffense(vision);
  playbook[CrossCreasePass::playNum] = new CrossCreasePass(vision);
  playbook[TouchShoot::playNum] = new TouchShoot(vision);
  playbook[JamAndShoot::playNum] = new JamAndShoot(vision);
  playbook[ThreeManDefense::playNum] = new ThreeManDefense(vision);
  playbook[CreatorPass::playNum] = new CreatorPass(vision);
  playbook[BackPass::playNum] = new BackPass(vision);
  playbook[PuntKick::playNum] = new PuntKick(vision);
  playbook[KickUpfield::playNum] = new KickUpfield(vision);
  playbook[PongDefense::playNum] = new PongDefense(vision);
  
  //##########################
  //### 2002 Penalty Plays ###
  //##########################
  playbook[FreeKick::playNum] = new FreeKick(vision);
  playbook[PenaltyShotUsPivot2002::playNum] = new PenaltyShotUsPivot2002(vision);
  playbook[PenaltyShotUsOpenLoop2002::playNum] = new PenaltyShotUsOpenLoop2002(vision);
  playbook[PenaltyShotUsSideStep2002::playNum] = new PenaltyShotUsSideStep2002(vision);
  playbook[PenaltyShotThemRush2002::playNum] = new PenaltyShotThemRush2002(vision);
  playbook[PenaltyShotThemSmartRush2002::playNum] = new PenaltyShotThemSmartRush2002(vision);
  playbook[PenaltyShotThemDiveLeft2002::playNum] = new PenaltyShotThemDiveLeft2002(vision);
  playbook[PenaltyShotThemDiveRight2002::playNum] = new PenaltyShotThemDiveRight2002(vision);
  
  //######################
  //#### Skill Plays #####
  //######################

  //Will's Skills
  playbook[SimpleScreenSkillTest::playNum] = new SimpleScreenSkillTest(vision);
  playbook[JukeAroundOpponentSkillTest::playNum] = new JukeAroundOpponentSkillTest(vision);
  playbook[PullBallOffCornerSkillTest::playNum] = new PullBallOffCornerSkillTest(vision);
  playbook[DribblePointSkillTest::playNum] = new DribblePointSkillTest(vision);

  //Joel's Skills
  playbook[SpinAroundBallSkillTest::playNum] = new SpinAroundBallSkillTest(vision);
  playbook[ManeuverWithBallSkillTest::playNum] = new ManeuverWithBallSkillTest(vision);
  playbook[InterceptBallSkillTest::playNum] = new InterceptBallSkillTest(vision);
  playbook[TurnAndKickSkillTest::playNum] = new TurnAndKickSkillTest(vision);
  playbook[TightDefenseSpecialOpDSkillTest::playNum] = new TightDefenseSpecialOpDSkillTest(vision);

  //Evan Kuhn's Skills
  playbook[TestSkillPlay::playNum] = new TestSkillPlay(vision);
  playbook[MoveToBallPlay::playNum] = new MoveToBallPlay(vision);
  playbook[PickSkillPlay::playNum] = new PickSkillPlay(vision);
  playbook[MoveAroundPickSkillPlay::playNum] = new MoveAroundPickSkillPlay(vision);
  playbook[LooseDSpecOpTest::playNum] = new LooseDSpecOpTest(vision);
  playbook[TightDefenseBlockerSkillTest::playNum] = new TightDefenseBlockerSkillTest(vision);
  playbook[SpecialOpCageDefenseSkillTest::playNum] = new SpecialOpCageDefenseSkillTest(vision);

  //Evan Malone's Skills
  playbook[PullBallOffWallSkillTest::playNum] = new PullBallOffWallSkillTest(vision);
  
  //Nirav Shah's Skills
  playbook[SimpleDefenseSkillTest::playNum] = new SimpleDefenseSkillTest(vision);
  playbook[CutGoalSkillTest::playNum] = new CutGoalSkillTest(vision);
  playbook[LooseDefenseSkillTest::playNum] = new LooseDefenseSkillTest(vision);
  playbook[TightDefenseDefenderSkillTest::playNum] = new TightDefenseDefenderSkillTest(vision);
  playbook[ReceivePassSkillTest::playNum] = new ReceivePassSkillTest(vision);

  //Not assigned yet...
  playbook[CreatorLookForwardPassSkillTest::playNum] = new CreatorLookForwardPassSkillTest(vision);
  playbook[JamAndShootSkillTest::playNum] = new JamAndShootSkillTest(vision);
  playbook[BlockerGainPossessionSkillTest::playNum] = new BlockerGainPossessionSkillTest(vision);
  playbook[AcquirePossessionSkillTest::playNum] = new AcquirePossessionSkillTest(vision);
  playbook[DefenderCageDefenseSkillTest::playNum] = new DefenderCageDefenseSkillTest(vision);
  playbook[AggressorCageDefenseSkillTest::playNum] = new AggressorCageDefenseSkillTest(vision);
  playbook[SpecialOpCageDefenseSkillTest::playNum] = new SpecialOpCageDefenseSkillTest(vision);
  playbook[LookForChipKickSkillTest::playNum] = new LookForChipKickSkillTest(vision);
  playbook[CrossCreasePassSkillTest::playNum] = new CrossCreasePassSkillTest(vision);
  playbook[OppositeReboundOffenseSkillTest::playNum] = new OppositeReboundOffenseSkillTest(vision);
  playbook[MidReboundOffenseSkillTest::playNum] = new MidReboundOffenseSkillTest(vision);
  playbook[AdvanceBallUpfieldSkillTest::playNum] = new AdvanceBallUpfieldSkillTest(vision);
  playbook[StiffarmOffenseSkillTest::playNum] = new StiffarmOffenseSkillTest(vision);

  //############################
  //##### Individual Plays #####
  //############################
  playbook[TamasPlay::playNum] = new TamasPlay(vision);
  playbook[HollywoodPlay::playNum] = new HollywoodPlay(vision);
  playbook[JoelPlay::playNum] = new JoelPlay(vision);
  playbook[NiravPlay::playNum] = new NiravPlay(vision);
  playbook[WillPlay::playNum] = new WillPlay(vision);

  //##############################
  //##### Intimidation Plays #####
  //##############################
  playbook[RunLaps::playNum] = new RunLaps(vision);
  playbook[FiveManStar::playNum] = new FiveManStar(vision);
  playbook[RadarPlay::playNum] = new RadarPlay(vision);
  playbook[ConesPlay::playNum] = new ConesPlay(vision);
  playbook[EllipsesPlay::playNum] = new EllipsesPlay(vision);
  playbook[FigureEightPlay::playNum] = new FigureEightPlay(vision);

  //#######################
  //##### Other Plays #####
  //#######################
  playbook[RandomPlay::playNum] = new RandomPlay(vision);
  playbook[TimingTest::playNum] = new TimingTest(vision);
  playbook[LED_LatencyTest::playNum] = new LED_LatencyTest(vision);
  playbook[AngularLatencyTest::playNum] = new AngularLatencyTest(vision);
  playbook[VisionTesterPlay::playNum] = new VisionTesterPlay(vision);
  playbook[DribbleAngleTest::playNum] = new DribbleAngleTest(vision);
  playbook[PredictionTestPlay::playNum] = new PredictionTestPlay(vision);
	playbook[TortureTest::playNum] = new TortureTest(vision);
  playbook[SimpleKickTest::playNum] = new SimpleKickTest(vision);
  playbook[SimpleDribbleTest::playNum] = new SimpleDribbleTest(vision);
  playbook[SnowflakeTester::playNum] = new SnowflakeTester(vision);
  playbook[KickingChallengePlay::playNum] = new KickingChallengePlay(vision);
  playbook[PassingChallengePlay::playNum] = new PassingChallengePlay(vision);

  //###########################
  //##### Formation Plays #####
  //###########################
  playbook[FormationTester::playNum] = new FormationTester(vision);
  playbook[WallFormation::playNum] = new WallFormation(vision);
  playbook[KickFormation::playNum] = new KickFormation(vision);
  playbook[DribbleFormation::playNum] = new DribbleFormation(vision);
  playbook[KickoffUs::playNum] = new KickoffUs(vision);
  playbook[KickoffThem::playNum] = new KickoffThem(vision);
  playbook[PenaltyShotUs::playNum] = new PenaltyShotUs(vision);
  playbook[PenaltyShotThem::playNum] = new PenaltyShotThem(vision);
}

//buffer should be a large char[].
void Playbook::enumPlayNames(char* buffer) const
{
  buffer[0] = NULL;
  char temp[80];
  Play play = NoPlay::playNum;
  play++; //don't include NO_PLAY

  for(; play < NUM_PLAYS; play++)
  { 
    if(playbook[play] != NULL)
    {
      strcpy(temp,  playbook[play]->getName());
      int i = 0;

      //replace ' ' with '_' in the play names:
      for (; (i < 79) && temp[i] != 0; i++)
      {
        if(temp[i] == ' ')
          temp[i] = '_';
      }

      //add a space to the end:
      temp[i] = ' ';
      temp[i+1] = 0;
    }
    else
    {
      strcpy(temp, "play_not_implemented ");
    }

    strncat(buffer, temp, 79);
  }



}

      Play  INVALID_PLAY = -1,
      //These ranges are used by the GUI to iterate 
      //over various groupings of play names
      START_2002_PLAYS, 
      END_2002_PLAYS,

      START_2002_PENALTY_PLAYS,
      END_2002_PENALTY_PLAYS,

      START_FORMATION_PLAYS, 
      END_FORMATION_PLAYS,  

      START_EVAN_KUHN_SKILL_TEST_PLAYS,
      END_EVAN_KUHN_SKILL_TEST_PLAYS,

      START_EVAN_MALONE_SKILL_TEST_PLAYS,
      END_EVAN_MALONE_SKILL_TEST_PLAYS,

      START_JOEL_SKILL_TEST_PLAYS,
      END_JOEL_SKILL_TEST_PLAYS,

      START_NIRAV_SKILL_TEST_PLAYS,
      END_NIRAV_SKILL_TEST_PLAYS,

      START_WILL_SKILL_TEST_PLAYS,
      END_WILL_SKILL_TEST_PLAYS,

      START_INDIVIDUAL_PLAYS, 
      END_INDIVIDUAL_PLAYS,

      START_INTIMIDATION_PLAYS, 
      END_INTIMIDATION_PLAYS,

      START_OTHER_PLAYS, 
      END_OTHER_PLAYS,

      NUM_PLAYS;
