//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/pair/pair.h"
/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Blocker::penaltyShotThemRush2002(RobotIndex ID,
                                      BasePlay* play,
                                      const VisionData& field,
                                      RobocupStrategyData* sd)
{
  //Display Message
  sd->setMessage(ID, "BLOCKER pstRUSH");   

  //Set Robot Velocity
  Destination *command = sd->getDestination(ID);

  //Find opponent who has the ball
  RobotIndex opponentID = NO_ROBOT;
  float minDist = -1;

  Pair ballLoc = sd->getDefensiveBallLocation();

  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    if(
        theirRobotFound(i, field, sd->getSystemParams()) &&
        (
          opponentID == NO_ROBOT ||
          getLocation(sd->getSystemParams().general.OTHER_TEAM,
                      i, 
                      field).distanceTo(ballLoc) < minDist
        )
      )
    {
      minDist = getLocation(sd->getSystemParams().general.OTHER_TEAM,
                            i, 
                            field).distanceTo(ballLoc);
      opponentID = i;
    }

  }

  Pair dest;

  //Draw vector from opponent through ball, and extend by distance to dribbler
  if(opponentID == NO_ROBOT)
  {
    dest.set(ballLoc);
  }
  else
  {
    dest.set(getLocation(sd->getSystemParams().general.OTHER_TEAM,
                         opponentID,
                         field));
    extendPoint(dest, ballLoc, sd->getSystemParams().general.DIST_TO_DRIBBLER, dest);
  }

  //Go to this point, using the current robot rotation to make it as fast as possible
  command->setRotation(0.0f);
  command->setPos(dest);
  command->setDribble(DRIBBLE_DEFAULT);
  command->setVerticalDribble(V_DRIBBLE_DEFAULT);
  command->setControl(OMNI_NORMAL_ENTERBOX);
  command->setSpeed(GOALIE_SPEED);
}
//----------------------------------------------------------------------------