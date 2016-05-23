#include "corner/corner.h"
#include "../math/dist.h"
#include "math/angle.h"
#include "../datatypes/ai.h"
#include "../../plays/basePlay.h"


using namespace ScratchVars;
//---------------------------------------------------------------------------------//
//returns true if the ball is in the corner (26cm away)
bool ballInCorner(const VisionData& field,RobocupStrategyData* sd){

  const SystemParameters& params =sd->getSystemParams();
  float posXball,posYball;

  posXball=field.getBall(0,0).getXPos();
  posYball=field.getBall(0,0).getYPos();
  if (  
        ((params.field.THEIR_GOAL_LINE-params.strategy2002.CORNER_DIST) <= posXball) &&
        
        ( 
          ((params.field.LEFT_SIDE_LINE-params.strategy2002.CORNER_DIST)  <= posYball) ||
          ((params.field.RIGHT_SIDE_LINE+params.strategy2002.CORNER_DIST) >= posYball)
          
        ) 
     )
    return true;
  else 
    return false;
}

//---------------------------------------------------------------------------------//
//returns true if the ball is in the corner area (30cm away)
bool ballStillInCorner(const VisionData& field,
                  const SystemParameters& params){

  float posXball,posYball;

  posXball=field.getBall(0,0).getXPos();
  posYball=field.getBall(0,0).getYPos();

  if (((params.field.THEIR_GOAL_LINE-0.3f)<=posXball)&&((params.field.LEFT_SIDE_LINE-0.3f)<=ABS(posYball)) ||
      ((params.field.THEIR_GOAL_LINE-0.3f)<=posXball)&&((params.field.LEFT_SIDE_LINE-0.3f)<=ABS(posYball)))
      return true;
  else 
    return false;
}

//---------------------------------------------------------------------------------//
void getBallOutOfCorner(RobotIndex ID,const VisionData& field,
               Destination * command, RobocupStrategyData* sd,BasePlay* play){

  Pair robotPos;
  int destination;
  int cornerCounter=sd->getMoveCounter(ID);
  const SystemParameters& params =sd->getSystemParams();

  robotPos=field.getRobot(params.general.TEAM,ID).getPos();

  if ((cornerCounter>params.strategy2002.STEP1COUNTER)&&(cornerCounter<params.strategy2002.STEP2COUNTER)){
      destination=2;
  }
  else if ((cornerCounter==params.strategy2002.STEP2COUNTER)&&(cornerCounter<params.strategy2002.STEP3COUNTER)){
      destination=3;
      play->startTimer();
  }
  else if ((cornerCounter>params.strategy2002.STEP2COUNTER)&&(cornerCounter<params.strategy2002.STEP3COUNTER)){
    destination=3;
  }
  else if ((cornerCounter==params.strategy2002.STEP3COUNTER)&&(cornerCounter<params.strategy2002.STEP4COUNTER)){
      destination=4;
      play->startTimer();
  }
  else if ((cornerCounter>params.strategy2002.STEP3COUNTER)&&(cornerCounter<params.strategy2002.STEP4COUNTER)){
    destination=4;
  }
  else if ((cornerCounter==params.strategy2002.STEP4COUNTER)&&(cornerCounter<params.strategy2002.STEP5COUNTER)){
      destination=5;
      play->startTimer();
  }
  else if ((cornerCounter>params.strategy2002.STEP4COUNTER)&&(cornerCounter<params.strategy2002.STEP5COUNTER)){
    destination=5;
  }
  else if ((cornerCounter==params.strategy2002.STEP5COUNTER)&&(cornerCounter<params.strategy2002.STEP6COUNTER)){
      destination=6;
      play->startTimer();
  }
  else if ((cornerCounter>params.strategy2002.STEP5COUNTER)&&(cornerCounter<params.strategy2002.STEP6COUNTER)){
    destination=6;
  }
  else if ((cornerCounter==params.strategy2002.STEP6COUNTER)&&(cornerCounter<params.strategy2002.STEP7COUNTER)){
      destination=7;
      play->startTimer();
  }
  else if ((cornerCounter>params.strategy2002.STEP6COUNTER)&&(cornerCounter<params.strategy2002.STEP7COUNTER)){
    destination=7;
  }
  else {
    destination=1;
    if ((robotPos.distanceTo(params.field.THEIR_GOAL_LINE-params.strategy2002.LEFT_CORNER_POINT_1X,params.field.LEFT_SIDE_LINE-params.strategy2002.LEFT_CORNER_POINT_1Y)>0.01f)&&(field.getBall(0,0).getYPos()>0))
          sd->resetMoveCounter(ID);
    if ((robotPos.distanceTo(params.field.THEIR_GOAL_LINE-params.strategy2002.RIGHT_CORNER_POINT_1X,params.field.RIGHT_SIDE_LINE+params.strategy2002.RIGHT_CORNER_POINT_1Y)>0.01f)&&(field.getBall(0,0).getYPos()<0))
          sd->resetMoveCounter(ID);

    //if 3 seconds pass without reaching the target, set next target...
    if(play->readTimer() > 3)
    {
      destination++;
      play->startTimer();

    }
  }

  printf("Destination %d\n",destination);

  command->setSpeed(CAREFUL_SPEED);
  command->setDribble(FAST_DRIBBLE);
  command->setVerticalDribble(FAST_V_DRIBBLE);
  
  switch (destination){
  case 1:{
      command->setSpeed(DEFAULT_SPEED);
      if (field.getBall(0,0).getYPos()<0){
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.RIGHT_CORNER_POINT_1X);
        command->setYPos(params.field.RIGHT_SIDE_LINE+params.strategy2002.RIGHT_CORNER_POINT_1Y);
      }
      else{
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.LEFT_CORNER_POINT_1X);
        command->setYPos(params.field.LEFT_SIDE_LINE-params.strategy2002.LEFT_CORNER_POINT_1Y);
      }
	    command->setRotation(PI/4);
      break;
    }
  case 2:{
    if (field.getBall(0,0).getYPos()<0){
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.RIGHT_CORNER_POINT_2X);
        command->setYPos(params.field.RIGHT_SIDE_LINE+params.strategy2002.RIGHT_CORNER_POINT_2Y);
    }
    else{
      command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.LEFT_CORNER_POINT_2X);
      command->setYPos(params.field.LEFT_SIDE_LINE-params.strategy2002.LEFT_CORNER_POINT_2Y);
    }
      command->setRotation(PI/4);
      break;
    }
  case 3:{
    if (field.getBall(0,0).getYPos()<0){
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.RIGHT_CORNER_POINT_3X);
        command->setYPos(params.field.RIGHT_SIDE_LINE+params.strategy2002.RIGHT_CORNER_POINT_3Y);
    }
    else{
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.LEFT_CORNER_POINT_3X);
        command->setYPos(params.field.LEFT_SIDE_LINE-params.strategy2002.LEFT_CORNER_POINT_3Y);
    }
	    command->setRotation(PI/4);
      break;
    }
  case 4:{
    if (field.getBall(0,0).getYPos()<0){
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.RIGHT_CORNER_POINT_4X);
        command->setYPos(params.field.RIGHT_SIDE_LINE+params.strategy2002.RIGHT_CORNER_POINT_4Y);
    }
    else{
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.LEFT_CORNER_POINT_4X);
        command->setYPos(params.field.LEFT_SIDE_LINE-params.strategy2002.LEFT_CORNER_POINT_4Y);
    }
	    command->setRotation(PI/3);
      break;
    }
  case 5:{
    if (field.getBall(0,0).getYPos()<0){
      command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.RIGHT_CORNER_POINT_5X);
      command->setYPos(params.field.RIGHT_SIDE_LINE+params.strategy2002.RIGHT_CORNER_POINT_5Y);
    }
    else{
      command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.LEFT_CORNER_POINT_5X);
      command->setYPos(params.field.LEFT_SIDE_LINE-params.strategy2002.LEFT_CORNER_POINT_5Y);
    }
	    command->setRotation(PI/4);
      break;
    }
  case 6:{
    if (field.getBall(0,0).getYPos()<0){
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.RIGHT_CORNER_POINT_6X);
        command->setYPos(params.field.RIGHT_SIDE_LINE+params.strategy2002.RIGHT_CORNER_POINT_6Y);
    }
    else{
        command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.LEFT_CORNER_POINT_6X);
        command->setYPos(params.field.LEFT_SIDE_LINE-params.strategy2002.LEFT_CORNER_POINT_6Y);
    }
	    command->setRotation(PI/8);
      break;
    }
  case 7:{
    if (field.getBall(0,0).getYPos()<0){
      command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.RIGHT_CORNER_POINT_7X);
      command->setYPos(params.field.RIGHT_SIDE_LINE+params.strategy2002.RIGHT_CORNER_POINT_7Y);
    }
    else{
      command->setXPos(params.field.THEIR_GOAL_LINE-params.strategy2002.LEFT_CORNER_POINT_7X);
      command->setYPos(params.field.LEFT_SIDE_LINE-params.strategy2002.LEFT_CORNER_POINT_7Y);
    }
	    command->setRotation(-PI/2);
      //increase angular gains...
      break;
    }
  }//end switch
 
  if (field.getBall(0,0).getYPos()<0)
     command->setRotation(-command->getRotation());
  

}
//----------------------------------------------------------------------//
//returns TRUE if any of the robot's is executing the corner manuever...
bool inCornerManuever(RobocupStrategyData* sd){
  
  RobotIndex i;

  for(i=ROBOT0;i<NUM_ROBOTS;i++){
    if (sd->getMoveCounter(i)>0)
      return true;
  }
  return false;

}

//----------------------------------------------------------------------//
void stripBallFromWall(BallStuckStatus stuckFlag,RobotIndex ID,const VisionData& field,
               Destination * command, RobocupStrategyData* sd){

  command->setControl(OMNI_NORMAL);

  Pair newDestination,ballPos,robotPos;
  float separation, theta, newTheta;
  const SystemParameters& params =sd->getSystemParams();
  float robotRot,ballRot;

  ballPos=field.getBall(0,0).getPos();
  ballRot=field.getBall(0,0).getRotation();

  robotPos=field.getRobot(params.general.TEAM,ID).getPos();
  robotRot=field.getRobot(params.general.TEAM,ID).getRotation();
  separation=params.general.BALL_RADIUS+params.general.PLAYER_RADIUS;

  const float LEFT1_WAY_POINT = params.field.LEFT_SIDE_LINE-1.5f*params.general.PLAYER_RADIUS;
  const float RIGHT1_WAY_POINT = params.field.RIGHT_SIDE_LINE+1.5f*params.general.PLAYER_RADIUS;
  const float LEFT2_WAY_POINT = params.field.LEFT_SIDE_LINE-0.5f*params.general.PLAYER_RADIUS;
  const float RIGHT2_WAY_POINT = params.field.RIGHT_SIDE_LINE+0.5f*params.general.PLAYER_RADIUS;
  const float LEFT_Y_BACKING_UP_POINT = params.field.LEFT_SIDE_LINE-2*params.general.PLAYER_RADIUS;
  const float RIGHT_Y_BACKING_UP_POINT = params.field.RIGHT_SIDE_LINE+2*params.general.PLAYER_RADIUS;

  Pair wayPoint1Left(ballPos.getX(),LEFT1_WAY_POINT);
  Pair wayPoint1Right(ballPos.getX(),RIGHT1_WAY_POINT);

  Pair wayPoint2Left(ballPos.getX(),LEFT2_WAY_POINT);
  Pair wayPoint2Right(ballPos.getX(),RIGHT2_WAY_POINT);

  const float FRONT1_WAY_POINT = params.field.THEIR_GOAL_LINE-1.5f*params.general.PLAYER_RADIUS;
  const float BACK1_WAY_POINT = params.field.OUR_GOAL_LINE+1.5f*params.general.PLAYER_RADIUS;
  const float FRONT2_WAY_POINT = params.field.THEIR_GOAL_LINE-0.5f*params.general.PLAYER_RADIUS;
  const float BACK2_WAY_POINT = params.field.OUR_GOAL_LINE+0.5f*params.general.PLAYER_RADIUS;
  const float FRONT_Y_BACKING_UP_POINT = params.field.THEIR_GOAL_LINE-2*params.general.PLAYER_RADIUS;
  const float BACK_Y_BACKING_UP_POINT = params.field.OUR_GOAL_LINE+2*params.general.PLAYER_RADIUS;

  Pair wayPoint1Front(FRONT1_WAY_POINT,ballPos.getY());
  Pair wayPoint1Back(BACK1_WAY_POINT,ballPos.getY());

  Pair wayPoint2Front(FRONT2_WAY_POINT,ballPos.getY());
  Pair wayPoint2Back(BACK2_WAY_POINT,ballPos.getY());

  //always dribble in this maneuver...
  command->setDribble(FAST_DRIBBLE);
  command->setVerticalDribble(FAST_V_DRIBBLE);

  /*
  //going to the initial position in front of the ball...
  if (hasPossession(ID, field, sd->getSystemParams()))
  {
    sd->setMessage(ID, "Strip: Backing up with possession...");
    command->setSpeed(CAREFUL_SPEED);
    if(ballPos.getY()>0){
      //command->setYPos(LEFT_Y_BACKING_UP_POINT);
      command->setYPos(0);
	    command->setRotation(PI/2);
    }
    else{
      //command->setYPos(RIGHT_Y_BACKING_UP_POINT);
      command->setYPos(0);
	    command->setRotation(-PI/2);
    }
  }
  else
  */

  //if the ball is found, go towards it...
  if( isRobocupBallFound(field) )
  {

    newTheta=angleBetween(ID, ballPos,field,sd->getSystemParams());
    thetaRobot=getRotation(ID,field,sd->getSystemParams());
    theta = newTheta - thetaRobot;
    theta = normalizeAngle(theta);
    theta = (float)fabs(theta);

    //Prevents obstacle avoidance from makings us "avoid" the ball...
    //
    if (distToRoboCupBall(sd->getSystemParams().general.TEAM,ID,field) < sd->getSystemParams().general.PLAYER_RADIUS+0.05f){
        command->setControl(OMNI_NO_AVOID);
    }

    //if the angle is too wide with respect to the ball, go in front of it (16cm..)..
    if(theta>PI/6)
    {
      sd->setMessage(ID, "Strip: Going to correct angle...");
      //if it is stuck on one of the sides...
      if (stuckFlag < 3)
      {
        command->setXPos(ballPos.getX());
        if (ballPos.getY()>0)
        {
//	        command->setRotation(PI/2);
	        command->setRotation(newTheta);
          command->setYPos(ballPos.getY()-0.16f);
        }
        else
        {
//          command->setRotation(-PI/2);
          command->setRotation(newTheta);
          command->setYPos(ballPos.getY()+0.16f);
        }
      }
      //if it is stuck on one of the goal line walls...
      else
      {
        //when we get close to the ball, reduce the speed...
        if (distToRoboCupBall(ID,field,sd->getSystemParams())<0.20f)
          command->setSpeed(CAREFUL_SPEED);

        command->setYPos(ballPos.getY());
        if (ballPos.getX()>0)
        {
	        command->setRotation(0);
          command->setXPos(ballPos.getX()-0.16f);
        }
        else
        {
          command->setRotation(PI);
          command->setXPos(ballPos.getX()+0.16f);
        }
      }
    }
    //if the angle is good enough, go straight for the ball...
    else
    {
      sd->setMessage(ID, "Strip: Going to ball...");

      //if it is stuck on one of the sides...
      if (stuckFlag < 3)
      {
        command->setXPos(ballPos.getX());
        if (ballPos.getY()>0)
        {
          //to prevent the robot from slamming into the wall...
          command->setYPos(ballPos.getY()-0.06f);
        }
        else
        {
          //to prevent the robot from slamming into the wall...
          command->setYPos(ballPos.getY()+0.06f);
        }
	      command->setRotation(angleBetween(ID,ballPos ,field,params));

      }
      //if it is stuck on one of the goal line walls...
      else
      {
        //when we get close to the ball, reduce the speed...
        if (distToRoboCupBall(ID,field,sd->getSystemParams())<0.20f)
          command->setSpeed(CAREFUL_SPEED);

        command->setYPos(ballPos.getY());
        if (ballPos.getX()>0)
        {
          //to prevent the robot from slamming into the wall...
          command->setXPos(ballPos.getX()-0.06f);
        }
        else
        {
          //to prevent the robot from slamming into the wall...
          command->setXPos(ballPos.getX()+0.06f);
        }
	      command->setRotation(angleBetween(ID,ballPos ,field,params));
      }
    }
  }
  //
  //IF THE BALL IS OCCLUDED...I don't think we'll ever use it, but just in case...
  //
  else
  {
    
    //if the ball is stuck on one of the SIDE line walls...
    if(stuckFlag <3)
    {
      command->setXPos(ballPos.getX());
  
      //go to the first position of the "blind" maneuver...
      if((dist(robotPos,wayPoint1Left)>0.01)&&(dist(robotPos,wayPoint1Right)>0.01)){
        sd->setMessage(ID, "Blind: going to pos 1...");
        if(ballPos.getY()>0){
          command->setYPos(LEFT1_WAY_POINT);
          command->setRotation(PI/2);
        }
        else{
          command->setYPos(RIGHT1_WAY_POINT);
          command->setRotation(-PI/2);
        }
      
      }
      // go towards the ball...second step...
      else if((dist(robotPos,wayPoint2Left)>0.01)&&(dist(robotPos,wayPoint2Right)>0.01)){
        sd->setMessage(ID, "Blind: going to ball...");
      
        command->setSpeed(CAREFUL_SPEED);
        if(ballPos.getY()>0){
          command->setYPos(LEFT2_WAY_POINT);
          command->setRotation(PI/2);
        }
        else{
          command->setYPos(RIGHT2_WAY_POINT);
          command->setRotation(-PI/2);
        }
      }
      //even though we can't see it, we have possession, so back up with the ball...
      else{
        sd->setMessage(ID, "Blind: backing up with ball...");
        command->setSpeed(CAREFUL_SPEED);
        if(ballPos.getY()>0){
          command->setYPos(LEFT_Y_BACKING_UP_POINT);
          command->setRotation(PI/2);
        }
        else{
          command->setYPos(RIGHT_Y_BACKING_UP_POINT);
          command->setRotation(-PI/2);
        }
      
      }
    }
    //
    //if the ball is stuck on one of the goal line walls...
    //
    else
    {
      command->setSpeed(CAREFUL_SPEED);

      command->setYPos(ballPos.getY());
      
      //go to the first position of the "blind" maneuver...
      if((dist(robotPos,wayPoint1Front)>0.01)&&(dist(robotPos,wayPoint1Back)>0.01)){
        sd->setMessage(ID, "Blind: going to pos 1...");
        if(ballPos.getX()>0){
          command->setXPos(FRONT1_WAY_POINT);
          command->setRotation(0);
        }
        else{
          command->setYPos(BACK1_WAY_POINT);
          command->setRotation(PI);
        }
      
      }
      // go towards the ball...second step...
      else if((dist(robotPos,wayPoint2Front)>0.01)&&(dist(robotPos,wayPoint2Back)>0.01))
      {
        sd->setMessage(ID, "Blind: going to ball...");
        if(ballPos.getX()>0){
          command->setXPos(FRONT2_WAY_POINT);
          command->setRotation(0);
        }
        else{
          command->setXPos(BACK2_WAY_POINT);
          command->setRotation(PI);
        }
      }
      //even though we can't see it, we have possession, so back up with the ball...
      else
      {
        sd->setMessage(ID, "Blind: backing up with ball...");
        if(ballPos.getX()>0){
          command->setXPos(FRONT_Y_BACKING_UP_POINT);
          command->setRotation(0);
        }
        else{
          command->setXPos(BACK_Y_BACKING_UP_POINT);
          command->setRotation(PI);
        }
      
      }
    }
  }
}