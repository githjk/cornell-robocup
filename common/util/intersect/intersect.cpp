/*
 *  intersect.cpp
 */

#include "intersect.h"
#include "static_vars.h"
#include "info/ball/ball.h"
#include "math/angle.h"

//-------------------------------------------
//Static variables for findLineIntersection
//Where we will store our answer
static float slopeA;
static float slopeB;
static float yInterceptA;
static float yInterceptB;
//-------------------------------------------
//Finds the intersection between two lines
void findLineIntersection(const Line &A, 
                          const Line &B,
                          Pair *intersection)
{
  //==============================================
  //=== Case 1: neither line has a change in X ===
  //==============================================
  //If there is no change in x for both lines, 
  //either lines will NEVER or ALWAYS intersect.
  if(
     //No change in x for line A...
     (
      (
       ( A.getB() ).getX() - 
       ( A.getA() ).getX()  
      ) == 0
     ) &&
     //No change in x for line B...
     (
      (
       ( B.getB() ).getX() - 
       ( B.getA() ).getX()  
      ) == 0
     )
    )
  {
    //Ok, if their x values are equal, return 
    //intersection point as line A's point A.
    //Yes, this is a little arbitratry. But 
    //theoreticaly this section of code will almost
    //never be executed.
    if(
       ( A.getA() ).getX() ==
       ( B.getA() ).getX()
      )
    {
      intersection->setX( ( A.getA() ).getX() );
      intersection->setY( ( A.getA() ).getY() );
    }
    //Else lines will never intersect,
    //return pair (-32000,-32000)
    else
    {
      intersection->setX( NEVER_INTERSECT);
      intersection->setY( NEVER_INTERSECT);
    }       
  }

  //==============================================
  //Else, we know at least one of the lines 
  //does NOT have a slope of infinity!!!
  //==============================================

  //==============================================
  //=== Case 2: line A has no change in X      ===
  //==============================================
  //If line A has an infinite slope (no change in x)
  //we know line B does not have an infinite slope...
  else if(
          //No change in x for line A...
          (
           ( A.getB() ).getX() - 
           ( A.getA() ).getX()  
          ) == 0
         )         
  {
    slopeB = (
              (
               ( B.getB() ).getY() - 
               ( B.getA() ).getY()
              ) /
              (
               ( B.getB() ).getX() - 
               ( B.getA() ).getX()
              )
             );
   
   yInterceptB = (
                  B.getA().getY() - 
                  (slopeB * B.getA().getX())
                 );
   //y = mx+b
   intersection->setX( ( A.getB() ).getX() );
   intersection->setY(
                     ( slopeB * intersection->getX() ) +
                     ( yInterceptB)
                    );
  }
  //==============================================
  //=== Case 3: line B has no change in X      ===
  //==============================================
  //If line B has an infinite slope (no change in x)
  //we know line A does not have an infinite slope...
  else if(
          //No change in x for line B...
          (
           ( B.getB() ).getX() - 
           ( B.getA() ).getX()  
          ) == 0
         )         
  {
    slopeA = (
              (
               ( A.getB() ).getY() - 
               ( A.getA() ).getY()
              ) /
              (
               ( A.getB() ).getX() - 
               ( A.getA() ).getX()
              )
             );
   
   yInterceptA = (
                  A.getA().getY() - 
                  (slopeA * A.getA().getX())
                 );
   //y = mx+b
   intersection->setX( ( B.getB() ).getX() );
   intersection->setY(
                     ( slopeA * intersection->getX() ) +
                     ( yInterceptA)
                    );
  }
  //==================================================
  //=== Case 4: both lines have non infinit slopes ===
  //==================================================
  else
  {
    slopeA = (
              (
               ( A.getB() ).getY() - 
               ( A.getA() ).getY()
              ) /
              (
               ( A.getB() ).getX() - 
               ( A.getA() ).getX()
              )
             );
    
    slopeB = (
              (
               ( B.getB() ).getY() - 
               ( B.getA() ).getY()
              ) /
              (
               ( B.getB() ).getX() - 
               ( B.getA() ).getX()
              )
             );

   yInterceptA = (
                  A.getA().getY() - 
                  (slopeA * A.getA().getX())
                 );

   yInterceptB = (
                  B.getA().getY() - 
                  (slopeB * B.getA().getX())
                 );

   //y1 = mx1+b
   //y2 = nx2+c
   //at intersection y1=y2 and x1 = x2 so...
   //mx +b = nx + c
   //x(m-n) = c-b
   //x = (c-b)/(m-n)
   //where m and n are slope and
   //b and c are y-intercepts.
   //x = (c-b)/(m-n)

   intersection->setX(
                     (yInterceptB - yInterceptA) / 
                     (slopeA - slopeB)
                    );

   //y = mx + b
   intersection->setY(
                     (slopeA * intersection->getX()) +
                     yInterceptA
                    );
  }
  //Phew!! All done! =-)
}
//-------------------------------------------

//===============================================================
//===============================================================
//===============================================================
//===============================================================
void intersectFastMovingBall(const VisionData& field, 
							 const SystemParameters& params,
                             float xValueOfDefendingLine,
                             Destination* command)
{
	Line defenderLine, ballVelocityVector;
  //-------------------------------
  defenderLine.setAposX( xValueOfDefendingLine );
  defenderLine.setBposX( xValueOfDefendingLine );
  defenderLine.setAposY( -200.0f);
  defenderLine.setAposY( 200.0f);
  //-------------------------------
  ballVelocityVector.setA( getRobocupBall(field).getPos() );
  ballVelocityVector.setBposX(
                               getRobocupBall(field).getXPos() + 
                               (5.0f*getRobocupBall(field).getXVel()) 
                             );

  ballVelocityVector.setBposY(
                               getRobocupBall(field).getYPos() +
                               (5.0f*getRobocupBall(field).getYVel())
                             );

  Pair intersection;

  //find the intersection point for where we can best intercept the ball along our line
  findLineIntersection(defenderLine, 
                       ballVelocityVector, 
                       &intersection);

	//if we cannot find an intersection, just match the ball's position
	if(
	   (intersection.getX() == NEVER_INTERSECT) ||
	   (intersection.getY() == NEVER_INTERSECT)
	  )
		intersection.setY( getRobocupBall(field).getYPos() );

	//always set our x to be the x line we sent in.
	intersection.setX( xValueOfDefendingLine );

  //---------------------------------------------

	//If the interception point is off the field, or the ball is moving slowly, 
	//just follow the Y position of the ball
	if( offField(params, intersection) )
	{
		if(intersection.getY() > (params.field.LEFT_SIDE_LINE - params.general.PLAYER_RADIUS))
			intersection.setY( params.field.LEFT_SIDE_LINE - params.general.PLAYER_RADIUS );
		if(intersection.getY() < (params.field.RIGHT_SIDE_LINE + params.general.PLAYER_RADIUS))
			intersection.setY( params.field.RIGHT_SIDE_LINE + params.general.PLAYER_RADIUS );
	}

  //actually set the desination for good =-)
  command->setPos( intersection );

  //finally, set the rotation to look straight at the ball
  command->setRotation( angleToBall( command->getPos(), field) );
}
//===============================================================
//===============================================================
//===============================================================
//===============================================================
//----------------------------------------------------------------------
//Intersect the vector from ball->to middle of goal along defending line
//----------------------------------------------------------------------
void intersectBallGoal( const VisionData& field, 
                        const SystemParameters& params,
                        float xValueOfDefendingLine,
                        float goalLeft,	//sometimes we want to restrict
                        float goalRight,  //which part of the goal we will protect
                        Destination* command)
{
  //move back and forth along width of field on this line
  command->setXPos( xValueOfDefendingLine );

	//Difference in ball->middle of goal position
  //-------------------------------------------
	xDiff = ( (goalLeft + goalRight) / 2) - 
          ( getBallLocation(field).getX() );

	yDiff = ( params.field.OUR_GOAL_LINE - getBallLocation(field).getY() );
    
	//Difference in our X position and that of the ball
	xOther = ( command->getXPos() - getBallLocation(field).getX() );

	//Calculate intersection point along defensive line
	if(xOther == 0)
  {
    command->setYPos( getBallLocation(field).getY() );
  }
	else
  {
    command->setYPos( getBallLocation(field).getY() + ((yDiff/xDiff)*xDiff) );
  }
	//If destination is off the field, just follow the ball
	if( offField( params, command->getPos() ) )
  {
    command->setYPos( getBallLocation(field).getY() );
  }

  //finally, set the rotation to look straight at the ball
  command->setRotation( angleToBall( command->getPos(), field ) );
}

//===============================================================
//===============================================================
//===============================================================
//===============================================================
