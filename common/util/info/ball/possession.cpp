/*
 *	POSSESSION.CPP
 */

#include "possession.h"
#include "info/robot/robot.h"
#include "info/ball/ball.h"
#include "math/angle.h"
#include "math/dist.h"
#include "math/angle.h"
#include "GUI/GUI.h"

/****************Begin Possession Algorithms***********************/
//--------------------------------------------------------------------------
//Returns true if the ball is within the radius of our robot at the specified position
bool ballWithinRadius(RobotIndex index, 
					  const VisionData& field, 
					  const SystemParameters& params)
{
  return robotFound(index, field, params)
    && (distToRoboCupBall(index, field, params)
      < (params.general.PLAYER_RADIUS + 2*params.general.BALL_RADIUS + 5));
}
//--------------------------------------------------------------------------


static float thetaToBall;
static float thetaBot;
static float maxDeltaTheta;
static float deltaTheta;
static float botDistToBall;
static Pair robotPos;
static Pair ballPos;

static int possessionArray[NUM_ROBOTS];
static int theirPossessionArray[NUM_ROBOTS];
static Pair lastBallPos;

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
bool friendlyHasPossession(RobotIndex index, 
                           const SystemParameters& p)
{
  //evil robots can never have the ball, silly programmer
  if(index < ROBOT0 || index >= NUM_ROBOTS)
    return false;

  // Have we had possession for GAIN_POSSESSION_FRAMES?
  if (possessionArray[index] >= p.general.GAIN_POSSESSION_FRAMES)
    return true;

  // Have we lost possession for only LOSE_POSSESSION_FRAMES?
  else if ( (possessionArray[index] < 0) && 
            ( possessionArray[index] > (-1 * p.general.LOSE_POSSESSION_FRAMES) ) )
    return true;

  // We have lost possession
  else
    return false;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
bool friendlyHasPossession(RobotIndex index, 
                           const SystemParameters& p,
                           const VisionData& field,
                           RobocupStrategyData& rsd,
                           bool useOffensiveBallWhenLost)
{
  //evil robots can never have the ball, silly programmer
  if(index < ROBOT0 || index >= NUM_ROBOTS)
    return false;

  //if ball found then use possesion array
  if(field.isBallFound(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX))
  {
    return friendlyHasPossession(index, p);
  }
  //else, use offensive or defensive ball and 
  //check distance to and rotation with respect to
  else
  {
    return (
             index == rsd.getOffensiveBallRobot() ||
             friendlyHasPossession(index, p)
           );
    //--------------------------------------------------------------------------------
  }  
}//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Returns true if one of our robots has possession of the ball
bool weHavePossession(VisionData& field,  // This is where everyone else is now
                      SystemParameters& p,      // This contains TEAM, FRAME_RATE, wall locations, etc.
                      RobocupStrategyData& rsd, //to get access to offensive ball data
                      RobotIndex * bot)     
{
  for(RobotIndex ID=ROBOT0; ID<NUM_ROBOTS; ID++)
  {
    //if the robot is found...
    if(field.isRobotFound(p.general.TEAM,ID))
    {
      //if it has possession... (using offensive ball if necessary)
      if(friendlyHasPossession(ID, p, field, rsd, true))
      {
        *bot=ID;
        return true;
      }
    }
  }
  *bot=NO_ROBOT;
  return false;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
bool opponentHasPossession(RobotIndex index, 
                           const SystemParameters& p) 
{ 
  //evil robots can never have the ball, silly programmer
  if(index < ROBOT0 || index >= NUM_ROBOTS)
    return false;

  // Have they had possession for GAIN_POSSESSION_FRAMES?
  if (theirPossessionArray[index] >= p.general.GAIN_POSSESSION_FRAMES)
    return true;

  // Have they lost possession for only LOSE_POSSESSION_FRAMES?
  else if ( (theirPossessionArray[index] < 0) && 
            ( theirPossessionArray[index] > (-1 * p.general.LOSE_POSSESSION_FRAMES) ) )
    return true;

  // They have lost possession
  else
    return false;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Returns true if any enemy robot has possession of the ball
bool theyHavePossession(const VisionData& field, 
                        const SystemParameters& params,
                        RobotIndex * bot) 
{

  for(RobotIndex i=ROBOT0; i<=ROBOT4; i++){

    if( opponentHasPossession(i,params) ) 
    {
        *bot=i;
        return true;
    }
  }
  return false;
}
//--------------------------------------------------------------------------
//*********************************************************//
//--------------------------------------------------------------------------
//*********************************************************//
//--------------------------------------------------------------------------
//*********************************************************//
//--------------------------------------------------------------------------
void updateRobotsPossession(const RawVisionData& field, 
                            const SystemParameters& p,
                            StrategyData* sd)
{
  //if ball is found, store permanantly
  if(field.isBallFound(ROBOCUP_BALL_TEAM,ROBOCUP_BALL_INDEX))
  {
    lastBallPos.set(field.getBall(ROBOCUP_BALL_TEAM,ROBOCUP_BALL_INDEX).getPos());
  }
  //update ball location
  ballPos.set(lastBallPos);

  for (RobotIndex i=ROBOT0;i<NUM_ROBOTS;i++)
  {
      updateFriendlyHasPossession(i,field,p);
      sd->setFriendlyPossession(i, friendlyHasPossession(i, p));
      
      updateOpponentPossession(i,field,p);
      sd->setOpponentPossession(i, opponentHasPossession(i, p));
  }
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void updateFriendlyHasPossession(RobotIndex index, // ID of the travelling robot
							                	 const RawVisionData& field,  // This is where everyone else is now
                                 const SystemParameters& p)      // This contains TEAM, FRAME_RATE, wall locations, etc
{
  if (index == NO_ROBOT)
    return;

  //if robot lost, dec counter
  if(!field.isRobotFound(p.general.TEAM, index))
  {
    if (possessionArray[index] > 0)
      possessionArray[index] = -1;
    else if( possessionArray[index] > -1000)
      possessionArray[index]--;
    return;
  }

  robotPos.set(field.getRobot(p.general.TEAM,index).getPos());
  thetaBot = field.getRobot(p.general.TEAM,index).getRotation();
    
  //--------------------------------------------------------------------------------
  if ( (ballPos.getX() == robotPos.getX() ) && (ballPos.getY() == robotPos.getY() ) ) 
  {
    //==============================
    //==== Incrementing Counter ====
    //==============================
    //lost possession for a long time, must build up possession again
    if (possessionArray[index] <= -p.general.LOSE_POSSESSION_FRAMES)
    {
      possessionArray[index] = 1;
    }
    //lost possession for les than lose possession frames, thus keep possession!
    else if(possessionArray[index] < 0)
    {
      possessionArray[index] = p.general.GAIN_POSSESSION_FRAMES;
    }
    //increment counter if posible
    else 
    {
      if (possessionArray[index] < 1000) 
      {
        possessionArray[index]++;
      }
    } 
    return;
  }
  //--------------------------------------------------------------------------------

  thetaToBall = angleBetween(robotPos, ballPos);

  if(thetaBot <= -1000.0)
  {
    deltaTheta = 0.0f;
  }
  else
  {
    deltaTheta = normalizeAngle(thetaBot-thetaToBall);
  }
  botDistToBall = ballPos.distanceTo(robotPos);


    //--------------------------------------------------------------------------------
    if (botDistToBall < p.general.PLAYER_RADIUS)
    {
      //==============================
      //==== Incrementing Counter ====
      //==============================
      //lost possession for a long time, must build up possession again
      if (possessionArray[index] <= -p.general.LOSE_POSSESSION_FRAMES)
      {
        possessionArray[index] = 1;
      }
      //lost possession for les than lose possession frames, thus keep possession!
      else if(possessionArray[index] < 0)
      {
        possessionArray[index] = p.general.GAIN_POSSESSION_FRAMES;
      }
      //increment counter if posible
      else 
      {
        if (possessionArray[index] < 1000) 
        {
          possessionArray[index]++;
        }
      }
      return;
    }
    
    //--------------------------------------------------------------------------------

    maxDeltaTheta = ABS(ATAN2(p.general.DRIBBLER_LENGTH/2,p.general.DIST_TO_DRIBBLER + p.general.BALL_RADIUS));

    //--------------------------------------------------------------------------------
    //pick tolerance
    float tol;
    if(possessionArray[index] >= p.general.GAIN_POSSESSION_FRAMES)
      tol = p.general.POSSESSION_LOSE_TOLERANCE;
    else
      tol = p.general.POSSESSION_GAIN_TOLERANCE;

    if( (botDistToBall*cos(deltaTheta) <= p.general.DIST_TO_DRIBBLER + p.general.BALL_RADIUS + tol)
      && (ABS(deltaTheta) <= maxDeltaTheta)) 
    {
      //==============================
      //==== Incrementing Counter ====
      //==============================
      //lost possession for a long time, must build up possession again
      if (possessionArray[index] <= -p.general.LOSE_POSSESSION_FRAMES)
      {
        possessionArray[index] = 1;
      }
      //lost possession for les than lose possession frames, thus keep possession!
      else if(possessionArray[index] < 0)
      {
        possessionArray[index] = p.general.GAIN_POSSESSION_FRAMES;
      }
      //increment counter if posible
      else 
      {
        if (possessionArray[index] < 1000) 
        {
          possessionArray[index]++;
        }
      }
    }
    //--------------------------------------------------------------------------------
    else 
    {
      //==============================
      //==== Decrementing Counter ====
      //==============================
      //only decrement if ball found
      if(field.isBallFound(ROBOCUP_BALL_TEAM,ROBOCUP_BALL_INDEX))
      {

      //Has possession, start counting down
      if (possessionArray[index] >= p.general.GAIN_POSSESSION_FRAMES)
      {
        possessionArray[index] = -1;
      }
      //didn't have possesion, so restart
      else if(possessionArray[index] > 0)
      {
        possessionArray[index] = -p.general.LOSE_POSSESSION_FRAMES;
      }
      //already negative, keep counting down
      else if(possessionArray[index] > -1000) 
      {
        possessionArray[index]--;
      }

      }
    }
    //--------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void updateOpponentPossession(RobotIndex index, 
							  const RawVisionData& field, 
							  const SystemParameters& params) 
{
  Pair robotPos(field.getRobot(params.general.OTHER_TEAM,index).getPos());

  if(field.isRobotFound(params.general.OTHER_TEAM,index) &&
     ballPos.distanceTo(robotPos) < params.general.OPPONENT_RADIUS + 3*params.general.BALL_RADIUS)
  {
    //==============================
    //==== Incrementing Counter ====
    //==============================
    //lost possession for a long time, must build up possession again
    if (theirPossessionArray[index] <= -params.general.LOSE_POSSESSION_FRAMES)
    {
      theirPossessionArray[index] = 1;
    }
    //lost possession for les than lose possession frames, thus keep possession!
    else if(theirPossessionArray[index] < 0)
    {
      theirPossessionArray[index] = params.general.GAIN_POSSESSION_FRAMES;
    }
    //increment counter if posible
    else 
    {
      if (theirPossessionArray[index] < 1000) 
      {
        theirPossessionArray[index]++;
      }
    }
  }
  else 
  {
    //==============================
    //==== Decrementing Counter ====
    //==============================
    //only decrement if ball found
    if(field.isBallFound(ROBOCUP_BALL_TEAM,ROBOCUP_BALL_INDEX))
    {

        //Has possession, start counting down
    if (theirPossessionArray[index] >= params.general.GAIN_POSSESSION_FRAMES)
    {
      theirPossessionArray[index] = -1;
    }
    //didn't have possesion, so restart
    else if(theirPossessionArray[index] > 0)
    {
      theirPossessionArray[index] = -params.general.LOSE_POSSESSION_FRAMES;
    }
    //already negative, keep counting down
    else if(theirPossessionArray[index] > -1000) 
    {
      theirPossessionArray[index]--;
    }

    }
  }
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Resets possession counters to zero...
void resetPossessionCounters()
{
  
  for(int i=0;i<NUM_ROBOTS;i++){
    possessionArray[i]= -1000;
    theirPossessionArray[i]= -1000;
  }

}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Returns the coordinates of the front of the robot (at a specified point and rot)
Pair frontOfRobot(Pair robotLoc,
                  float robotRot,
                  const SystemParameters& params
)
{
  float distToFront;
  distToFront = params.general.DIST_TO_DRIBBLER;
  float x = robotLoc.getX() + (distToFront*COS(robotRot));
  float y = robotLoc.getY() + (distToFront*SIN(robotRot));

  return Pair(x, y);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Returns the coordinates of the front of the robot
Pair frontOfRobot(RobotIndex index, 
                  const VisionData& field, 
                  const SystemParameters& params
)
{
  thetaBot = getRotation(index, field, params);
  float distToFront;
  distToFront = params.general.DIST_TO_DRIBBLER;
  float x = getXLocation(index, field, params) + (distToFront*COS(thetaBot));
  float y = getYLocation(index, field, params) + (distToFront*SIN(thetaBot));

  return Pair(x, y);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Returns the coordinates of a point a certain distance in the front of the robot
Pair frontOfRobot(RobotIndex index, 
                  const VisionData& field, 
                  const SystemParameters& params,
                  float tolerance             // extra distance in front of the robot
)
{
  thetaBot = getRotation(index, field, params);
  float distToFront;
  distToFront = params.general.DIST_TO_DRIBBLER;
  float x = getXLocation(index, field, params) + (tolerance+distToFront)*COS(thetaBot);
  float y = getYLocation(index, field, params) + (tolerance+distToFront)*SIN(thetaBot);

  return Pair(x, y);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Given the front of the robot and the angle at which the robot should be facing
// returns the position the robot should be in. Used for receiving passes.
Pair robotPositionGivenFront(const Pair& frontOfBot,
							 float thetaBot,
							 const SystemParameters& params) 
{
  float distToFront;
  distToFront = params.general.DIST_TO_DRIBBLER;
  float x = frontOfBot.getX() - distToFront*COS(thetaBot);
  float y = frontOfBot.getY() - distToFront*SIN(thetaBot);
  return Pair(x, y);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Given a point a distance 'tolerance' in front of the robot and the angle at which the
// robot should be facing returns the position the robot should be in. Used for receiving passes.
Pair robotPositionGivenFront(const Pair& frontOfBot,
							 float thetaBot,
                             const SystemParameters& params,
							 float tolerance) 
{
  float distToFront;
  distToFront = params.general.DIST_TO_DRIBBLER;
  float x = frontOfBot.getX() - (tolerance+distToFront)*COS(thetaBot);
  float y = frontOfBot.getY() - (tolerance+distToFront)*SIN(thetaBot);
  return Pair(x, y);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
