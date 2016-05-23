#include "Weather.h"
#include "plays/PlayIncludes.h"
//-------------------
//Formations
#include "formation_position_switching/WallFormationStorm.h"
#include "formation_position_switching/ShortestDistanceStorm.h"
//-------------------
//2002 play storms
#include "2002_position_switching/CageDefenseStorm.h"
#include "2002_position_switching/CreatorAssistOffenseStorm.h"
#include "2002_position_switching/CrossCreasePassStorm.h"
#include "2002_position_switching/CreatorPassStorm.h"
#include "2002_position_switching/DoublePickOffenseStorm.h"
#include "2002_position_switching/JamAndShootStorm.h"
#include "2002_position_switching/LooseDefenseStorm.h"
#include "2002_position_switching/RemoveBallDefenseStorm.h"
#include "2002_position_switching/SingleAssistOffenseStorm.h"
#include "2002_position_switching/TightDefenseStorm.h"
#include "2002_position_switching/TouchShootStorm.h"
#include "2002_position_switching/UpfieldDefenseStorm.h"
#include "2002_position_switching/KickUpfieldStorm.h"
#include "2002_position_switching/ThreeManDefenseStorm.h"
#include "2002_position_switching/BackPassStorm.h"
#include "2002_position_switching/PuntKickStorm.h"
//-------------------

//========================================
Weather::Weather(RoboCupStrategyModule* strategy)
{
  this->strategy = strategy;

  //---------------------------------
  //Construct array of storm pointers
  //---------------------------------
  storms = new Storm*[NUM_PLAYS + 1];
  for(int i=0; i<(NUM_PLAYS + 1); i++)
  {
    storms[i] = NULL;
  }

  //---------------------------------
  //Create storm objects that exist for given plays
  //---------------------------------
  //formations
  storms[WallFormation::playNum] = new WallFormationStorm(strategy);
  storms[KickoffUs::playNum] = new ShortestDistanceStorm(strategy);
  storms[KickFormation::playNum] = new ShortestDistanceStorm(strategy);
  storms[DribbleFormation::playNum] = new ShortestDistanceStorm(strategy);
  storms[KickoffThem::playNum] = new ShortestDistanceStorm(strategy);
  storms[PenaltyShotUs::playNum] = new ShortestDistanceStorm(strategy);
  storms[PenaltyShotThem::playNum] = new ShortestDistanceStorm(strategy);
  //2002 plays
  storms[CageDefense::playNum] = new CageDefenseStorm(strategy);
  storms[CreatorAssistOffense::playNum] = new CreatorAssistOffenseStorm(strategy);
  storms[CrossCreasePass::playNum] = new CrossCreasePassStorm(strategy);
  storms[CreatorPass::playNum] = new CreatorPassStorm(strategy);
  storms[DoublePickOffense::playNum] = new DoublePickOffenseStorm(strategy);
  storms[JamAndShoot::playNum] = new JamAndShootStorm(strategy);
  storms[LooseDefense::playNum] = new LooseDefenseStorm(strategy);
  storms[RemoveBallDefense::playNum] = new RemoveBallDefenseStorm(strategy);
  storms[SingleAssistOffense::playNum] = new SingleAssistOffenseStorm(strategy);
  storms[TightDefense::playNum] = new TightDefenseStorm(strategy);
  storms[TouchShoot::playNum] = new TouchShootStorm(strategy);
  storms[UpfieldDefense::playNum] = new UpfieldDefenseStorm(strategy);
  storms[ThreeManDefense::playNum] = new ThreeManDefenseStorm(strategy);
  storms[PuntKick::playNum] = new PuntKickStorm(strategy);
  storms[BackPass::playNum] = new BackPassStorm(strategy);
  storms[KickUpfield::playNum] = new KickUpfieldStorm(strategy);
  storms[PongDefense::playNum] = new ShortestDistanceStorm(strategy);
  //2002 penalty plays
  storms[FreeKick::playNum] = new KickUpfieldStorm(strategy);


}
//========================================
Weather::~Weather()
{
  for(int i=0; i<NUM_PLAYS; i++)
  {
    delete storms[i];
    storms[i] = NULL;
  }
  delete[] storms;
  storms = NULL;
}
//========================================
void Weather::preformSwitching(VisionModule* vm,
                               SystemParameters* params)
{
  //call preform switching on current 
  //play's storm if available
  RobocupStrategyData* sd = strategy->getCurrentRoboCupFrame();
  Play currentPlay = sd->getCurrentPlay();
  if(storms[currentPlay] != NULL)
    storms[currentPlay]->preformSwitching(vm, params);
}
//========================================

void Weather::loadWeatherParams(){
  for(int i=0; i<NUM_PLAYS; i++){
    storms[i]->loadValues();
  }
}
