/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::puntKick(RobotIndex ID,
                         BasePlay* play,
                         const VisionData& field,
                         RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "AGGRESSOR Punt Kick");
}
//----------------------------------------------------------------------------