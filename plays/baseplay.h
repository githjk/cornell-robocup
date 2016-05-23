class BasePlay;

#ifndef BASEPLAY_H
#define BASEPLAY_H

#include "robocup_strategy_data.h"
#include "plays.h"
#include "hires_timer.h"

#include "positions/2002positions/creator/Creator.h"
#include "positions/2002positions/defender/Defender.h"
#include "positions/2002positions/blocker/Blocker.h"
#include "positions/2002positions/aggressor/Aggressor.h"
#include "positions/2002positions/special_op_creator/Special_Op_Creator.h"
#include "positions/2002positions/special_op_aggressor/Special_Op_Aggressor.h"
#include "positions/2002positions/special_op_defender/Special_Op_Defender.h"

typedef enum
{
  NORMAL_PLAY,
  PASS_TO_CREATOR,
  PASS_TO_SPECIAL_OP,
  GUARD_BALL,
  DUMPING_TO_CORNER
} PlayState;



class BasePlay : public ParameterOperations
{
public:
  BasePlay(VisionModule *vm) { timer = new VisionTimer(vm);}

  virtual ~BasePlay() {delete timer;}

  //These functions must be virual or inheritance will not work.

  //making them pure virtual (with the "=0") ensures we will notice if a subclass

  // forgets to define them.



  virtual void ReloadParameters(SystemParameters* P){};



  virtual Play getID() const = 0;

  virtual char* getName()  const= 0;



  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd) = 0;

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd)

  {

	  standardExecution(*vision, rsd);

  }



  virtual ActionFunction getActionFunction(Position pos) = 0;



  // Most plays won't use this, but some (eg latency tests) need to clean up the parameters object

  // when they end.  Therefore, this should be called rught before switching the play.

  virtual void endPlay(RobocupStrategyData* rsd) {}
  
  virtual void loadValues() {}


  inline void startTimer(void) { timer->resetTimer(); }

  inline double readTimer(void) {return timer->currentTime(); } 


  inline PlayState getPlayState(void) {return playState;}

  inline void setPlayState(PlayState state){playState=state;}



protected:



  //Most implementations of executePlay just call this.

  void standardExecution(const VisionData& vision, RobocupStrategyData* rsd);



  PlayState playState;



  VisionTimer *timer;



};



#endif //BASEPLAY_H

