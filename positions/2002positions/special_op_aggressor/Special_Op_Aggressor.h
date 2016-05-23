#ifndef SPECIAL_OP_AGGRESSOR_H
#define SPECIAL_OP_AGGRESSOR_H
/***********************************************************************************/
//	HEADER FILES
#include "plays/plays.h"
#include "robocup_strategy_data.h"
/***********************************************************************************/
//Basic rule of thumb, each play gets it's own action function!!! NOOO exceptions.
//If many plays have position doing same thing, make special action function and
//have the various action functions call that action function. Make debugging much easier.
namespace Special_Op_Aggressor 
{
  //=================================================================
  //Play Action Functions
  //-----------------------------------------------------------------
  void dummyActionFunction(RobotIndex ID, 
                           BasePlay* play, 
                           const VisionData& field, 
                           RobocupStrategyData* sd);
  //-----------------------------------------------------------------
}
/***********************************************************************************/
#endif	// SPECIAL_OP_AGGRESSOR_H



