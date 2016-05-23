#ifndef _PLAY_INCLUDES_H
#define _PLAY_INCLUDES_H

#include "noplay.h"
//////////////////////////////////////////////////////////////////////////////
/// To add a play to the system:                                           ///
/// - add include to playincludes.h                                        ///
/// - add playNum initialization to registerplays() (in registerplays.cpp) ///
/// - add constructor to playbook.cpp                                      ///
//////////////////////////////////////////////////////////////////////////////

//######################
//##### 2002 Plays #####
//######################
#include "2002plays/removeBallDefense.h"
#include "2002plays/cageDefense.h"
#include "2002plays/tightDefense.h"
#include "2002plays/looseDefense.h"
#include "2002plays/upfieldDefense.h"
#include "2002plays/singleAssistOffense.h"
#include "2002plays/doublePickOffense.h"
#include "2002plays/creatorAssistOffense.h"
#include "2002plays/crossCreasePass.h"
#include "2002plays/touchShoot.h"
#include "2002plays/jamAndShoot.h"
#include "2002plays/threeManDefense.h"
#include "2002plays/CreatorPass.h"
#include "2002plays/BackPass.h"
#include "2002plays/PuntKick.h"
#include "2002plays/kickUpfield.h"
#include "2002plays/pongDefense.h"

//##############################
//##### 2002 Penalty Plays #####
//##############################
#include "2002plays/freeKick.h"
#include "2002plays/penaltyShotUsPivot2002.h"
#include "2002plays/penaltyShotUsOpenLoop2002.h"
#include "2002plays/penaltyShotUsSideStep2002.h"
#include "2002plays/penaltyShotThemRush2002.h"
#include "2002plays/penaltyShotThemSmartRush2002.h"
#include "2002plays/penaltyShotThemDiveLeft2002.h"
#include "2002plays/penaltyShotThemDiveRight2002.h"



//######################
//#### Skill Plays #####
//######################
//Will's Skillz
#include "skillPlays/SimpleScreenSkill_Test.h"
#include "skillPlays/JukeAroundOpponentSkill_Test.h"
#include "skillPlays/PullBallOffCornerSkill_Test.h"
#include "skillPlays/DribblePointSkill_Test.h"

//Joel's Skills
#include "skillPlays/SpinAroundBallSkill_Test.h"
#include "skillPlays/ManeuverWithBallSkill_Test.h"
#include "skillPlays/TurnAndKickSkill_Test.h"
#include "skillPlays/InterceptBallSkill_Test.h"
#include "skillPlays/TightDefenseSpecialOpDSkill_Test.h"

//Evan Kuhn's Skills
#include "skillPlays/TestSkill_Test.h"
#include "skillPlays/MoveToBall_Test.h"
#include "skillPlays/PickSkill_Test.h"
#include "skillPlays/MoveAroundPickSkill_Test.h"
#include "skillPlays/LooseDSpecOpSkill_Test.h"
#include "skillPlays/TightDefenseBlockerSkill_Test.h"
#include "skillPlays/SpecialOpCageDefenseSkill_Test.h"

//Evan Malone's Skills
#include "skillPlays/PullBallOffWallSkill_Test.h"

//Nirav Shah's Skills
#include "skillPlays/SimpleDefenseSkill_Test.h"
#include "skillPlays/CutGoalSkill_Test.h"
#include "skillPlays/LooseDefenseSkill_Test.h"
#include "skillPlays/TightDefenseDefenderSkill_Test.h"
#include "skillPlays/ReceivePassSkill_Test.h"
//unassigned
#include "skillPlays/CreatorLookForwardPassSkill_Test.h"
#include "skillPlays/JamAndShootSkill_Test.h"
#include "skillPlays/BlockerGainPossessionSkill_Test.h"
#include "skillPlays/AcquirePossessionSkill_Test.h"
#include "skillPlays/DefenderCageDefenseSkill_Test.h"
#include "skillPlays/AggressorCageDefenseSkill_Test.h"
#include "skillPlays/SpecialOpCageDefenseSkill_Test.h"
#include "skillPlays/LookForChipKickSkill_Test.h"
#include "skillPlays/CrossCreasePassSkill_Test.h"
#include "skillPlays/OppositeReboundOffenseSkill_Test.h"
#include "skillPlays/MidReboundOffenseSkill_Test.h"
#include "skillPlays/AdvanceBallUpfieldSkill_Test.h"
#include "skillPlays/StiffarmOffenseSkill_Test.h"


//############################
//##### Individual Plays #####
//############################
#include "individualPlays/tamas_play.h"
#include "individualPlays/hollywood_play.h"
#include "individualPlays/joel_play.h"
#include "individualPlays/nirav_play.h"
#include "individualPlays/will_play.h"

//##############################
//##### Intimidation Plays #####
//##############################
#include "intimidationPlays/five_man_star.h"
#include "intimidationPlays/radar_play.h"
#include "intimidationPlays/cones_play.h"
#include "intimidationPlays/ellipses.h"
#include "intimidationPlays/figure_eight.h"
#include "intimidationPlays/runlaps.h"

//#######################
//##### Other Plays #####
//#######################
#include "otherPlays/LED_latency_test.h"
#include "otherPlays/angular_latency_test.h"
#include "otherPlays/random_play.h"
#include "otherPlays/timing_test.h"
#include "otherPlays/visionTester.h"
#include "otherPlays/dribbleAngle.h"
#include "otherPlays/prediction_test_play.h"
#include "otherPlays/torturetest.h"
#include "otherPlays/SimpleKick_Test.h"
#include "otherPlays/SimpleDribble_Test.h"
#include "otherPlays/SnowflakeTester.h"
#include "otherPlays/KickingChallengePlay.h"
#include "otherPlays/PassingChallengePlay.h"

//###########################
//##### Formation Plays #####
//###########################
#include "formations/wall_formation.h"
#include "formations/kick_formation.h"
#include "formations/dribble_formation.h"
#include "formations/kickoff_us.h"
#include "formations/kickoff_them.h"
#include "formations/penalty_shot_us.h"
#include "formations/penalty_shot_them.h"
#include "formations/formation_tester.h"

#endif //_PLAY_INCLUDES_H