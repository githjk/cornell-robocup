/***********************************************************************************/
//	HEADER FILES
#include "Creator.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Creator::puntKick(RobotIndex ID,
                       BasePlay* play,
                       const VisionData& field,
                       RobocupStrategyData* sd)
{
  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "CREATOR Punt Kick");
}
//----------------------------------------------------------------------------