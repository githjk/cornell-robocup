#include "defending.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/shoot/shoot.h"
#include "common/util/info/ball/ball.h"
#include "RoboCup_modules\RoboCupStrategyModule.h"

//================================================================================
// Finds the "shadow" cast by one of our robots on our goal line.  It returns the
// left and right points of that shadow, such that y1<y2
void findShadow(Pair ball, Pair robot, const SystemParameters &params, float &y1, float &y2){
  findShadow(ball,robot,params.field.OUR_GOAL_LINE,params,y1,y2);
}
//========================================================================================
// determines whether a robot is in the "consideration zone" for the tight defense skills.
bool isInConsiderationZone(Pair robot,
                           RobocupStrategyData *rsd, 
                           const VisionData &vision, 
                           const SystemParameters &params)
{
  Pair goalMid;
  goalMid.setX(params.field.OUR_GOAL_LINE);
  goalMid.setY(params.field.SPLIT_LINE);

  Pair ball = rsd->getDefensiveBallLocation();
  Position wallRobot=DEFENDER;
  if(badRobot(wallRobot,.03f,vision,*rsd,params)) wallRobot=BLOCKER;
  Pair wallGuy = getLocation(rsd->getRobotByPosition(wallRobot),vision,params);
  wallGuy.setX(wallGuy.getX()+params.general.PLAYER_RADIUS);//top of goalie wanted

  //is it to the correct side of the Blocker?
  if(ABS(ball.getY()- wallGuy.getY()) > ABS(ball.getY() - robot.getY())) return false;

  //is it within the radius?
  if(goalMid.distanceTo(robot) > goalMid.distanceTo(ball)) return false;

  //is it outside of the goalie's shadow?
  if(ball.getX()==wallGuy.getX()) return true;  //avoid divide by zero

  //find goalie shadow line
  float dxdy = (ball.getX() - wallGuy.getX()) / (ball.getY()-wallGuy.getY());
  float xInt = wallGuy.getX() - dxdy*wallGuy.getY();

  if(robot.getX() < (dxdy * robot.getY() + xInt)) return false;

  //it must be in the consideration zone
  return true;
}

bool isInConsiderationZone(const int team,
                           RobotIndex robotID,
                           RobocupStrategyData *rsd, 
                           const VisionData &vision, 
                           const SystemParameters &params)
{
  if(!robotFound(team,robotID,vision,params)) return false;
  return isInConsiderationZone(getLocation(team,robotID,vision),rsd,vision,params);
}
//================================================================================

//===============================================================================
//Finds the left and right endpoints of the shot on goal that op2 has.
//pl is the left endpoint, pr is the right endpoint
//   Originally written for LooseDSpecialOp.  Stolen so ShotBlock can us it, too.
bool findOpShot(RobotIndex op2Index, RobotIndex ignoreID,
                                       float &openShotLeft, 
                                       float &openShotRight,
                                       const VisionData &field,
                                       const SystemParameters &rp) 
{	 
  Pair op2Loc;
  Pair ballLoc;
  op2Loc = getTheirRobotLocation(op2Index, field, rp);
  ballLoc = getBallLocation(field);

  float left = rp.field.LEFT_GOAL_POST;
  float right = rp.field.RIGHT_GOAL_POST;
  float shadowLeft, shadowRight;

  //Iterate through all robots and find their shadows. With this info, find 
  //the open shot that op2 has on our goal.
  //Ignore any robot that is behind op2
  for(RobotIndex index=ROBOT0; index<NUM_ROBOTS; index++) {    
    //First consider one of our robots and the shadow it casts
    //Do nothing if this is the index of the robot using this skill      
    if(robotFound(index,field,rp) && index != ignoreID
      && getXLocation(index, field, rp) < op2Loc.getX()) {
      
      findShadow(op2Loc, getLocation(index,field,rp), rp, shadowRight, shadowLeft);
      
      //Case 0: if the shadow does not fall on any part of the goal, do nothing
      if(shadowRight > left || shadowLeft < right) {}
      //Case 1: if one end of the shadow is outside the goal and one is inside the
      //goal, update the move the left or right marker in (depending on the side)
      else if(shadowLeft > left) left = shadowRight;
      else if(shadowRight < right) right = shadowLeft;
      //Case 2: the shadow must lie completely in the goal.  In this case, ignore 
      //the open area on the same side as the ball, because we are guarding the
      //side OPPOSITE from the ball
      else {
        if(ballLoc.getY() >= 0.0f) left = shadowRight;
        else right = shadowLeft;
      }
    }
    

    //Now consider the opponent of the same RobotIndex and the shadow it casts
    //on the goal.  Ignore op2
    if(theirRobotFound(index,field,rp) && index != op2Index
      && getXLocation(index, field, rp) < op2Loc.getX()) {

      findShadow(op2Loc, getTheirRobotLocation(index,field,rp), rp, shadowRight, shadowLeft);
      
      //Case 0: if the shadow does not fall on any part of the goal, do nothing
      if(shadowRight > rp.field.LEFT_GOAL_POST || shadowLeft < rp.field.RIGHT_GOAL_POST) {}
      //Case 1: if one end of the shadow is outside the goal and one is inside the
      //goal, update the move the left or right marker in (depending on the side)
      else if(shadowLeft > left) left = shadowRight;
      else if(shadowRight < right) right = shadowLeft;
      //Case 2: the shadow must lie completely in the goal.  In this case, ignore 
      //the open area on the same side as the ball, because we are guarding the
      //side OPPOSITE from the ball
      else {
        if(ballLoc.getY() >= 0.0f) left = shadowRight;
        else right = shadowLeft;
      }
    }

  } // END FOR-LOOP
  
  //return the left and right values through the reference params
  openShotLeft = left;
  openShotRight = right;
  //Op2 has a shot if right < left
  return (openShotRight < openShotLeft);

}
//================================================================================
//Calculates whether the ball is heading towards our goal at the specified speed
bool towardGoal(MovingObject ball, 
                float goal_line, 
                float leftbound, 
                float rightbound,
                float ballspeed)
{
  if(leftbound < rightbound)
  {
    float temp = leftbound;
    leftbound = rightbound;
    rightbound = temp;
  }

  //see if ball is moving fast enough
  if(SQRT(ball.getXVel()*ball.getXVel() + ball.getYVel()*ball.getYVel()) < ballspeed){
    return false;
  }

  //see if ball is headed towards goal
  float moveangle = ATAN2(ball.getYVel(),ball.getXVel());
  return towardGoal(ball.getPos(),
                    moveangle,
                    goal_line,
                    leftbound,
                    rightbound);
}

//================================================================================
//Calculates whether something is heading towards our goal
bool towardGoal(Pair thing, float angle, float goal_line, float leftbound, float rightbound)
{
  if(leftbound < rightbound)
  {
    float temp = leftbound;
    leftbound = rightbound;
    rightbound = temp;
  }

  int end=1;
  if (goal_line < thing.getX()) end=-end;
  float yGoalIntersect = end*TAN(angle)*ABS(goal_line-thing.getX()) + thing.getY();
  return ((goal_line < thing.getX()) ^ (ABS(angle) < PI/2)) &&
         yGoalIntersect <= leftbound &&
         yGoalIntersect >= rightbound;

  //return ((angleDifference(angleBetween(thing.getX(),thing.getY(),goal_line,rightbound),angle) >= 0) &&
  //   (angleDifference(angle,angleBetween(thing.getX(),thing.getY(),goal_line,leftbound)) >= 0 ));
}

//================================================================================
//finds out if the defender is gone or out of position
bool badRobot(Position position,float position_dist, 
                 const VisionData &field, 
                 RobocupStrategyData &sd,
                 const SystemParameters &rp)
{
  RobotIndex robotID=sd.getRobotByPosition(position);
  if(robotFound(robotID,field,rp))
  {
    return sd.getDestination(robotID)->getPos().distanceTo(getLocation(robotID,field,rp)) > position_dist;
  }
  else 
  {
    return true;
  }
}
