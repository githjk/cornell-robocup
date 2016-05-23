#include "WallAvoidance.h"
#include "Trajectory.h"

#include "math/cmath.h"

static float OUR_GOAL_LINE;
static float THEIR_GOAL_LINE;

static float RIGHT_SIDE_LINE;
static float RIGHT_GOALIE_BOX;
static float RIGHT_GOAL_POST;

static float LEFT_SIDE_LINE;
static float LEFT_GOALIE_BOX;
static float LEFT_GOAL_POST;

static float OUR_GOALIE_BOX;
static float THEIR_GOALIE_BOX;

static float PLAYER_RADIUS;

static float MAX_WALL_OFFSET;
static float MAX_WALL_OFFSET_DISTANCE;


//================================================================
// This stores several variables used for wall avoidance
void updateWallAvoidance
(
  float ourGoalLine,
  float theirGoalLine,
  float rightSideLine,
  float rightGoalBox,
  float rightGoalPost,
  float leftSideLine,
  float leftGoalBox,
  float leftGoalPost,
  float ourGoalieBox,
  float theirGoalieBox,
  float playerRadius,
  float maxWallOffset,
  float maxWallOffsetDistance
)
{
  OUR_GOAL_LINE             = ourGoalLine;
  THEIR_GOAL_LINE           = theirGoalLine;

  RIGHT_SIDE_LINE           = rightSideLine;
  RIGHT_GOALIE_BOX          = rightGoalBox;
  RIGHT_GOAL_POST           = rightGoalPost;

  LEFT_SIDE_LINE            = leftSideLine;
  LEFT_GOALIE_BOX           = leftGoalBox;
  LEFT_GOAL_POST            = leftGoalPost;

  OUR_GOALIE_BOX            = ourGoalieBox;
  THEIR_GOALIE_BOX          = theirGoalieBox;

  PLAYER_RADIUS             = playerRadius;

  MAX_WALL_OFFSET           = maxWallOffset;
  MAX_WALL_OFFSET_DISTANCE  = maxWallOffsetDistance;

}


//================================================================
// Choose a new destination such that the robot will not
// hit the corners of the goal

const float AVOIDANCE_TOLERANCE = 0.02f;

void avoidGoalCorners(ObjectPosVel  start,
                      ObjectPosVel& final )
{

  //--------------------------------------------------------------------
  // If we are starting behind the goal line, and our destination will
  // take us outside past one of the goal posts, choose a temporary 
  // destination which will not intercept the goal post

  if(start.xPos < OUR_GOAL_LINE + PLAYER_RADIUS - AVOIDANCE_TOLERANCE)
  {
    if( (start.yPos > RIGHT_GOAL_POST) && (final.yPos < RIGHT_GOAL_POST + PLAYER_RADIUS) )
    {
      final.xPos = OUR_GOAL_LINE + PLAYER_RADIUS;
      final.yPos = RIGHT_GOAL_POST + PLAYER_RADIUS;
    }
    else if ( (start.yPos < LEFT_GOAL_POST) && (final.yPos > LEFT_GOAL_POST - PLAYER_RADIUS ) )
    {
      final.xPos = OUR_GOAL_LINE + PLAYER_RADIUS;
      final.yPos = LEFT_GOAL_POST - PLAYER_RADIUS;
    }
  }
  
  //--------------------------------------------------------------------
  // If we are starting ahead of the goal line, and our destination will
  // take us inside past one of the goal posts, choose a temporary 
  // destination which will not intercept the goal post

  else if (final.xPos < OUR_GOAL_LINE + PLAYER_RADIUS - AVOIDANCE_TOLERANCE)
  {
    if ( (start.yPos < RIGHT_GOAL_POST - PLAYER_RADIUS) && (final.yPos > RIGHT_GOAL_POST) )
    {
      final.xPos = OUR_GOAL_LINE + PLAYER_RADIUS;
      final.yPos = RIGHT_GOAL_POST + PLAYER_RADIUS;
    }
    else if ( (start.yPos > LEFT_GOAL_POST + PLAYER_RADIUS) && (final.yPos < LEFT_GOAL_POST) )
    {
      final.xPos = OUR_GOAL_LINE + PLAYER_RADIUS;
      final.yPos = LEFT_GOAL_POST - PLAYER_RADIUS;
    }
  }
}


//================================================================
// Make sure the nextStep of the robot's path will not lead
// it into the walls

const float maxAccel            = 4.0f;

void avoidWalls (float frameRate,
                 ObjectPosVel final,
                 ObjectPosVel &nextStep )
{

  float accRequired;
  float maxPushoutVelocity  = maxAccel/frameRate;

  //----------------------------------------------------------------
  // Based on the robot's distance from its destination, choose
  // a wall offset.  When the robot is far from its destination, it 
  // should stay clear of the wall (to avoid getting stuck against it).
  // Once it is close to its destination, it should be allowed to get
  // close to the wall (which may be necessary if the robot is trying
  // to get the ball out of the corner)

  float wallOffset;
  float distance;

  distance = (float)sqrt(
      (final.xPos - nextStep.xPos)*(final.xPos - nextStep.xPos) + 
      (final.yPos - nextStep.yPos)*(final.yPos - nextStep.yPos)
    );

  if (distance > MAX_WALL_OFFSET_DISTANCE){
    wallOffset = MAX_WALL_OFFSET; 
  }
  else {
    wallOffset = (MAX_WALL_OFFSET) * (distance / MAX_WALL_OFFSET_DISTANCE);
  }

  if (
     (final.xPos < THEIR_GOAL_LINE + PLAYER_RADIUS) &&
     (final.xPos > OUR_GOAL_LINE   - PLAYER_RADIUS) &&
     (final.yPos < LEFT_SIDE_LINE  - PLAYER_RADIUS) &&
     (final.yPos > RIGHT_SIDE_LINE + PLAYER_RADIUS)
  )
  {
    wallOffset = 0.0f;
  }

  //----------------------------------------------------------------
  // If the robot is already predicted at the wall, push the
  // robot back inside to avoid division by zero

  if (nextStep.yPos == RIGHT_SIDE_LINE + PLAYER_RADIUS + wallOffset) {
    nextStep.yPos += 0.01f;
    return;
  }
  if (nextStep.yPos == LEFT_SIDE_LINE - PLAYER_RADIUS - wallOffset){
    nextStep.yPos -= 0.01f;
    return;
  }
  if (nextStep.xPos == OUR_GOAL_LINE + PLAYER_RADIUS + wallOffset){
    nextStep.xPos += 0.01f ;
    return;
  }
  if (nextStep.xPos == THEIR_GOAL_LINE - PLAYER_RADIUS - wallOffset){
    nextStep.xPos -= 0.01f ;
    return;
  }


  //----------------------------------------------------------------
  // From the robot's current location and velocity, calculate the
  // acceleration required to reach the wall at zero velocity.  Once
  // this acceleration is greater than the hardcoded 'maxAccel'
  // constant, then begin slowing down the robot to make sure it
  // doesn't hit the wall.

  // Avoid right sideline
  if (nextStep.yVel < 0.0f) {

    accRequired = ABS( nextStep.yVel * nextStep.yVel / 
                        (nextStep.yPos - (PLAYER_RADIUS + RIGHT_SIDE_LINE + wallOffset)) );

    if (accRequired > maxAccel) {
      nextStep.yVel =  MIN(maxPushoutVelocity,nextStep.yVel + accRequired / frameRate);
    }
  }

  //----------------------------------------------------------------
  // Avoid left sideline
  if (nextStep.yVel > 0.0f) {
    accRequired = -1.0f * ABS( nextStep.yVel * nextStep.yVel / 
                        ((LEFT_SIDE_LINE - PLAYER_RADIUS - wallOffset) - nextStep.yPos) );

    if (accRequired < -maxAccel) {
      nextStep.yVel = MAX(-maxPushoutVelocity,nextStep.yVel + accRequired / frameRate);
    }
  }

  //----------------------------------------------------------------
  // Avoid our goal line
  if (nextStep.xVel < 0.0f) {
    accRequired = ABS( nextStep.xVel * nextStep.xVel / 
                        (nextStep.xPos - (PLAYER_RADIUS + OUR_GOAL_LINE + wallOffset)) );

    if (accRequired > maxAccel) {
      nextStep.xVel = MIN(maxPushoutVelocity,nextStep.xVel + accRequired / frameRate);
    }
  }

  //----------------------------------------------------------------
  // Avoid their goal line
  if (nextStep.xVel > 0.0f) {
    accRequired = -1.0f * ABS( nextStep.xVel * nextStep.xVel / 
                        ((THEIR_GOAL_LINE - PLAYER_RADIUS - wallOffset) - nextStep.xPos) );

    if (accRequired < -maxAccel) {
      nextStep.xVel = MAX(maxPushoutVelocity,nextStep.xVel + accRequired / frameRate);
    }
  }

}


//================================================================
// Make sure the nextStep of the robot's path will not lead
// it into either goalie box

const float RTol      = 0.00f;
const float inBoxGain = 40.0f;

// These constants are used to help get the robot out of the corner
const float unstuckVelocity = 0.40f;
const float sideFactor = 1.5f;

void avoidGoalieBox (ObjectPosVel final,                    
                     ObjectPosVel &nextStep,
                     float frameRate
)
{

  float maxPushoutVelocity  = maxAccel/frameRate;
  float accRequired;

  //=====================================================================================
  // If we are inside our own goalie box, choose the shorter distance out (towards center
  // or on either side) and scoot at a velocity that is proportional to how much we are
  // inside the goalie box.
  if (nextStep.xPos - OUR_GOALIE_BOX   <= PLAYER_RADIUS + RTol && 
      nextStep.yPos - LEFT_GOALIE_BOX  <= PLAYER_RADIUS + RTol &&
      RIGHT_GOALIE_BOX - nextStep.yPos <= PLAYER_RADIUS + RTol) 
  {
    // If the robot is closer to the goal box line than to either of the goal box sides, 
    // rush forward
    if (nextStep.xPos - OUR_GOALIE_BOX > 
        MAX (nextStep.yPos - LEFT_GOALIE_BOX, RIGHT_GOALIE_BOX - nextStep.yPos)) 
    {
      nextStep.xVel = MAX (
         inBoxGain * (PLAYER_RADIUS + RTol - (nextStep.xPos - OUR_GOALIE_BOX)), 
         nextStep.xVel );
    }
    // if the robot is closer to the left goal box line than to the other two goal box bounds
    // move left
    else if (nextStep.yPos - LEFT_GOALIE_BOX > RIGHT_GOALIE_BOX - nextStep.yPos) 
    {
      nextStep.yVel = MAX (
         inBoxGain * (PLAYER_RADIUS + RTol - (nextStep.yPos - LEFT_GOALIE_BOX)), 
         nextStep.yVel );
    }
    // If the robot is closer to the right goal box line move towards right
    else
    {
      nextStep.yVel = MIN (
         -1.0f * inBoxGain * (PLAYER_RADIUS + RTol - (RIGHT_GOALIE_BOX - nextStep.yPos)), 
         nextStep.yVel );
    }
  }

  //=====================================================================================
  // If we are inside their goalie box, choose the shorter distance out (towards center
  // or on either side) and scoot at a velocity that is proportional to how much we are
  // inside the goalie box.
  else if (THEIR_GOALIE_BOX - nextStep.xPos <= PLAYER_RADIUS + RTol && 
      nextStep.yPos - LEFT_GOALIE_BOX <= PLAYER_RADIUS + RTol &&
      RIGHT_GOALIE_BOX - nextStep.yPos <= PLAYER_RADIUS + RTol) 
  {
    // If the robot is closer to the goal box line than to either of the goal box sides, 
    // rush forward
    if (THEIR_GOALIE_BOX - nextStep.xPos > 
        MAX (nextStep.yPos - LEFT_GOALIE_BOX, RIGHT_GOALIE_BOX - nextStep.yPos)) 
    {
      nextStep.xVel = MIN (
         -1.0f * inBoxGain * (PLAYER_RADIUS + RTol - (THEIR_GOALIE_BOX - nextStep.xPos)), 
         nextStep.xVel
      );
    }
    // if the robot is closer to the left goal box line than to the other two goal box bounds
    // move left
    else if (nextStep.yPos - LEFT_GOALIE_BOX > RIGHT_GOALIE_BOX - nextStep.yPos)
    {
      nextStep.yVel = MAX (
         inBoxGain * (PLAYER_RADIUS + RTol - (nextStep.yPos - LEFT_GOALIE_BOX)), 
         nextStep.yVel );
    }
    // If the robot is closer to the right goal box line move towards right
    else
    {
      nextStep.yVel = MIN (
         -1.0f * inBoxGain * (PLAYER_RADIUS + RTol - (RIGHT_GOALIE_BOX - nextStep.yPos)), 
         nextStep.yVel 
      );
    }
  }

  //=====================================================================================
  // If we are not inside the goalie box, but heading towards it, calculate the acceleration
  // required to stop the robot from going into the box. If the acceleration required is
  // larger than maxAccel, reduce the speed of the robot normal to the direction in which 
  // it is entering the box.
  else {

    //--------------------------------------------------------------------------------
    // If we are headed towards our goal box in the x-direction, reduce the x-component 
    // of the velocity by the required amount
    if ( nextStep.xVel < 0.0f && 
        (nextStep.yPos < LEFT_GOALIE_BOX  + PLAYER_RADIUS) &&
        (nextStep.yPos > RIGHT_GOALIE_BOX - PLAYER_RADIUS ) && 
        (nextStep.xPos < 0.0f)
       )
    {

      accRequired = ABS( nextStep.xVel * nextStep.xVel / 
                    ((OUR_GOALIE_BOX - PLAYER_RADIUS) - nextStep.xPos) );

      if (accRequired > maxAccel) {
        nextStep.xVel = MIN(maxPushoutVelocity, nextStep.xVel + accRequired / frameRate);
      }

      // Avoid getting robots stuck near left corner of box
      if( (final.xPos < OUR_GOALIE_BOX + PLAYER_RADIUS) &&
          (final.yPos > LEFT_GOALIE_BOX) &&
          (nextStep.yPos < LEFT_GOALIE_BOX + PLAYER_RADIUS) &&
          (nextStep.xPos > OUR_GOALIE_BOX) )
      {
        nextStep.yVel =  MAX(unstuckVelocity, nextStep.yVel);
      }

      // Avoid getting robots stuck near right corner of box
      if( (final.xPos < OUR_GOALIE_BOX + PLAYER_RADIUS) &&
          (final.yPos < RIGHT_GOALIE_BOX) &&
          (nextStep.yPos > RIGHT_GOALIE_BOX - PLAYER_RADIUS) &&
          (nextStep.xPos > OUR_GOALIE_BOX) )
      {
        nextStep.yVel = MIN(-unstuckVelocity, nextStep.yVel);
      }
    }

    //--------------------------------------------------------------------------------
    // If we are headed towards their goal box in the x-direction, reduce the x-component 
    // of the velocity by the required amount
    if ( nextStep.xVel > 0.0f && 
        (nextStep.yPos < LEFT_GOALIE_BOX  + PLAYER_RADIUS) &&
        (nextStep.yPos > RIGHT_GOALIE_BOX - PLAYER_RADIUS) &&
        (nextStep.xPos > 0.0f)
       )
    {

      accRequired = -1.0f * ABS( nextStep.xVel * nextStep.xVel / 
                    (nextStep.xPos - (PLAYER_RADIUS + THEIR_GOALIE_BOX)) );

      if (accRequired < -maxAccel) {
        nextStep.xVel = MAX(-maxPushoutVelocity, nextStep.xVel + accRequired / frameRate);
      }

      // Avoid getting robots stuck near left corner of box
      if( (final.xPos > THEIR_GOALIE_BOX - PLAYER_RADIUS) &&
          (final.yPos > LEFT_GOALIE_BOX) &&
          (nextStep.yPos < LEFT_GOALIE_BOX  + PLAYER_RADIUS) &&
          (nextStep.xPos < THEIR_GOALIE_BOX) )
      {
        nextStep.yVel =  MAX(unstuckVelocity, nextStep.yVel);
      }

      // Avoid getting robots stuck near right corner of box
      if( (final.xPos > THEIR_GOALIE_BOX - PLAYER_RADIUS) &&
          (final.yPos < RIGHT_GOALIE_BOX) &&
          (nextStep.yPos > RIGHT_GOALIE_BOX - PLAYER_RADIUS) &&
          (nextStep.xPos < THEIR_GOALIE_BOX) )
      {
        nextStep.yVel = -MAX(unstuckVelocity,-nextStep.yVel);
      }
    }

    //--------------------------------------------------------------------------------
    // If we are headed towards a goalie box from left, increase the y-component of 
    // velocity to avoid going into the box
    if ((nextStep.yVel < 0.0f) && 
        (nextStep.yPos > PLAYER_RADIUS + LEFT_GOALIE_BOX) &&
        ( (nextStep.xPos < OUR_GOALIE_BOX + PLAYER_RADIUS) ||
          (nextStep.xPos > THEIR_GOALIE_BOX - PLAYER_RADIUS))
    ) 
    {
      accRequired = ABS( nextStep.yVel * nextStep.yVel / 
                          (nextStep.yPos - (PLAYER_RADIUS + LEFT_GOALIE_BOX)) );

      if (accRequired > maxAccel) {
        nextStep.yVel = MIN(maxPushoutVelocity, nextStep.yVel + accRequired / frameRate);
      }

      // Avoid getting robots stuck near corner of our box
      if( (final.yPos < LEFT_GOALIE_BOX) &&
          (nextStep.xPos < 0.0f)
        )
      {
        nextStep.xVel = MAX(unstuckVelocity, nextStep.xVel);
      }

      // Avoid getting robots stuck near corner of their box
      if( (final.yPos < LEFT_GOALIE_BOX) &&
          (nextStep.xPos > 0.0f)
        )
      {
        nextStep.xVel = MIN(-unstuckVelocity, nextStep.xVel);
      }

    }

    //--------------------------------------------------------------------------------
    // If we are headed into the box from right, reduce the y-component (increase in 
    // negative y-direction) to avoid going into the box
    if ((nextStep.yVel > 0.0f) && 
        (nextStep.yPos < RIGHT_GOALIE_BOX - PLAYER_RADIUS) &&
        ( (nextStep.xPos < OUR_GOALIE_BOX + PLAYER_RADIUS) ||
          (nextStep.xPos > THEIR_GOALIE_BOX - PLAYER_RADIUS)
        )
    )
    {

      accRequired = -1.0f * ABS( nextStep.yVel * nextStep.yVel / 
                          ((RIGHT_GOALIE_BOX - PLAYER_RADIUS) - nextStep.yPos) );

      if (accRequired < -maxAccel) {
        nextStep.yVel = MAX(-maxPushoutVelocity, nextStep.yVel + accRequired / frameRate);
      }

      // Avoid getting robots stuck near corner of our box
      if( (final.yPos > RIGHT_GOALIE_BOX) &&
          (nextStep.xPos < 0.0f)
        )
      {
        nextStep.xVel = MAX(unstuckVelocity, nextStep.xVel);
      }

      // Avoid getting robots stuck near corner of their box
      if( (final.yPos > RIGHT_GOALIE_BOX) &&
          (nextStep.xPos > 0.0f)
        )
      {
        nextStep.xVel = MIN(-unstuckVelocity, nextStep.xVel);
      }

    }
  }

  //=====================================================================================

}


//================================================================
// Make sure the nextStep of the robot's path will not lead
// it into the walls

const float maxGoalieAccel = 4.5f;

void avoidGoalieWalls (float frameRate,
                       ObjectPosVel final,
                       ObjectPosVel &nextStep )
{

  float accRequired;
  float maxPushoutVelocity  = maxGoalieAccel/frameRate;

  //----------------------------------------------------------------
  // Based on the robot's distance from its destination, choose
  // a wall offset.  When the robot is far from its destination, it 
  // should stay clear of the wall (to avoid getting stuck against it).
  // Once it is close to its destination, it should be allowed to get
  // close to the wall (which may be necessary if the robot is trying
  // to get the ball out of the corner)

  float wallOffset;
  float distance;

  distance = (float)sqrt(
      (final.xPos - nextStep.xPos)*(final.xPos - nextStep.xPos) + 
      (final.yPos - nextStep.yPos)*(final.yPos - nextStep.yPos)
    );

  if (distance > MAX_WALL_OFFSET_DISTANCE){
    wallOffset = MAX_WALL_OFFSET; 
  }
  else {
    wallOffset = (0.01f) * (distance / MAX_WALL_OFFSET_DISTANCE);
  }

  if (
     (final.xPos < THEIR_GOAL_LINE + PLAYER_RADIUS) &&
     (final.xPos > OUR_GOAL_LINE   - PLAYER_RADIUS) &&
     (final.yPos < LEFT_SIDE_LINE  - PLAYER_RADIUS) &&
     (final.yPos > RIGHT_SIDE_LINE + PLAYER_RADIUS)
  )
  {
    wallOffset = 0.0f;
  }

  //----------------------------------------------------------------
  // If we are predicted behind the goal line but outside the goal 
  // posts, push the goalie back onto the field

  //----------------------------------------------------------------
  // If the robot is already predicted at the wall, push the
  // robot back inside to avoid division by zero

  if (nextStep.yPos == RIGHT_GOAL_POST + PLAYER_RADIUS + wallOffset) {
    nextStep.yPos += 0.01f;
    return;
  }
  if (nextStep.yPos == LEFT_GOAL_POST - PLAYER_RADIUS - wallOffset){
    nextStep.yPos -= 0.01f;
    return;
  }
  if (nextStep.xPos == OUR_GOAL_LINE + PLAYER_RADIUS + wallOffset){
    nextStep.xPos += 0.01f ;
    return;
  }

/*
  if (
    (nextStep.yPos <= RIGHT_GOAL_POST + PLAYER_RADIUS + wallOffset) &&
    (nextStep.xPos <= OUR_GOAL_LINE   + PLAYER_RADIUS + wallOffset)
  )
  {
    nextStep.xVel = maxPushoutVelocity;
    nextStep.yVel = maxPushoutVelocity;
    return;
  }
  if (
    (nextStep.yPos >= LEFT_GOAL_POST - PLAYER_RADIUS - wallOffset) &&
    (nextStep.xPos <= OUR_GOAL_LINE  + PLAYER_RADIUS + wallOffset)
  )
  {
    nextStep.xVel =  maxPushoutVelocity;
    nextStep.yVel = -maxPushoutVelocity;
    return;
  }
*/

  //----------------------------------------------------------------
  // From the robot's current location and velocity, calculate the
  // acceleration required to reach the wall at zero velocity.  Once
  // this acceleration is greater than the hardcoded 'maxAccel'
  // constant, then begin slowing down the robot to make sure it
  // doesn't hit the wall.

  // Avoid right goal post if we are behind the goal line
  if (
    (nextStep.yVel < 0.0f) &&
    (nextStep.xPos <= OUR_GOAL_LINE  + PLAYER_RADIUS + wallOffset)
  )
  {
    accRequired = ABS ( nextStep.yVel * nextStep.yVel / 
                        (nextStep.yPos - (PLAYER_RADIUS + RIGHT_GOAL_POST + wallOffset)) );

    if (accRequired > maxGoalieAccel) {
      nextStep.yVel =  MIN(maxPushoutVelocity,nextStep.yVel + accRequired / frameRate);
    }
  }

  //----------------------------------------------------------------
  // Avoid left goal post if we are behind the goal line
  if (
    (nextStep.yVel > 0.0f) &&
    (nextStep.xPos <= OUR_GOAL_LINE  + PLAYER_RADIUS + wallOffset)
  )
  {
    accRequired = -1.0f * ABS( nextStep.yVel * nextStep.yVel / 
                        ((LEFT_GOAL_POST - PLAYER_RADIUS - wallOffset) - nextStep.yPos) );

    if (accRequired < -maxGoalieAccel) {
      nextStep.yVel = MAX(-maxPushoutVelocity,nextStep.yVel + accRequired / frameRate);
    }
  }

  //----------------------------------------------------------------
  // Avoid our goal line if we are moving backwards outside the 
  // goal posts
  if (
    (nextStep.xVel < 0.0f) && 
    ( (nextStep.yPos >= LEFT_GOAL_POST - PLAYER_RADIUS - wallOffset) ||
      (nextStep.yPos <= RIGHT_GOAL_POST + PLAYER_RADIUS + wallOffset) )
  )
  {
    accRequired = nextStep.xVel * nextStep.xVel / 
                        (nextStep.xPos - (PLAYER_RADIUS + OUR_GOAL_LINE + wallOffset));

    if (accRequired > maxGoalieAccel) {
      nextStep.xVel = MIN(maxPushoutVelocity,nextStep.xVel + accRequired / frameRate);
    }
  }

}