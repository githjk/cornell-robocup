
#include "five_man_star.h"
#include "../basic_actions.h"
#include "info/ball/ball.h"
#include "shoot/shoot.h"
#include "math/angle.h"
#include "pass/receive_pass.h"
#include "pass/pass.h"
#include "math/dist.h"
#include "lane/lane.h"

#define STAR_RADIUS .60f
Play FiveManStar::playNum = -1;

FiveManStar::FiveManStar(VisionModule *vm) : BasePlay(vm)
{

}


void FiveManStar::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  starState = RETRIEVING_BALL;
  for (RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++)
  {
    if( friendlyHasPossession(i,rsd->getSystemParams()) )
    {
      
    }
  }
  starPos[0].set(STAR_RADIUS*COS(PI),STAR_RADIUS*SIN(PI));
  starPos[1].set(STAR_RADIUS*COS(3*PI/5),STAR_RADIUS*SIN(3*PI/5));
  starPos[2].set(STAR_RADIUS*COS(PI/5),STAR_RADIUS*SIN(PI/5));
  starPos[3].set(STAR_RADIUS*COS(-PI/5),STAR_RADIUS*SIN(-PI/5));
  starPos[4].set(STAR_RADIUS*COS(-3*PI/5),STAR_RADIUS*SIN(-3*PI/5));
  
}

ActionFunction FiveManStar::getActionFunction(Position pos)
{
  return stopRobot;
}
//sets the state based on who has the ball
void FiveManStar::setStarState(RobotIndex ID)
{
  prevState = starState;
  switch(ID)
  {
  case (ROBOT0):  starState = PASSER_0;
    break;
  case (ROBOT1):  starState = PASSER_1;
    break;
  case (ROBOT2):  starState = PASSER_2;
    break;
  case (ROBOT3):  starState = PASSER_3;
    break;
  case (ROBOT4):  starState = PASSER_4;
    break;
  default:        starState = RETRIEVING_BALL;
  }
}
/*
//sets the state based on who has the ball
void FiveManStar::setStarState(RobotIndex ID)
{
switch(ID)
{
case (ROBOT0):  starState = PASSING_TO_2;
break;
case (ROBOT1):  starState = PASSING_TO_3;
break;
case (ROBOT2):  starState = PASSING_TO_4;
break;
case (ROBOT3):  starState = PASSING_TO_0;
break;
case (ROBOT4):  starState = PASSING_TO_1;
break;
default:        starState = RETRIEVING_BALL;
}
}
*/
/*
Pair FiveManStar::getStarPos(RobotIndex ID)
{
switch(ID)
{
case (ROBOT0):
return Pair pos(STAR_RADIUS*COS(PI),STAR_RADIUS*SIN(PI));
case (ROBOT1):
return Pair pos(STAR_RADIUS*COS(3*PI/5),STAR_RADIUS*SIN(3*PI/5));
case (ROBOT2):
return Pair pos(STAR_RADIUS*COS(PI/5),STAR_RADIUS*SIN(PI/5));
case (ROBOT3):
return Pair pos(STAR_RADIUS*COS(-PI/5),STAR_RADIUS*SIN(-PI/5));
case (ROBOT4):
return Pair pos(STAR_RADIUS*COS(-3*PI/5),STAR_RADIUS*SIN(-3*PI/5));
default:
ASSERT(ID != NO_ROBOT,"Bad Position: Can't run Five Man Star with less than 5 robots.");
return Pair pos(0.0f,0.0f);
}
}
*/
/*
float FiveManStar::getStarAngle(RobotIndex ID)
{
switch(ID)
{
case (ROBOT0):
return angleBetween(ID,Pair(0.0f,0.0f),field,params);
case (ROBOT1):
return Pair pos(STAR_RADIUS*COS(3*PI/5),STAR_RADIUS*SIN(3*PI/5));
case (ROBOT2):
return Pair pos(STAR_RADIUS*COS(PI/5),STAR_RADIUS*SIN(PI/5));
case (ROBOT3):
return Pair pos(STAR_RADIUS*COS(-PI/5),STAR_RADIUS*SIN(-PI/5));
case (ROBOT4):
return Pair pos(STAR_RADIUS*COS(-3*PI/5),STAR_RADIUS*SIN(-3*PI/5));
default:
ASSERT(ID != NO_ROBOT,"Bad Angle: Can't run Five Man Star with less than 5 robots.");
return Pair pos(0.0f,0.0f);
}
}
*/

bool FiveManStar::receiveStarPass(RobotIndex passerID,RobotIndex receiverID,
                                  const VisionData& vision,RobocupStrategyData* rsd)
{
  rsd->setMessage(receiverID, "Receiving Pass");
  Destination* command = rsd->getDestination(receiverID);
  command->setControl(OMNI_NORMAL_ENTERBOX);
  
  Pair interceptionPoint;
  bool validPass;
  validPass = interceptPass(
    getBallLocation(vision),
    ballVel(vision),
    starPos[(int)receiverID],
    &interceptionPoint,
    rsd->getSystemParams()
    );
  
  float destAngle;
  Pair ballDest;
  command->setDribble(FAST_DRIBBLE);
  
  // This is a pass-pass combination, so rebound to the next robot
  if ( validPass )
  {
    // Enable the kicker
    /*    command->setKick(KICK_PASS);
    destAngle = reboundAngle(
    ballVel(vision),
    interceptionPoint,
    starPos[((int)receiverID+2)%5],
    rsd->getParams()
    );
    
    */
    //    command->setVerticalDribble();
    
    destAngle = angleToBall(receiverID,vision,rsd->getSystemParams());
    command->setRotation(destAngle);
    command->setPos(robotPositionGivenFront(interceptionPoint, destAngle, rsd->getSystemParams()));
    return true;
  }
  /*
  // Otherwise, just trap the pass
  else
  {
  // Enable the dribbler
  command->setDribble();
  destAngle = angleToBall(ID, field, sd->getParams());
  
    sd->setMessage(ID, "Trapping pass");
    
      }
  */
  else {
    return false;
  }
  
}


void FiveManStar::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  Pair bVel(ballVel(*vision));
  
  RobotIndex activeID;
  
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    rsd->setMessage(i,"Waiting in position");
    rsd->getDestination(i)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(i)->setSpeed(DEFAULT_SPEED);
  }
  
  if ( starState == RETRIEVING_BALL )
  {
    //find closest robot to the ball and have him retrieve the ball and have the rest go to position
    findShortestRobotDistance(*vision,rsd->getSystemParams().general.TEAM,&activeID);
    rsd->getDestination(i)->setSpeed(CAREFUL_SPEED);
    
    for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
    {
      if( i != activeID)
      {
        rsd->setMessage(i,"Going to position");
        rsd->getDestination(i)->setPos(starPos[(int)i]);
        rsd->getDestination(i)->setRotation(angleBetween(i,Pair(0.0f,0.0f),*vision,rsd->getSystemParams()));
      }
      
    }
    //if we have possession, go the robot's point in the star, otherwise get the ball
    if ( friendlyHasPossession(activeID,rsd->getSystemParams()) )
    {
      if( dist(activeID,Pair(0.0f,0.0f),*vision,rsd->getSystemParams()) < 70.0f &&
        dist(activeID,starPos[(int)activeID],*vision,rsd->getSystemParams()) < 20.0f)
      {
        rsd->setMessage(activeID,"Aiming and passing");
        if(true)
//        if( aimTowardsXY(starPos[(int)(activeID+2)%5].getX(),
 //         starPos[(int)(activeID+2)%5].getY(),
  //        activeID,*vision,rsd,rsd->getDestination(activeID)) < rsd->getSystemParams().strategy2002.PASS_TOL )
        {
          setStarState(activeID);
        }
      }
      //if close to position, then aim towards the next robot and change the state
      else if( starPos[(int)activeID].distanceTo( getLocation(activeID,*vision,rsd->getSystemParams()) ) < 0.10f )
      {
        rsd->setMessage(activeID,"Aiming and passing");
        if(true)
//        if( aimTowardsXY(starPos[(int)(activeID+2)%5].getX(),
//          starPos[(int)(activeID+2)%5].getY(),
 //         activeID,*vision,rsd,rsd->getDestination(activeID)) < rsd->getSystemParams().strategy2002.PASS_TOL )
        {
          setStarState(activeID);
        }
      }
      // dribble back to position in the star
      else
      {
        // spin around ball if necessary
        //        if( ABS(normalizeAngle(angleBetween(getLocation(activeID,*vision,rsd->getParams()),
        //                                            starPos[(int)activeID])-getRotation(activeID,*vision,rsd->getParams()))) > PI/6)

        if(true)
//        if( aimTowardsXYnew(starPos[(int)activeID].getX(),starPos[(int)activeID].getY(),activeID,
 //         *vision,rsd,rsd->getDestination(activeID)) > PI/6)
        {
          rsd->setMessage(activeID,"Spinning with ball");
          rsd->getDestination(activeID)->setDribble(FAST_DRIBBLE);
          rsd->getDestination(activeID)->setVerticalDribble(FAST_V_DRIBBLE);
        }
        else
        {
          rsd->setMessage(activeID,"Going back to position with ball");
          rsd->getDestination(activeID)->setDribble(FAST_DRIBBLE);
          rsd->getDestination(activeID)->setVerticalDribble(FAST_V_DRIBBLE);
          rsd->getDestination(activeID)->setPos(starPos[(int)activeID]);
          //          rsd->getDestination(activeID)->setRotation(angleBetween(activeID,starPos[(int)activeID],*vision,rsd->getParams()));
        }
      }
    }
    // go get the ball
    else
    {
      rsd->setMessage(activeID,"Getting ball");
      rsd->getDestination(activeID)->setDribble(FAST_DRIBBLE);
      rsd->getDestination(activeID)->setVerticalDribble(FAST_V_DRIBBLE);
      rsd->getDestination(activeID)->setPos( robotPositionGivenFront(getBallLocation(*vision),
        angleToBall(activeID,*vision,rsd->getSystemParams()),
        rsd->getSystemParams(),
        -.01f) );
      rsd->getDestination(activeID)->setRotation( angleToBall(activeID,*vision,rsd->getSystemParams()) );
    }
  }
  
  // pass the ball to next robot
  else
  {
    RobotIndex passerID = (RobotIndex)starState;
    RobotIndex receiverID = (RobotIndex)(((int)starState+2)%5);
    
    rsd->setMessage(passerID,"Passing the ball");
    rsd->setMessage(receiverID,"Receiving the ball");
    for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
    {
      if( i != passerID && i!= receiverID)
      {
        rsd->setMessage(i,"Going to position");
        rsd->getDestination(i)->setPos(starPos[(int)i]);
        rsd->getDestination(i)->setRotation(angleBetween(i,Pair(0.0f,0.0f),*vision,rsd->getSystemParams()));
      }
    }
    
    // if a robot just retrieved the ball, just aim toward the next robot and pass the ball
    //   if( prevState == RETRIEVING_BALL )
    //   {
    if( friendlyHasPossession(passerID,rsd->getSystemParams()) )
    {
//      aimAndShoot(starPos[(int)receiverID],passerID,*vision,rsd,rsd->getDestination(passerID),KICK_PASS);
    }
    else
    {
        rsd->setMessage(passerID,"Going to position with ball");
        rsd->getDestination(passerID)->setPos(starPos[(int)i]);
        rsd->getDestination(passerID)->setRotation(angleBetween(passerID,Pair(0.0f,0.0f),*vision,rsd->getSystemParams()));
    }
    if( !receiveStarPass(passerID,receiverID,*vision,rsd) )
    {
      if( getBallLocation(*vision).distanceTo(Pair(0.0f,0.0f)) > STAR_RADIUS ||
        bVel.magnitude() <.05f )
      {
        rsd->setMessage(receiverID, "Missed pass");
        prevState = starState;
        starState = RETRIEVING_BALL;
      }
      else
      {
        prevState = starState;
        starState = (PassState)( ((int)starState+1)%5 );
      }
    }
  }
}
