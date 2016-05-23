#include "playTransitionIncludes.h"
#include "playTransition.h"

//======================================================
PlayTransition::PlayTransition(VisionModule* vision) 
{
  //create array of transitions
  transitionBook=new Transition * [NUM_PLAYS];
  for (int i = 0; i < NUM_PLAYS; i++)
  {
    transitionBook[i] = NULL;
  }

  //2002 play transitions
  transitionBook[CageDefense::playNum] = new CageDefenseTransition(vision);
  transitionBook[TightDefense::playNum] = new TightDefenseTransition(vision);
  transitionBook[LooseDefense::playNum] = new LooseDefenseTransition(vision);
  transitionBook[UpfieldDefense::playNum] = new UpfieldDefenseTransition(vision);
  transitionBook[SingleAssistOffense::playNum] = new SingleAssistOffenseTransition(vision);
  transitionBook[CrossCreasePass::playNum] = new CrossCreasePassTransition(vision);
  transitionBook[CreatorPass::playNum] = new CreatorPassTransition(vision);
  transitionBook[CreatorAssistOffense::playNum] = new CreatorAssistOffenseTransition(vision);
  transitionBook[TouchShoot::playNum] = new TouchShootTransition(vision);
  transitionBook[JamAndShoot::playNum] = new JamAndShootTransition(vision);
  transitionBook[DoublePickOffense::playNum] = new DoublePickOffenseTransition(vision); 
  transitionBook[RemoveBallDefense::playNum] = new RemoveBallDefenseTransition(vision);
  transitionBook[KickUpfield::playNum] = new KickUpfieldTransition(vision);
  transitionBook[ThreeManDefense::playNum] = new ThreeManDefenseTransition(vision);
  transitionBook[BackPass::playNum] = new BackPassTransition(vision);
  transitionBook[PuntKick::playNum] = new PuntKickTransition(vision);
  
  //2002 penalty play transitions
  transitionBook[FreeKick::playNum] = new FreeKickTransition(vision);
  transitionBook[PenaltyShotUsPivot2002::playNum] = new PenaltyShotUsPivot2002Transition(vision);
  transitionBook[PenaltyShotUsOpenLoop2002::playNum] = new PenaltyShotUsOpenLoop2002Transition(vision);
  transitionBook[PenaltyShotUsSideStep2002::playNum] = new PenaltyShotUsSideStep2002Transition(vision);
  transitionBook[PenaltyShotThemRush2002::playNum] = new PenaltyShotThemRush2002Transition(vision);
  transitionBook[PenaltyShotThemSmartRush2002::playNum] = new PenaltyShotThemSmartRush2002Transition(vision);
  transitionBook[PenaltyShotThemDiveLeft2002::playNum] = new PenaltyShotThemDiveLeft2002Transition(vision);
  transitionBook[PenaltyShotThemDiveRight2002::playNum] = new PenaltyShotThemDiveRight2002Transition(vision);
}
//======================================================
PlayTransition::~PlayTransition() 
{
  for (int i = 0; i < NUM_PLAYS; i++)
  {
    if (transitionBook[i] != NULL)
    {
       delete transitionBook[i];
    }
  }
  delete [] transitionBook;
}
//======================================================
void PlayTransition::transition(VisionData* vision, RobocupStrategyData* rsd, BasePlay* currentPlay)
{
  //********************************************************************************
  // Call the executePlayTransition function based on the current play
  //********************************************************************************
	if(transitionBook[currentPlay->getID()] != NULL)
	{
		transitionBook[currentPlay->getID()]->executePlayTransition(*vision, rsd, currentPlay);  
	
	}
}
//======================================================
void PlayTransition::loadTransitionParams(){
  for (int i = 0; i < NUM_PLAYS; i++)
  {
    if (transitionBook[i] != NULL)
    {
       transitionBook[i]->loadValues();
    }
  }
}
