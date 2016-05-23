/***********************************************************************************/
//	HEADER FILES
#include "Defender.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Defender::puntKick(RobotIndex ID,
                        BasePlay* play,
                        const VisionData& field,
                        RobocupStrategyData* sd)
{
  //===================
  //===================
  //Handle the Defender
  //===================
  //===================

  //-------------------
  //set default message
  //-------------------
  sd->setMessage(ID, "DEFENDER Punt Kick");
}
//----------------------------------------------------------------------------