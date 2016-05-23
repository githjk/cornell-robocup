//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Blocker::penaltyShotThemDiveRight2002 (RobotIndex ID,
                           BasePlay* play,
                           const VisionData& field,
                           RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "BLOCKER pstD RIGHT");   

  //Set Robot Velocity
  Destination *command = sd->getDestination(ID);
  command->setPos(sd->getSystemParams().field.OUR_GOAL_LINE + 
                  sd->getSystemParams().general.PLAYER_RADIUS,
                  sd->getSystemParams().field.RIGHT_GOAL_POST);
  command->setRotation(0.0f);
  command->setControl(OMNI_GOALIE);
  command->setSpeed(GOALIE_SPEED);
}
//----------------------------------------------------------------------------