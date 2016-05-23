/*
 * screen.cpp
 */

#include "screen.h"
#include "info/robot/robot.h"
#include "info/ball/ball.h"
#include "obstacle.h"
#include "math/angle.h"
#include "pass/pass.h"
#include "shoot/shoot.h"
#include "base_types.h"
#include "pass/forward_pass.h"

//Static variables used in Screen_Robot_On_Offense
//Get robots ID #'s =-)
static Pair scaredRobotLocation;
/*//Location of scared robot
static float Distances[5];
static int RobotID[5];
static float XDiff;
static float YDiff;
static int tempRobotID;
static float tempRobotDistance;
static location screenedEnemyLoc;
static float Scared_Screen_Slope;
//how far to stay away from the evil robot
#define NO_GOOD_ROBOT_TO_SCREEN 987654321.0f
*/
//stay this far away from the screenee
#define SCREENING_DISTANCE (params.general.OPPONENT_RADIUS + (2*params.general.PLAYER_RADIUS))

const float THRESHHOLD_DISTANCE = 75.0f;

static Pair destinationLoc;
static float dx;
static float dy;
static Pair myPosition;
static Obstacles theObstacles;

//Tells "this robot" to go to a position that will screen 
//"scared robot" as s/he moves up the field
void offensiveScreen(const VisionData& field,
                     const SystemParameters& params,
                     //Indentification of this robot
                     RobotIndex ID,
                     //Indentification of our friend we are trying to help out =-)
                     RobotIndex scaredRobot,
                     Destination* command)
{
  //Location of scared robot
  scaredRobotLocation = getLocation(scaredRobot, field, params);

  //------------------
  //Step 1.) Make array of obstacles and order by increasing distance from "scared robot"
  //------------------    
  //Make array of obstacles
  //obstacles 0-4 are enemy bots. 5-8 are friendly bots, ignore those.
  findObstacles(field,
                params,
                ID,
                theObstacles.getObstacleArray());

  //Ignore obstacles 5 through 8
  theObstacles.ignoreUs();

  theObstacles.ignoreLost();

  //Remove obstacles that are too far back or better, are too far up front (like their goalie)
  for(int i=0; i<NUM_PLAYERS_ON_TEAM; i++) {
    if(  (theObstacles.getObstacleLoc(i).getX()
          < (ballXLocation(field) - (4*params.general.PLAYER_RADIUS)))
       ||(theObstacles.getObstacleLoc(i).getX()
          > (params.field.THEIR_GOALIE_BOX - (2*params.general.PLAYER_RADIUS)))
      )
    {
      theObstacles.ignore(i);
    }
  }

  int screenedEnemy = theObstacles.findClosestObstacle(scaredRobotLocation, THRESHHOLD_DISTANCE);

  if(screenedEnemy != NO_SUITABLE_OBSTACLE)
  {
    Pair screenedEnemyLoc = theObstacles.getObstacleLoc(screenedEnemy);
    //Step 1.) Draw vector from that obstacle to scared robot
    //Step 2.) Offset like pivoting in pass reception off obstacle and face our friend 
    //         (actually functions to receive passes in worst case scenario)
    if(scaredRobotLocation.getX() - screenedEnemyLoc.getX() == 0)
    {
      destinationLoc.setX(screenedEnemyLoc.getX());
      destinationLoc.setY(screenedEnemyLoc.getY() - SCREENING_DISTANCE);
    }
    else
    {
      dx = ABS(screenedEnemyLoc.getX() - scaredRobotLocation.getX());
      dy = ABS(screenedEnemyLoc.getY() - scaredRobotLocation.getY());

      static float ratio;
      ratio = SCREENING_DISTANCE/SQRT( (dx*dx) + (dy*dy) );

      //Shift X to be in front of the bad guy
      if(screenedEnemyLoc.getX() > scaredRobotLocation.getX())
        destinationLoc.setX( screenedEnemyLoc.getX() - (ratio*dx) );
      else
        destinationLoc.setX( screenedEnemyLoc.getX() + (ratio*dx) );

      //Shift Y to be in front of the bad guy
      if(screenedEnemyLoc.getY() > scaredRobotLocation.getY())
        destinationLoc.setY( screenedEnemyLoc.getY() - (ratio*dy) );
      else
        destinationLoc.setY( screenedEnemyLoc.getY() + (ratio*dy) );
    }
  }
  //Else, just move along behind the friendly robot
  else
  {
    destinationLoc.setX( scaredRobotLocation.getX() - (8*params.general.PLAYER_RADIUS) );

    //find which width is larger, go to that side
    float widthLeft, widthRight;
    if(scaredRobotLocation.getY() > 0.0f)
    {
      widthLeft = params.field.LEFT_SIDE_LINE - scaredRobotLocation.getY();
      widthRight = scaredRobotLocation.getY();

      if(widthLeft > widthRight)
        destinationLoc.setY( params.field.LEFT_SIDE_LINE - (2*params.general.PLAYER_RADIUS) );
      else
        destinationLoc.setY( 0.0f );
    }
    else
    {
      widthLeft = -scaredRobotLocation.getY();
      widthRight = scaredRobotLocation.getY() - params.field.RIGHT_SIDE_LINE;

      if(widthLeft > widthRight)
        destinationLoc.setY( 0.0f );
      else
        destinationLoc.setY( params.field.RIGHT_SIDE_LINE + (2*params.general.PLAYER_RADIUS) );
    }

    //ok, so we will be looking towards the scared robot, kinda behind him, going along
    //his side. cool, but if he is too far back, we don't want to get
    //in the way of the single defender et al so crop the back limit to 0.0f (x>=0).
    if(destinationLoc.getX() < 0.0f)
      destinationLoc.setX( 0.0f );
    //Also, crop the front limit so that we never go into their goalie box -- always
    //stay say 2 player radii behind the goalie box so no questions need to
    //be asked after we kick their goalies butt. =-)
    if(destinationLoc.getX() > (params.field.THEIR_GOALIE_BOX - (2*params.general.PLAYER_RADIUS)))
      destinationLoc.setX( params.field.THEIR_GOALIE_BOX - (2*params.general.PLAYER_RADIUS) );
  }
  //----------------------------------------------------------------------------
  //If the destination is inside the enemies goal box, move it out!
  //Goalie Box
  if( nearTheirGoalieBox(destinationLoc, params, params.general.PLAYER_RADIUS) )
  {
    destinationLoc.setX( (params.field.THEIR_GOALIE_BOX - (2*params.general.PLAYER_RADIUS)) );
    //If closer to left, go to the left side
    if(getYLocation(params.general.TEAM, ID, field) > 0.0f)
      destinationLoc.setY( params.field.LEFT_GOALIE_BOX + params.general.PLAYER_RADIUS );
    else
      destinationLoc.setY( params.field.RIGHT_GOALIE_BOX - params.general.PLAYER_RADIUS );
  }
  //----------------------------------------------------------------------------
  //Step 3.) Never NEVER get in the way of the active player

  //We never want the screening robot to get in the way of the active player. Say the
  //screening robot is screening a bad guy, and the active guy comes up and is close to
  //the bad guy that was beeing screened. Or, say the bad guys just drives up towards
  //the active guy dribbling the ball up the field, you want the screening robot to 
  //maybe move behind and maybe to the other side. This is the WORST CASE SCENARIO. Why?
  //Because the two robots are moving right at each other, and if obstacle avoidence does
  //a bad job, this just mucks everything up. Also, it is dumb to be getting in the way,
  //especially the role the screening robot is doing is absolutely worthless in comparison
  //to the active player who has top priority to dribble up the field.
  //
  //Solution: instead of having the screening robot move ACROSS the active player, 
  //just move him back. This will keep him out of the way until he is significantly 
  //behind the active player.

  myPosition = getLocation(ID, field, params);        

  //Moving 1->2->3
  //
  //   1                                 1      
  //    2             -> better if            
  //     *                                   *
  //       3                             2     3 
  if(  (myPosition.getY() >= scaredRobotLocation.getY())
     &&(scaredRobotLocation.getY() >= destinationLoc.getY())
     &&(myPosition.getX() >= (scaredRobotLocation.getX() - (2*params.general.PLAYER_RADIUS)))
     &&(destinationLoc.getX() <= scaredRobotLocation.getX())
     )
  {
    destinationLoc.setY( (scaredRobotLocation.getY() + (6*params.general.PLAYER_RADIUS)) );
  }
  //Moving 1->2->3
  //
  //        1                                 1      
  //      2          -> better if            
  //     *                                  *
  //   3                                 3    2
  if(  (myPosition.getY() <= scaredRobotLocation.getY())
     &&(scaredRobotLocation.getY() <= destinationLoc.getY())
     &&(myPosition.getX() >= (scaredRobotLocation.getX() - (2*params.general.PLAYER_RADIUS)))
     &&(destinationLoc.getX() <= scaredRobotLocation.getX())
    )
  {
    destinationLoc.setY( (scaredRobotLocation.getY() - (6*params.general.PLAYER_RADIUS)) );
  }

  //------------------------------
  //Moving 1->2->3
  //
  //    1                                2   1      
  //    2             -> better if            
  //     *                                   *
  //  3                                  3 
  if(  (myPosition.getY() >= scaredRobotLocation.getY())
     &&(scaredRobotLocation.getY() <= destinationLoc.getY())
     &&(myPosition.getX() >= (scaredRobotLocation.getX() - (2*params.general.PLAYER_RADIUS)))
     &&(destinationLoc.getX() <= scaredRobotLocation.getX())
     &&((myPosition.getY() - scaredRobotLocation.getY()) <= (4*params.general.PLAYER_RADIUS))
    )
  {
    destinationLoc.setX( (scaredRobotLocation.getX() + (4*params.general.PLAYER_RADIUS)) );
  }
  //Moving 1->2->3
  //
  //     1                                   1  2      
  //     2             -> better if            
  //    *                                   *
  //        3                                   3 
  if(  (myPosition.getY() <= scaredRobotLocation.getY())
     &&(scaredRobotLocation.getY() >= destinationLoc.getY())
     &&(myPosition.getX() >= (scaredRobotLocation.getX() - (2*params.general.PLAYER_RADIUS)))
     &&(destinationLoc.getX() <= scaredRobotLocation.getX())
     &&((scaredRobotLocation.getY() - myPosition.getY()) <= (4*params.general.PLAYER_RADIUS))
    )
  {
    destinationLoc.setX( (scaredRobotLocation.getX() + (4*params.general.PLAYER_RADIUS)) );
  }

  //----------------------------------------------------------------------------
  //Step 6.) Send robot to that position. =-)
  command->initialize();  //destination velocity = 0
  command->setPos(destinationLoc);
  command->setRotation( angleToBall(destinationLoc, field) );

}


/*

//Static variables used in Intersect_Fast_Moving_Ball
//Make array of obstacles
//Make array of just enemy obstacles
//Other robots also trying to get forward passes
static float X_Position;
static float Y_Position;

*/

//static RobotIndex thisRobotID;
static Pair enemyObstacles[NUM_ROBOTS];
//static Obstacles theObstacles;
static Pair ballPosition;
static Box FieldDim;
static Pair otherRobots[NUM_ROBOTS-1];
static Pair endpoint;
static Pair thisRobotLocation;
static float distanceFromDestination;
static float optdistance;

//-----------------------------------------------------------------------------------
//Use this algorithm to send robots into forward-pass lanes and also screen opponents
//-----------------------------------------------------------------------------------
void defensiveScreen(const VisionData& field, 
                     RobocupStrategyData* sd,
                     //Identification of this robot
                     RobotIndex ID,
                     //ID's for other friends
                     //also going into pass lanes
                     const RobotIndex otherFriendlyRobots[4],
                     Destination* command
                     )
{
  const SystemParameters& params = sd->getSystemParams();
  //Ball Position
  ballPosition = getBallLocation(field);

  //Make array of obstacles
  findObstacles(field,
                params,
                ID,
                theObstacles.getObstacleArray());

  //Make array of just enemy obstacles
  for(int i=0; i<5; i++)
  {
    enemyObstacles[i].set( theObstacles.getObstacleLoc(i) );
  }

  //This robot's position
  thisRobotLocation = getLocation(ID, field, params);

  //Field
  FieldDim.setLeft(params.field.LEFT_SIDE_LINE);
  FieldDim.setRight(params.field.RIGHT_SIDE_LINE);
  FieldDim.setBottom(params.field.OUR_GOAL_LINE);
  FieldDim.setTop(params.field.THEIR_GOAL_LINE);

  //Other robots also trying to get forward passes
  int numberOfAssigningRobots = 1; //by default until we find others
  for(i=0; i<4; i++)
  {   
    if(otherFriendlyRobots[i] == UNINITIALIZED)
    {
      otherRobots[i].setX( (float)UNINITIALIZED );
      otherRobots[i].setY( (float)UNINITIALIZED );
    }
    else
    {
      numberOfAssigningRobots++; //we just found one more!
      otherRobots[i] = getLocation(otherFriendlyRobots[i], field, params);
    }
  }
 
	//Optimal pass distance
	optdistance = 0.40f;//~45 inches

  //ok, figure out where we are going to go =-)
  findForwardPassLane(field, 
                      sd, 
                      ballPosition,
                      theObstacles.getObstacleArray(),
                      optdistance,
                      thisRobotLocation,
                      &endpoint);

  //Set position! =-)
  command->setXPos( endpoint.getX());
  command->setYPos( endpoint.getY());
  command->setRotation( angleToBall(endpoint, field) );


  //-------------------------------------------------
  //Set the pass_position and value so that the passer
  //knows where we are going and if he can pass to us
  sd->setPassDest(ID, Pair(endpoint.getX(), endpoint.getY()) );

  distanceFromDestination = thisRobotLocation.distanceTo(endpoint);
  if( //If I can see the ball...
     isLane(thisRobotLocation, ballPosition, params.general.PLAYER_RADIUS, field, params, false) &&

    //..and I'm near my destination
    //16 inches seems like a good number so 16*2.54 = 
      (distanceFromDestination <=  0.4064)
    )
  {
    sd->setPassValue(ID, 1);
  } else {
    sd->setPassValue(ID, 0);
  }
  //-------------------------------------------------
  
}
