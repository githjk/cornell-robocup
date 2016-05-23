/***********************************************************************************/
//	HEADER FILES
#include "Special_Op_Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
/***********************************************************************************/
//----------------------------------------------------------------------------
void Special_Op_Aggressor::dummyActionFunction(RobotIndex ID,
                                             BasePlay* play,
                                             const VisionData& field,
                                             RobocupStrategyData* sd)
{
  /*
  //Special Op players do not set their desinations. By default every
  //frame their velocities are set to 0.
  //If special op exists, the other appropriate position should utilize it directly.
  sd->setMessage(ID, "SPECIAL_OP_AGGRESSOR dummy function");
  initializeDestination(ID, field, sd->getParams(), sd->getDestination(ID));
  */
}
//----------------------------------------------------------------------------