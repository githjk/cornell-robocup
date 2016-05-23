/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Blocker::puntKick(RobotIndex ID,
                       BasePlay* play,
                       const VisionData& field,
                       RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "BLOCKER Punt Kick");
}
//----------------------------------------------------------------------------