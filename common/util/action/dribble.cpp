/*
 *  dribble.cpp
 */

#include "dribble.h"
#include "info/robot/robot.h"
#include "info/ball/ball.h"
#include "math/angle.h"
#include "info/ball/possession.h"
#include "shoot/shoot.h"
#include "math/find_open_space.h"

//-------------------------------------------------
      /*****************************
    **********************************
  ***                                ***
***   Below are the infamous dribbling ***
***    algorithms, 'sugar-coated' to   ***
***        Nicole's delight. =-)       ***
  ***                                ***
    **********************************
       ***************************/
//-------------------------------------------------



//Instructs thisRobot to DRIBBLE UP THE FIELD
//If there is nothing between thisRobot and the goal post
//on the current side of the field, it heads right at that
//goal post.
void dribbleBallUpfield(const VisionData& field,
                        const SystemParameters& params,
                        RobotIndex thisRobot, 
                        Destination* command,
                        RobotIndex screeningRobot) 
{
  //Location of this robot
  Pair location = getLocation(thisRobot, field, params);
  Pair destination;
  
  destination.setX( params.field.THEIR_GOALIE_BOX - params.general.PLAYER_RADIUS );
  if(location.getY() > 0.0f)
    destination.setY( params.field.LEFT_GOAL_POST - params.general.PLAYER_RADIUS );
  else
    destination.setY( params.field.RIGHT_GOAL_POST + params.general.PLAYER_RADIUS );
  
  dribbleBall(field, params, thisRobot, destination, command, screeningRobot);

}
//--------------------------------------------------------------------------


//Instructs thisRobot to DRIBBLE BACK DOWN THE FIELD
//If there is nothing between thisRobot and the center of 
//the field current side of the field, it heads right at 
//the center of the field.
void dribbleBallBack(const VisionData& field,
                     const SystemParameters& params,
                     RobotIndex thisRobot,   
                     Destination* command,
                     RobotIndex screeningRobot) 
{
  Pair destination(0.0f, 0.0f);
  
  dribbleBall(field,
              params,
              thisRobot,
              destination,
              command,
              screeningRobot,
              true);
}
//--------------------------------------------------------------------------

static Pair robotPos;
static Pair ballPos;
static float leftLimit, rightLimit, bottomLimit, topLimit;
static float cosXsin, cosSquared, sinSquared;
static bool noObstacles;

//This is the main dribble algorithm that the sugar coated versions
//above all call. Takes a point and dribbles to it as best it can
//Using the find_open_space and spin_around_the_ball algorithms =-)
void dribbleBall(const VisionData& field,
                 const SystemParameters&  params,
                 RobotIndex        thisRobot, 
                 const Pair&       ultimateDestination,
                 Destination*      command,
                 RobotIndex        screeningRobot,
                 bool              backwards)
{
  bool spinningAroundBall = false;
  
  robotPos = getLocation(thisRobot, field, params);
  
  ballPos = getBallLocation(field);
  
  //***************************************************************************
  //***************************************************************************
  // GET INTERMEDIATE TARGET
  //***************************************************************************
  //***************************************************************************
  
  //Width of searching box
  const float searchWidth = 0.85f;
  
  //Height of searching box
  const float searchHeight = 0.70f;
  
  //will try to find a destination between minTravelDistance
  // and (minTravelDistance + searchHeight) away
  const float minTravelDistance = 0.10f;
  //---------------------------------------------------
  //If target is ahead of us, dribble upfield, also dribble back
  //---------------------------------------------------   
  //Move upfield
  if(!backwards)
  {
    bottomLimit = robotPos.getX() + minTravelDistance; //ALWAYS go forward
    topLimit = robotPos.getX() +  searchHeight + minTravelDistance;
    //If too far back, crop extra on the back, add to front
    if(bottomLimit < params.field.OUR_GOAL_LINE)
    {
      topLimit+=(params.field.OUR_GOAL_LINE - bottomLimit);
      bottomLimit = params.field.OUR_GOAL_LINE;
    }
    //If too far up, crop extra on front.
    if(topLimit > params.field.THEIR_GOAL_LINE)
      topLimit = params.field.THEIR_GOAL_LINE;
  }
  
  //Move back
  else
  {
    topLimit = robotPos.getX() - 10.0f; //ALWAYS go backward
    bottomLimit = robotPos.getX() - searchHeight - 0.10f;
    //If too far up, crop extra on front, add to back
    if(topLimit > params.field.THEIR_GOAL_LINE)
    {
      bottomLimit-=(topLimit - params.field.THEIR_GOAL_LINE);
      topLimit = params.field.THEIR_GOAL_LINE;
    }
    //If too far back, crop extra on the back.
    if(bottomLimit < params.field.OUR_GOAL_LINE)
      bottomLimit = params.field.OUR_GOAL_LINE;
  }
  
  //---------------------------------------------------
  //These limits are the same for both directions
  leftLimit   = robotPos.getY() + (searchWidth/2);
  rightLimit  = robotPos.getY() - (searchWidth/2);
  //---------------------------------------------------
  //This cropping is the same for both directions
  //Crop when necessary
  //---------------------------------------------------
  //If too far left, crop left, add extra to right side
  if(leftLimit > params.field.LEFT_SIDE_LINE)
  {
    rightLimit -= (leftLimit - params.field.LEFT_SIDE_LINE);
    leftLimit   = params.field.LEFT_SIDE_LINE;
  }
  
  //If too far right, crop right, add extra to left side
  if(rightLimit < params.field.RIGHT_SIDE_LINE)
  {
    leftLimit  += (params.field.RIGHT_SIDE_LINE - rightLimit);
    rightLimit  = params.field.RIGHT_SIDE_LINE;
  }
  //---------------------------------------------------
  //find most open space in range
  Pair destinationPos;

  sendRobotintoOpenSpace(field, 
                         params, 
                         command,
                         leftLimit,
                         rightLimit,
                         topLimit,
                         bottomLimit,
                         noObstacles,
                         thisRobot,
                         screeningRobot);

  //set the pair, since sendRobotintoOpenSpace only sets the actual robots destination
  destinationPos.set( command->getPos() );
  //---------------------------------------------------
  //If there is a line of sight between us and 
  //our destination, head right at it.
  if(isLane(robotPos, ultimateDestination, params.general.PLAYER_RADIUS,  field, params, false))
  {
      destinationPos = ultimateDestination;
  }
  //---------------------------------------------------

  //If the destination is inside the enemies goal box, move it out!
  //Goalie Box
  if(nearTheirGoalieBox(destinationPos, params, params.general.PLAYER_RADIUS))
  {
    destinationPos.setX( (params.field.THEIR_GOALIE_BOX - params.general.PLAYER_RADIUS) );
    //If closer to left, go to the left side
    if(robotPos.getY() > 0.0f)
      destinationPos.setY( (params.field.LEFT_GOAL_POST + params.general.PLAYER_RADIUS) );
    else
      destinationPos.setY( (params.field.RIGHT_GOAL_POST - params.general.PLAYER_RADIUS) );
  }
  //---------------------------------------------------
  
  //****************************************************************************
  //***************************************************************************
  // DRIBBLE TOWARDS THAT TARGET
  //***************************************************************************
  //***************************************************************************
  
  command->initialize();
  command->setPos(destinationPos);

  const float ROTATION_THRESHOLD = PI;
  
  float angleToDestination = angleBetween(robotPos, destinationPos);
  
  float currentAngle = getRotation(params.general.TEAM, thisRobot, field, params);
  
  // If the next destination is so great a turn that we would lose the ball,
  // or if we have already lost the ball, use spinAroundBall to reclaim the
  // ball facing the correct direction

  //if we are already spinning around the ball, keep doing so, break out
  //with boolean internally =-)
  if(spinningAroundBall)
  {
    spinAroundBall(params,
      field,
      thisRobot,
      robotPos, 
      destinationPos,
      command);
    
    if((float)(fabs(currentAngle - angleToDestination)) < (PI/8))
      spinningAroundBall = false;
    
  }
  //if not spinning, but need to beacuse angle too great OR ball is NOT between
  //us and our target (in which case you could run through the ball), then begin
  //sprin around the ball until it's safe to dribble again
  else if(  
          (fabs(angleToBall(thisRobot, field, params) - angleToDestination) > ROTATION_THRESHOLD) ||
          (!isBallBetween(robotPos, 
                          destinationPos,       
                          (2.0f*params.general.PLAYER_RADIUS - 2.0f*params.general.BALL_RADIUS),
                          field))
         )
  {
    spinningAroundBall = true;
    
    spinAroundBall(params,
                   field,
                   thisRobot,
                   robotPos, 
                   ultimateDestination,
                   command);
  }
  //if movement is not too much different from facing the ball, just face the ball
  else
  {   
    command->setRotation( angleToDestination );
  }
  if(ballPos.distanceTo(robotPos) < 0.23f) {
  command->setDribble(FAST_DRIBBLE);
  command->setVerticalDribble(FAST_V_DRIBBLE);
  }
}






//Spins around the ball, keeping ball in front of robot
void spinAroundBall(
            const SystemParameters& params,
            const VisionData& field,
            RobotIndex          thisRobot,     
            const Pair&       ultimateDestination,
            Destination*      command
)
{
  spinAroundBall(params, field, thisRobot, getLocation(thisRobot, field, params), ultimateDestination, command);
}
//--------------------------------------------------------------------------

static Pair ballLocation;
static Pair pivotPoint;
static Pair rotatedDestination;
static float currentAngle;
static float dx;
static float dy;

//Spins around the ball, keeping ball on front of robot.
void spinAroundBall(const SystemParameters& params,
                    const VisionData& field,
                    RobotIndex          thisRobot,  
                    const Pair&       robotLocation, 
                    const Pair&       ultimateDestination,
                    Destination*      command)
{
  //stay a player radius away, but we are square in front, so multiply by 0.707,
  //and also subtract 0.5 to you exert a small pressure on the ball.
  
  //  #define rotation_radius ((params.general.PLAYER_RADIUS * 0.707f) + 0.5f)
  //Works very well 8-4-2000
  const float ROTATION_RADIUS = .0990f;
  
  //Works well, but slow
  //#define Spin_Around_Ball_Threshhold (PI/20.0f)
  
  //Works well, lightening fast =-)
  //#define Spin_Around_Ball_Threshhold (PI/8.0f)
  
  const float SPIN_AROUND_BALL_THRESHHOLD = PI/11.0f;
  
  ballLocation = getBallLocation(field);
  
  pivotPoint = frontOfRobot(thisRobot, 
                            field, 
                            params);
  
  if(!friendlyHasPossession(thisRobot, params)){
    pivotPoint = ballLocation;
    currentAngle = angleBetween(robotLocation, pivotPoint);
  }
  else {
    currentAngle = getRotation(thisRobot, field, params);
  }
  
  //================================================
  //Where we eventually want to go
  
  dx = (ultimateDestination.getX() - pivotPoint.getX());
  dy = (ultimateDestination.getY() - pivotPoint.getY());
  float distance = SQRT(dx*dx + dy*dy);
  float ratio = ROTATION_RADIUS / distance;
  rotatedDestination = Pair( pivotPoint.getX() - dx*ratio, pivotPoint.getY() - dy*ratio);

  //===================================================================
  //Find angle at that point
  float finalAngle = angleBetween(rotatedDestination, pivotPoint);

  float dAngle = angleDifference(finalAngle, currentAngle);
  
  if( dAngle > SPIN_AROUND_BALL_THRESHHOLD )
    finalAngle = currentAngle + SPIN_AROUND_BALL_THRESHHOLD;
  //If the difference in angle is greater than PI, go the opposite way
  else if (dAngle < -SPIN_AROUND_BALL_THRESHHOLD)
    finalAngle = currentAngle - SPIN_AROUND_BALL_THRESHHOLD;
  

  //Never send off the field
  constrainToField(params, &rotatedDestination);

  //Never go in goalie box
  if( nearTheirGoalieBox( rotatedDestination, params, params.general.PLAYER_RADIUS) ) {
    rotatedDestination.setX( params.field.OUR_GOALIE_BOX + (params.general.PLAYER_RADIUS) );
  }
  
  command->setPos(rotatedDestination);
  command->setVel(  0.0f, 0.0f );
  command->setRotation( finalAngle );
  command->setDribble(FAST_DRIBBLE);
  command->setVerticalDribble(FAST_V_DRIBBLE);

}


//--------------------------------------------------------------------------


//Dribbles the ball away from the wall
void dribbleBallAwayFromWall(const VisionData& field,
                             const SystemParameters& params,
                             //Identification of this robot
                             RobotIndex thisRobot,
                             Destination* command)
{
  Pair robotLocation;
  Pair ballLocation;
  Pair frontLocation;
  BallStuckStatus ballStuckStatus;
  
  //Determines how much away from the corner you should be...
  const float SPIN_CLEARANCE_MULTIPLIER = .011f;
  
  //Location of this robot
  robotLocation = getLocation(thisRobot, field, params);

  //Ball Location
  ballLocation = getBallLocation(field);
  
  //Get status on ball
  ballStuckStatus = ballStuck(field, params);
  
  float ballDistance = ballLocation.distanceTo(robotLocation);
  float ballFrontDistance = ballLocation.distanceTo(frontOfRobot(thisRobot, field, params));
  
  //if we are close enough to ball, turn on dribbler
  command->initialize();
  
  if(ballDistance < 0.22f){
  command->setDribble(FAST_DRIBBLE);
  }
  //-----------------------------------------
  //If you call this when I don't think it is 
  //stuck, I will just go to the ball
  if(ballStuckStatus == NOT_STUCK)
  {
    command->setPos(ballLocation);
    command->setVel( Pair() );
    command->setRotation( angleToBall(thisRobot, field, params) );
  }
  //Ball stuck on left or right wall
  else if((ballStuckStatus == STUCK_ON_LEFT) ||
          (ballStuckStatus == STUCK_ON_RIGHT))
  {
    //Do I have the ball?
    if( 
        friendlyHasPossession(thisRobot, params) || 
        (ballFrontDistance < .02f) || 
        ( 
          !field.isBallFound(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX) && 
          (ballFrontDistance < .04f) 
        ) || 
        isBallBetween(robotLocation,
                      Pair(params.field.THEIR_GOAL_LINE, 0.0f),
                      2*params.general.PLAYER_RADIUS,
                      field)
      )
    {
      //back up with the ball
      //brings the ball toward middle of field
      command->setPos( Pair(params.field.THEIR_GOAL_LINE - .070f,
                            params.field.LEFT_SIDE_LINE - .070f) );
      command->setVel( Pair() );

      if(ballStuckStatus == STUCK_ON_LEFT){
        command->setRotation( angleToBall(thisRobot, field, params) - PI/2);
      }
      else {//(ballStuckStatus == STUCK_ON_RIGHT)
        command->setRotation( angleToBall(thisRobot, field, params) + PI/2);
      }           
    }
    //No? Better go get it
    else
    {
      command->setXPos(ballLocation.getX() - params.general.PLAYER_RADIUS);           
      command->setXVel(0.0f);
      command->setRotation(0.0f);
      
      if(ballStuckStatus == STUCK_ON_LEFT)
      {
        command->setYPos( params.field.LEFT_SIDE_LINE - 6.60f );
        command->setYVel( 0.0f );

        //if in front of it, go behind it
        if(robotLocation.getX() > (ballLocation.getX()  - params.general.BALL_RADIUS))
        {
          command->setXPos(ballLocation.getX() - 2*params.general.PLAYER_RADIUS);
          command->setYPos(ballLocation.getY() - 2*params.general.PLAYER_RADIUS);
        }
      }
      else//(ballStuckStatus == STUCK_ON_RIGHT)
      {
        command->setYPos( params.field.RIGHT_SIDE_LINE + 6.60f );
        command->setYVel( 0.0f );
        //if in front of it, go behind it
        if(robotLocation.getX() > (ballLocation.getX()  - params.general.BALL_RADIUS))
        {
          command->setXPos(ballLocation.getX() - 2*params.general.PLAYER_RADIUS);
          command->setYPos(ballLocation.getY() - 2*params.general.PLAYER_RADIUS);
        }
      }
    }
  }

  
  //Ball stuck on top or bottom wall
  
  //Note: in theory, we should only thing the ball is stuck if it is. However
  //we decided that if the ball was in any corner (behind the goalie-box line, but left or
  //right of the goalie box, then we considered it stuck. Thus ball-stuck might be returning a
  //non 1,2,3,4 value since the ball isn't really stuck, but we want to handle this correctly.
  //thus the else below will be caught, and we test where the ball is in the internal if with a or.
  //In other words, it will dribble the ball back from the top or bottom walls if the ball
  //ever gets stuck on the top or bottom corners. Nuf said.
  
  else
  {
    //Do I have the ball?
    if(   friendlyHasPossession(thisRobot, params)
       || (ballFrontDistance < .25f) 
       || ( !field.isBallFound(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX)
           && (ballFrontDistance < .25f) )
       || (ballLocation.getX() < (robotLocation.getX() - params.general.PLAYER_RADIUS)) )
    {
      command->setVel( Pair() );
      
      //back up with the ball
      //brings the ball toward middle of field
      if(ballStuckStatus == STUCK_ON_THEIR_WALL)
      {
        //*********************************************************
        //If the ball is between us and the goal
        if(   (  (ballLocation.getY() > (robotLocation.getY() + params.general.PLAYER_RADIUS))
               &&(robotLocation.getY() < 0.0f) )

           || (  (ballLocation.getY() < (robotLocation.getY() - params.general.PLAYER_RADIUS))
               &&(robotLocation.getY() > 0.0f) ) )
        {
          command->setXPos(params.field.THEIR_GOAL_LINE - 
                           SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
          
          if(ballLocation.getY() > 0.0f) {
            command->setYPos(params.field.LEFT_GOAL_POST
                             + SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
          }
          else{
            command->setYPos(params.field.RIGHT_GOAL_POST
                             - SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
          }

          if(ballLocation.getY() > 0.0f){
            command->setRotation(-PI/2);
          }
          else {
            command->setRotation(PI/2);
          }

          if(ballLocation.getY() > 0.0f)
          {
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) < (-PI/4))
               &&(getRotation(thisRobot, field, params) > (-3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
          }
          else
          {
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) > (PI/4))
               &&(getRotation(thisRobot, field, params) < (3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
          }
        }
        
        //If the ball is not between us and the wall, then back up
        else if(ballLocation.getX() < (robotLocation.getX() - params.general.PLAYER_RADIUS))
        {
          command->setXPos(params.field.THEIR_GOAL_LINE - 4*params.general.PLAYER_RADIUS);
          
          if(ballLocation.getY() > 0.0f)
          {
            command->setYPos(params.field.LEFT_SIDE_LINE - 4*params.general.PLAYER_RADIUS);
            
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) < (-PI/4))
               &&(getRotation(thisRobot, field, params) > (-3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
          }
          else
          {
            command->setYPos(params.field.LEFT_SIDE_LINE + 4*params.general.PLAYER_RADIUS);
            
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) > (PI/4))
               &&(getRotation(thisRobot, field, params) < (3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }

          }
          
          command->setRotation(-PI);
        }
        //else spin to knock it out from the wall
        else
        {
          if(ballLocation.getY() > 0.0f)
          {
            command->setXPos(params.field.THEIR_GOAL_LINE - 
              SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
            
            command->setYPos(params.field.LEFT_SIDE_LINE - 
              SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
            
            command->setRotation(getRotation(thisRobot, field, params) - PI/3);
            
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) < (-PI/4))
               &&(getRotation(thisRobot, field, params) > (-3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
            
          }
          else
          {
            command->setXPos(params.field.THEIR_GOAL_LINE - 
              SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
            
            command->setYPos(params.field.RIGHT_SIDE_LINE + 
              SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
            
            command->setRotation(getRotation(thisRobot, field, params) + PI/3);
            
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) > (PI/4))
               &&(getRotation(thisRobot, field, params) < (3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
            
          }
        }
      }
      else
      {
        //If the ball is not between us and the wall, then back up
        if(ballLocation.getX() > (robotLocation.getX() + params.general.PLAYER_RADIUS))
        {
          command->setXPos(params.field.OUR_GOAL_LINE + 4*params.general.PLAYER_RADIUS);
          if(ballLocation.getY() > 0.0f)
            command->setYPos(params.field.LEFT_SIDE_LINE - 4*params.general.PLAYER_RADIUS);
          else
            command->setYPos(params.field.RIGHT_SIDE_LINE + 4*params.general.PLAYER_RADIUS);
          
          command->setRotation(0.0f);
        }
        else
        {
          if(ballLocation.getY() > 0.0f)
          {
            command->setXPos(params.field.OUR_GOAL_LINE + 
              SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
            
            command->setYPos(params.field.LEFT_SIDE_LINE - 
              SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
            
            command->setRotation(getRotation(thisRobot, field, params) - PI/3);
          }   
          else
          {
            command->setXPos(params.field.OUR_GOAL_LINE + 
              SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
            
            command->setYPos(params.field.RIGHT_SIDE_LINE +
              SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
            
            command->setRotation(getRotation(thisRobot, field, params) + PI/3);
          }
        }
      }
    }
    //No? Better go get it
    else
    {
      command->setVel(0.0f, 0.0f);
      if(ballStuckStatus == STUCK_ON_THEIR_WALL)
      {
        command->setRotation(0.0f);
        //command->setRotation(0);
        command->setXPos(params.field.THEIR_GOAL_LINE - 
          SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);

        if(ballLocation.getY() > 0.0f) {
          command->setYPos(params.field.LEFT_SIDE_LINE - 
            SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
        }
        else {
          command->setYPos(params.field.RIGHT_SIDE_LINE +
            SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
        }
        //*********************************************************
        //If the ball is behind us, then back up
        if(ballLocation.getX() < (robotLocation.getX() - params.general.PLAYER_RADIUS))
        {
          command->setXPos(params.field.THEIR_GOAL_LINE - 4*params.general.PLAYER_RADIUS);
          
          if(ballLocation.getY() > 0.0f)
          {
            command->setYPos(params.field.LEFT_SIDE_LINE - 4*params.general.PLAYER_RADIUS);
            
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) < (-PI/4))
               &&(getRotation(thisRobot, field, params) > (-3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
            
          }
          else
          {
            command->setYPos(params.field.LEFT_SIDE_LINE + 4*params.general.PLAYER_RADIUS);
            
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) > (PI/4))
               &&(getRotation(thisRobot, field, params) < (3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
            
          }
          
          command->setRotation(-PI);
        }
        //*********************************************************
        //If the ball is between us and the goal
        if(   (  (ballLocation.getY() > (robotLocation.getY()))
               &&(robotLocation.getY() < 0.0f) )
             
           || (  (ballLocation.getY() < (robotLocation.getY()))
               &&(robotLocation.getY() > 0.0f) ) )
        {
          command->setXPos(params.field.THEIR_GOAL_LINE - 
            SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
          
          if(ballLocation.getY() > 0.0f)
            command->setYPos(params.field.LEFT_GOAL_POST
                             + SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
          else
            command->setYPos(params.field.RIGHT_GOAL_POST
                             - SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
          
          if(robotLocation.getY() > 0.0f)
            command->setRotation(-PI/2);
          else
            command->setRotation(PI/2);
          
          if(ballLocation.getY() > 0.0f)
          {
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) < (-PI/4))
               &&(getRotation(thisRobot, field, params) > (-3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
            
          }
          else
          {
            //If we are pointing towards the middle section of the field then kick
            if(  (getRotation(thisRobot, field, params) > (PI/4))
               &&(getRotation(thisRobot, field, params) < (3*PI/4)) )
            {
              command->setKick(KICK_SHOT);
            }
           
          }
        }
      }
      else
      {
        command->setRotation(-PI);
        //command->setRotation(0);
        command->setXPos(params.field.OUR_GOAL_LINE + 
          SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
        if(ballLocation.getY() > 0.0f)
          command->setYPos(params.field.LEFT_SIDE_LINE - 
          SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
        else
          command->setYPos(params.field.RIGHT_SIDE_LINE +
          SPIN_CLEARANCE_MULTIPLIER*params.general.PLAYER_RADIUS);
      }
    }
  }
}










