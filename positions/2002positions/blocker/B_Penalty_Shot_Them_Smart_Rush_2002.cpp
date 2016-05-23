//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Blocker.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/pair/pair.h"
#include "common/util/math/angle.h"
#include "GUI/GUI.h"

/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Blocker::penaltyShotThemSmartRush2002(RobotIndex ID,
                                           BasePlay* play,
                                           const VisionData& field,
                                           RobocupStrategyData* sd)
{

  //sd->getStrategyModule()->getVisionModule()
  //Display Message
  sd->setMessage(ID, "BLOCKER pstRUSH");   

  //Set Robot Velocity
  Destination *command = sd->getDestination(ID);

  RawVisionData* rawField = sd->getStrategyModule().getVisionModule()->getRawFrame();

  Pair opLoc;
  float opponentAngle = getOpponentAngle(field,
                                         *rawField,
                                         sd,
                                         opLoc);
                    
  Pair ballLoc = sd->getStrategyModule().getVisionModule()->getLastUnpredictedBall();
  
  Pair ourLoc = getLocation(ID, field, sd->getSystemParams());

  float thresh = sd->getSystemParams().general.PLAYER_RADIUS +
                 sd->getSystemParams().general.BALL_RADIUS;

  //if the ball is not found just push up by opponent loc;
  if(!rawField->isBallFound(0,0))
  {
    ballLoc.set(opLoc.getX() - sd->getSystemParams().general.OPPONENT_RADIUS,
                opLoc.getY());
  }
  else
  {
    Pair ballLoc(rawField->getBall(0, 0).getPos());
  }

  float leftPostAngle = angleBetween(ballLoc, Pair(sd->getSystemParams().field.OUR_GOAL_LINE,
                                                   sd->getSystemParams().field.LEFT_GOAL_POST));
  float rightPostAngle = angleBetween(ballLoc, Pair(sd->getSystemParams().field.OUR_GOAL_LINE,
                                                   sd->getSystemParams().field.RIGHT_GOAL_POST));

  //if past left post, bound to left post
  if(opponentAngle > 0.0f &&
     opponentAngle < leftPostAngle)
  {
    opponentAngle = leftPostAngle;
  }

  //if past right post, bound to right post
  else if(opponentAngle < 0.0f &&
     opponentAngle > rightPostAngle)
  {
    opponentAngle = rightPostAngle;
  }

  float dx = ballLoc.getX() - ourLoc.getX();
  float dy = ballLoc.getY() - ourLoc.getY();

  float t = -(dx*COS(opponentAngle) + dy*SIN(opponentAngle));

  //opponent got around us go to the ball
  if(t < 0.0f)
  {
    command->setPos(ballLoc);
    command->setControl(OMNI_GOALIE);
    sd->setMessage(ID, "behind us, going to the ball");
  }
  else
  {
    Pair interceptPoint(ballLoc.getX() + t*COS(opponentAngle),
                        ballLoc.getY() + t*SIN(opponentAngle));

    //if the distance from our robot to this point is greater than thresh,
    //go to that point  
    if(ourLoc.distanceTo(interceptPoint) > thresh)
    {
      command->setPos(interceptPoint);
      command->setControl(OMNI_GOALIE);
      sd->setMessage(ID, "going to intercept");
    }
    //else, rush the ball
    else
    { 
      command->setPos(ballLoc);
      command->setControl(OMNI_GOALIE);
      sd->setMessage(ID, "rushing the ball");
    }

    command->setRotation(0.0f);
    command->setSpeed(GOALIE_SPEED);
    command->setControl(OMNI_GOALIE_FAST);
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
  }
}  
//==================================================================
float Blocker::getOpponentAngle(const VisionData& field,
                                const RawVisionData& rawField,
                                RobocupStrategyData* sd,
                                Pair& closestOp)
{
  //if the ball is not found, return 0.0f
  if(!rawField.isBallFound(0,0))
  {
    return PI;
  }

  //else, return the angle between the center of the robot and the ball
  else
  {
    Pair ballLoc(rawField.getBall(0, 0).getPos());

    //find closest opponent to the ball
    float closestOpDistance;
    RobotIndex closestOpID = NO_ROBOT;

    for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
    {
      if(rawField.isRobotFound(sd->getSystemParams().general.OTHER_TEAM, i))
      {
        Pair tempLoc(rawField.getRobot(sd->getSystemParams().general.OTHER_TEAM, i).getPos());
        float tempDist = ballLoc.distanceTo(tempLoc);
        if(closestOpID == NO_ROBOT ||
           tempDist < closestOpDistance)
        {
          closestOp.set(tempLoc);
          closestOpDistance = tempDist;
          closestOpID = i;
        }
      }
    }

    return angleBetween(closestOp,
                        ballLoc);
  }
}
//==================================================================
