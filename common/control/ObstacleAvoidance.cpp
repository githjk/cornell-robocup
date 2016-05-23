#include "ObstacleAvoidance.h"
#include "Trajectory.h"

#include "math/dist.h"

#include "float.h"

#include <windows.h>  // for the memcpy function

#include "GUI/GUI.h"

#define PATH_LENGTH MAX_TRAJECTORY_STEPS
#define NUM_TEMP_DESTINATIONS 40

#define IGNORE_COLLISION_FRAMES 20
#define MIN_COLLISION_FRAMES 10

//bookmark----
#define IGNORE_OPPONENTS_DURING_END_OF_PATH

//=====================================================================
// This static array will be built each frame.
//    It represents the predicted positions of each object
//    on the field during the next PATH_LENGTH steps

// Currently, it is using simple linear prediction based on
//    each robot's initial velocity
static VisionData predictionArray[PATH_LENGTH];

//=====================================================================

static float PLAYER_RADIUS;
static float OPPONENT_RADIUS;
static float BALL_RADIUS;

static float OUR_GOAL_LINE;
static float OUR_GOALIE_BOX;
static float THEIR_GOAL_LINE;
static float THEIR_GOALIE_BOX;

static float RIGHT_SIDE_LINE;
static float RIGHT_GOALIE_BOX;
static float LEFT_SIDE_LINE;
static float LEFT_GOALIE_BOX;

static float SLOW_CLEARANCE;
static float FAST_CLEARANCE;
static float FAST_CLEARANCE_VELOCITY;

static float LATENCY_FRAMES;

//=====================================================================
// UTILITY FUNCTION DECLARATIONS

bool checkPathForCollisions( ObjectPath thePath,
                                  int robotTeam,
                                  int robotIndex,
                                  float maxVelocity,
                                  int &team,
                                  int &index,
                                  int &frame );

void projectOntoField(ObjectPosVel& point);

//returns a boolean, true if point is off the field, false otherwise
bool offField(Pair point, float wallPushout)
{
	return ((point.getX() < OUR_GOAL_LINE   + wallPushout)
			||  (point.getX() > THEIR_GOAL_LINE - wallPushout)
			||  (point.getY() > LEFT_SIDE_LINE  - wallPushout)
			||  (point.getY() < RIGHT_SIDE_LINE + wallPushout));
}

//returns a boolean, true if point is off the field, false otherwise
bool offField(float x, float y, float wallPushout)
{
	return ((x < OUR_GOAL_LINE   + wallPushout)
			||  (x > THEIR_GOAL_LINE - wallPushout)
			||  (y > LEFT_SIDE_LINE  - wallPushout)
			||  (y < RIGHT_SIDE_LINE + wallPushout));
}

//=====================================================================
// This pre-calculates several variables used for obstacle avoidance
void updateObstacleAvoidance
(
  float playerRadius,
  float opponentRadius,
  float ballRadius,
  float ourGoalLine,
  float ourGoalieBox,
  float theirGoalLine,
  float theirGoalieBox,
  float rightSideLine,
  float rightGoalieBox,
  float leftSideLine,
  float leftGoalieBox,
  float slowClearance,
  float fastClearance,
  float fastClearanceVelocity,
  float latencyFrames
)
{

  PLAYER_RADIUS     = playerRadius;
  OPPONENT_RADIUS   = opponentRadius;
  BALL_RADIUS       = ballRadius;

  OUR_GOAL_LINE     = ourGoalLine;
  OUR_GOALIE_BOX    = ourGoalieBox;
  THEIR_GOAL_LINE   = theirGoalLine;
  THEIR_GOALIE_BOX  = theirGoalieBox;

  RIGHT_SIDE_LINE   = rightSideLine;
  RIGHT_GOALIE_BOX  = rightGoalieBox;

  LEFT_SIDE_LINE    = leftSideLine;
  LEFT_GOALIE_BOX   = leftGoalieBox;

  SLOW_CLEARANCE          = slowClearance;
  FAST_CLEARANCE          = fastClearance;
  FAST_CLEARANCE_VELOCITY = fastClearanceVelocity;

  LATENCY_FRAMES    = latencyFrames;
}

//=====================================================================
// This builds the static prediction array
void setupObstacleAvoidance(VisionData* currentField, float frameRate)
{
  // Build array of predicted obstacles
  memcpy(&predictionArray[0], currentField, sizeof(VisionData));

  float timeStep = 1.0f / frameRate;
  int i,j, k;

  // Predict each time step for PATH_LENGTH
  for (i=1; i < PATH_LENGTH; i++) {

    // Build array of predicted obstacles
    memcpy(&predictionArray[i], &predictionArray[i-1], sizeof(VisionData));

    // Predict ball
    if (predictionArray[i].isBallFound(0,0))
    {
      predictionArray[i].setBallXPos(0,0, 
        predictionArray[i].getBall(0,0).getXPos() + 
        timeStep*predictionArray[i].getBall(0,0).getXVel()
      );
      predictionArray[i].setBallYPos(0,0, 
        predictionArray[i].getBall(0,0).getYPos() + 
        timeStep*predictionArray[i].getBall(0,0).getYVel()
      );
    }

    for (j=0; j < NUM_PLAYERS_ON_TEAM; j++) {
      for (k=0; k < NUM_TEAMS; k++) {
  
        // Predict team robots
        if ( predictionArray[i].isRobotFound(k,j) )
        {
          predictionArray[i].setRobotXPos(k,j, 
            predictionArray[i].getRobot(k,j).getXPos() + 
            timeStep*predictionArray[i].getRobot(k,j).getXVel()
          );
          predictionArray[i].setRobotYPos(k,j, 
            predictionArray[i].getRobot(k,j).getYPos() + 
            timeStep*predictionArray[i].getRobot(k,j).getYVel()
          );

          //-----------------------------------------------------------------
          // Make sure robots don't get predicted off the field
          if ( predictionArray[i].getRobot(k,j).getXPos() > THEIR_GOAL_LINE - PLAYER_RADIUS )
          {
            predictionArray[i].setRobotXPos(k,j,THEIR_GOAL_LINE - PLAYER_RADIUS); 
            predictionArray[i].setRobotXVel(k,j,0.0f); 
          }
          else if ( predictionArray[i].getRobot(k,j).getXPos() < OUR_GOAL_LINE + PLAYER_RADIUS )
          {
            predictionArray[i].setRobotXPos(k,j,OUR_GOAL_LINE + PLAYER_RADIUS); 
            predictionArray[i].setRobotXVel(k,j,0.0f); 
          }

          if ( predictionArray[i].getRobot(k,j).getYPos() > LEFT_SIDE_LINE - PLAYER_RADIUS )
          {
            predictionArray[i].setRobotYPos(k,j,LEFT_SIDE_LINE - PLAYER_RADIUS); 
            predictionArray[i].setRobotYVel(k,j,0.0f); 
          }
          else if ( predictionArray[i].getRobot(k,j).getYPos() < RIGHT_SIDE_LINE + PLAYER_RADIUS )
          {
            predictionArray[i].setRobotYPos(k,j,RIGHT_SIDE_LINE + PLAYER_RADIUS); 
            predictionArray[i].setRobotYVel(k,j,0.0f); 
          }
          //-----------------------------------------------------------------

        }
      } // for (k=0; k < NUM_TEAMS; k++)
    } // for (j=0; j < NUM_PLAYERS_ON_TEAM; j++)

  } // for (i=1; i < PATH_LENGTH; i++)
}



const float biasConstant = 10.0f;
const float biasMultiplicator = 0.5f;

//=====================================================================
// Using the static prediction array, find a path from start
//    to final which will not hit any obstacles
void findOpenPath(ObjectPosVel start,
                  ObjectPosVel final,
                  int robotTeam,
                  int robotIndex,
                  float frameRate,
                  float maxAccel,
                  float maxThetaAccel,
                  float maxVelocity,
                  float maxThetaVelocity,
                  ObjectPath & thePath)
{
  float phi,newPhi,dphi,r,speed,bias,velocityAngle;

  //---------------------------------------------------------
  // Make sure the starting point is on the field
  projectOntoField(start);

  //---------------------------------------------------------
  // Build initial path
  zeroFinalVelocityPath(  start,
                          final,
                          frameRate,
                          maxAccel,
                          maxThetaAccel,
                          maxVelocity,
                          maxThetaVelocity,
                          thePath,
                          PATH_LENGTH );

  //---------------------------------------------------------
  // Look for collisions
  int team, index;
  int frame;

  bool collision = checkPathForCollisions(thePath,
                                          robotTeam,
                                          robotIndex,
                                          maxVelocity,
                                          team,
                                          index,
                                          frame );

  if (! collision) return;

  // If the start and end locations are identical, just return
  //WHAT if there is initial velocity??????????
  if ((start.xPos == final.xPos) && (start.yPos == final.yPos))
  {
    return;
  }

  //---------------------------------------------------------
  // Begin moving final destination point until we find a 
  // path with no collisions
  r = distance(start.xPos,start.yPos,final.xPos,final.yPos);
  dphi = 2*PI/NUM_TEMP_DESTINATIONS;

  //bias the search more towards the direction of the velocity if it is low
  speed = (float)sqrt(start.xVel*start.xVel+start.yVel*start.yVel);
  if(speed!=0.0f){
    bias = biasMultiplicator * (float)exp(-biasConstant*speed/maxVelocity);
    phi  = (float)atan2f(
      bias * start.yVel / speed+(1-bias)*(final.yPos-start.yPos)/r,
      bias * start.xVel / speed+(1-bias)*(final.xPos-start.xPos)/r
    );
  }
  else{
    phi = atan2f(final.yPos - start.yPos,final.xPos - start.xPos);
  }
    
  if ((start.yVel != 0.0f) || (start.yVel != 0.0f)) {
    velocityAngle = atan2f(start.yVel, start.xVel);
  }
  else {
    velocityAngle = 0.0f;
  }

  int angleDirection;

//  if ( (start.xVel*final.yPos) - (start.yVel*final.xPos) > 0.0f){
  if (velocityAngle<0){
    angleDirection=-1;
  }
  else
  {
    angleDirection=1;    
  }

  int count=1;
  collision = true;
  
  while(collision && count <= NUM_TEMP_DESTINATIONS)
  {
    // Flip angle direction
    angleDirection = -angleDirection;

    // Calculate new final destination
    newPhi = phi + angleDirection*( (count+1) / 2)*dphi;
    final.xPos = start.xPos + ( r * (float)cos(newPhi) );
    final.yPos = start.yPos + ( r * (float)sin(newPhi) );

    // Project the new final destination back on to
    // the field
    projectOntoField(final);

    zeroFinalVelocityPath( start,
                           final,
                           frameRate,
                           maxAccel,
                           maxThetaAccel,
                           maxVelocity,
                           maxThetaVelocity,
                           thePath,
                           PATH_LENGTH );

    // Check new path for collisions
    collision = checkPathForCollisions( thePath,
                                        robotTeam,
                                        robotIndex,
                                        maxVelocity,
                                        team,
                                        index,
                                        frame );

    count+=1;

  } //endwhile

  // If we can't find a path with no collisions,
  // just tell the robot to keep its current location
  //WHAT if we have initial velocity?
  if(collision)
  {   
    final.xPos = start.xPos;
    final.yPos = start.yPos; 

    zeroFinalVelocityPath( start,
                       final,
                       frameRate,
                       maxAccel,
                       maxThetaAccel,
                       maxVelocity,
                       maxThetaVelocity,
                       thePath,
                       1 );
  } //endif

}

//#define DRAW_DEBUG_PATH
static float BUFFER_TIME = 0.00f;

//=====================================================================
// Using the static prediction array, find a path from start
//    to final which will not hit any obstacles
void findTestOpenPath(ObjectPosVel start,
                      ObjectPosVel final,
                      int robotTeam,
                      int robotIndex,
                      float frameRate,
                      float maxAccel,
                      float maxThetaAccel,
                      float maxVelocity,
                      float maxThetaVelocity,
                      ObjectPath & thePath)
{
  //---------------------------------------------------------
  // Make sure the starting point is on the field
  projectOntoField(start);

  //---------------------------------------------------------
  // Build initial path
  zeroFinalVelocityPath(  start,
                          final,
                          frameRate,
                          maxAccel,
                          maxThetaAccel,
                          maxVelocity,
                          maxThetaVelocity,
                          thePath,
                          -1 );
//                          framesToStop );

  //---------------------------------------------------------
  // Look for collisions
  int team, index;
  int frame;

  bool collision = checkPathForCollisions(thePath,
                                          robotTeam,
                                          robotIndex,
                                          maxVelocity,
                                          team,
                                          index,
                                          frame );

  if (! collision) {

#ifdef DRAW_DEBUG_PATH
    // Draw debug path on screen
    ObjectPosVel step;
    int stepLength;
    for (int i=1; i <= 5; i++) {
      stepLength = PATH_LENGTH / 5;
      if ( thePath.get(i*stepLength - 1, step) )
        GUI_Record.debuggingInfo.setDebugPoint(robotIndex, i, step.xPos, step.yPos);
    }
#endif

    return;
  }

#ifdef DRAW_DEBUG_PATH
  Pair obstacle;
  obstacle = predictionArray[0].getRobot(team,index).getPos();
  GUI_Record.debuggingInfo.setDebugPoint(robotIndex, 19, obstacle.getX(), obstacle.getY());
#endif

  // If the start and end locations are identical, just return
  if ((start.xPos == final.xPos) && (start.yPos == final.yPos))
  {
    return;
  }

  float newPhi,dphi,r;
  float destinationAngle, velocityAngle;
  //---------------------------------------------------------
  // Begin moving final destination point until we find a 
  // path with no collisions
  r = distance(start.xPos,start.yPos,final.xPos,final.yPos);
  dphi = 2*PI/NUM_TEMP_DESTINATIONS;

  //---------------------------------------------------------
  // These angles are intentionally greater than 2*PI so that
  //    their magnitudes can be compared without worrying 
  //    about the flip between PI and -PI

  if ((start.xPos != final.xPos) || (start.yPos != final.yPos)) {
    destinationAngle = atan2f(final.yPos - start.yPos,final.xPos - start.xPos);
  }
  else {
    destinationAngle = 0.0f;
  }

  if ((start.xVel == 0.0f) && (start.yVel == 0.0f)) {
    velocityAngle = destinationAngle;
  }
  else {
    velocityAngle = atan2f(start.yVel, start.xVel);
  }
  
  //---------------------------------------------------------
  float angleDifference, angleDirection, angleOffset;

  angleDifference = normalizeAngle(velocityAngle - destinationAngle);

  if (angleDifference > 0.0f)
    angleDirection =  1.0f;
  else
    angleDirection = -1.0f;
  
  //---------------------------------------------------------
  // When this boolean becomes true, start alternating temp destination 
  //    points on either side of the original destination
  bool alternating = false;

  // This boolean toggles to determine which side of the original
  //    destination to look towards
  bool useAlternateDirection = false;

  // 'count' is the total number of steps checked
  // 'normalCount' is the number of steps checked towards the velocityAngle
  // 'alternateCount' is the number of steps checked in the opposite direction
  int count           = 0;
  int normalCount     = 0;
  int alternateCount  = 0;

  collision = true;

  //---------------------------------------------------------
  while(collision && count < NUM_TEMP_DESTINATIONS)
  {
    count++;

    // Calculate new final destination
    if (useAlternateDirection) {
      alternateCount++;
      
      angleOffset = angleDirection * alternateCount * dphi;
      newPhi = destinationAngle - angleOffset;
      
      final.xPos = start.xPos + ( r * (float)cos(newPhi) );
      final.yPos = start.yPos + ( r * (float)sin(newPhi) );
      useAlternateDirection = false;
    }
    else {
      normalCount++;

      angleOffset = angleDirection * normalCount * dphi;
      newPhi = destinationAngle + angleOffset;
      
      final.xPos = start.xPos + ( r * (float)cos(newPhi) );
      final.yPos = start.yPos + ( r * (float)sin(newPhi) );

      if (alternating) {
        useAlternateDirection = true;
      }
      else if ( ABS(angleOffset) > ABS(angleDifference) ) {
        alternating = true;
      }
 
    }

    // Project the new final destination back on to
    // the field
    projectOntoField(final);

    zeroFinalVelocityPath( start,
                           final,
                           frameRate,
                           maxAccel,
                           maxThetaAccel,
                           maxVelocity,
                           maxThetaVelocity,
                           thePath,
                           PATH_LENGTH );

    // Check new path for collisions
    collision = checkPathForCollisions( thePath,
                                        robotTeam,
                                        robotIndex,
                                        maxVelocity,
                                        team,
                                        index,
                                        frame );


  } //endwhile


#ifdef DRAW_DEBUG_PATH
  GUI_Record.debuggingInfo.setDebugPoint(robotIndex, 16, final.xPos, final.yPos);

  // Draw debug path on screen
  ObjectPosVel step;
  int stepLength;
  for (int i=1; i <= 5; i++) {
    stepLength = PATH_LENGTH / 5;
    if ( thePath.get(i*stepLength - 1, step) )
      GUI_Record.debuggingInfo.setDebugPoint(robotIndex, i, step.xPos, step.yPos);
  }
#endif

  // If we can't find a path with no collisions,
  // just tell the robot to keep its current location
  //WHAT if we have initial velocity?
  if(collision)
  {   
    final.xPos = start.xPos;
    final.yPos = start.yPos; 

    zeroFinalVelocityPath( start,
                       final,
                       frameRate,
                       maxAccel,
                       maxThetaAccel,
                       maxVelocity,
                       maxThetaVelocity,
                       thePath,
                       1 );


#ifdef DRAW_DEBUG_PATH
    char msg[80];
    sprintf(msg,"%d: Unable to find path", robotIndex);
    GUI_Record.debuggingInfo.addDebugMessage(msg);

    // Draw debug path on screen
    ObjectPosVel step;
    int stepLength;
    for (int i=1; i <= 5; i++) {
      stepLength = PATH_LENGTH / 5;
      if ( thePath.get(i*stepLength - 1, step) )
        GUI_Record.debuggingInfo.setDebugPoint(robotIndex, i, step.xPos, step.yPos);
    }
#endif



  } //endif

}


//=====================================================================
// Using the static prediction array, find a path from start
//    to final which will not hit any obstacles
void findFastOpenPath(ObjectPosVel start,
                      ObjectPosVel final,
                      int robotTeam,
                      int robotIndex,
                      float frameRate,
                      float maxAccel,
                      float maxThetaAccel,
                      float maxVelocity,
                      float maxThetaVelocity,
                      ObjectPath & thePath,
                      int numberOfSteps)
{
  fastPath( start,
            final,
            frameRate,
            maxAccel,
            maxVelocity,
            thePath,
            numberOfSteps );

  ObjectPosVel firstStep;

  if (thePath.get(1, firstStep))
  {
    firstStep.rotVel = 0.0f;
    thePath.set(1, firstStep);
  }
}

//=====================================================================
// UTILITY FUNCTIONS FOR OBSTACLE AVOIDANCE
//=====================================================================
bool checkPathForCollisions(ObjectPath thePath,
                            int robotTeam,
                            int robotIndex,
                            float maxVelocity,
                            int &team,
                            int &index,
                            int &frame )
{
  int numSteps = thePath.getNumSteps();

  ObjectPosVel nextStep;
  float collisionDistanceSquared, tempDistanceSquared;
  float minDistanceSquared = 1000.0f;

  float xPos, yPos;
  float xOffset, yOffset;

  float dotProduct;

  bool collision = false;
  float speed;
  float clearanceDistance;

  // If we are deccelerating, ignore opponents during the
  //    last IGNORE_STEPS of the path
  float initialSpeedSquared, nextSpeedSquared;
  int ignoreOpponentsFrame;

  // Calculate the initial speed of the robot
  thePath.get(0, nextStep);
  initialSpeedSquared = (float)sqrt(nextStep.xVel*nextStep.xVel+nextStep.yVel*nextStep.yVel);
  // Calculate the speed during the next step of the path
  thePath.get(1, nextStep);
  nextSpeedSquared = (nextStep.xVel*nextStep.xVel+nextStep.yVel*nextStep.yVel);

  ignoreOpponentsFrame = numSteps;

#ifdef IGNORE_OPPONENTS_DURING_END_OF_PATH
  if (nextSpeedSquared < initialSpeedSquared) {
    ignoreOpponentsFrame = 
      __max(MIN_COLLISION_FRAMES, numSteps - IGNORE_COLLISION_FRAMES);
  }
#endif

  // Check each step of the path for collisions
  for ( frame=1; frame < numSteps; frame = frame+2)
  {
    thePath.get(frame, nextStep);
  
    //----------------------------------------------------
    // Calculate the extra clearance distance based on the
    // robot's expected velocity
    speed = (float)sqrt(nextStep.xVel*nextStep.xVel+nextStep.yVel*nextStep.yVel);

    if (speed >= FAST_CLEARANCE_VELOCITY) {
      clearanceDistance = FAST_CLEARANCE;
    }
    else 
    {
      clearanceDistance = SLOW_CLEARANCE + (
                                             (FAST_CLEARANCE-SLOW_CLEARANCE) * 
                                             (speed/FAST_CLEARANCE_VELOCITY) *
                                             (speed/FAST_CLEARANCE_VELOCITY)
                                           );
    }
  
    //----------------------------------------------------
    // Check each team
    for (team = 0; team < NUM_TEAMS; team++) {
      
      //----------------------------------------------------
      // Calculate the distance that we need to avoid this team's robots
      // Square the distance to avoid calculating square roots
      if (team == robotTeam) {
        collisionDistanceSquared = (PLAYER_RADIUS + PLAYER_RADIUS + clearanceDistance)*(PLAYER_RADIUS + PLAYER_RADIUS + clearanceDistance);
      }
      else {
        // Ignore opponents for the last section of the path
        if (frame > ignoreOpponentsFrame) {
          continue;
        }
        collisionDistanceSquared = (PLAYER_RADIUS + OPPONENT_RADIUS + clearanceDistance)*(PLAYER_RADIUS + OPPONENT_RADIUS + clearanceDistance);
      }

      //----------------------------------------------------
      // Check for collisions with each robot
      for (index=0; index < NUM_PLAYERS_ON_TEAM; index++) {

        // Don't compare this robot with itself
        if ( (team == robotTeam) && (index == robotIndex) )
          continue;

        // Don't compare if the obstacle robot is lost
        if ( predictionArray[frame].isRobotFound(team,index) )
        {
          xPos = predictionArray[frame].getRobot(team,index).getXPos();
          yPos = predictionArray[frame].getRobot(team,index).getYPos();

          tempDistanceSquared = (xPos - nextStep.xPos)*(xPos - nextStep.xPos) +
                                (yPos - nextStep.yPos)*(yPos - nextStep.yPos);

          //----------------------------------------------------
          // Did a new collision occur?
          if (tempDistanceSquared < collisionDistanceSquared) {

            // Ignore the collision if this robot is moving away from the object
            xOffset = xPos - nextStep.xPos;
            yOffset = yPos - nextStep.yPos;

            dotProduct = nextStep.xVel*xOffset + nextStep.yVel*yOffset;

            if (dotProduct > 0.0f) {
              return true;
            }

          }

        }

      } // Check each robot

    } // Check each team

  } // Check each frame

  return false;
}

//=====================================================================
void projectOntoField(ObjectPosVel& point)
{

  if (point.xPos > THEIR_GOAL_LINE - PLAYER_RADIUS)
    point.xPos = THEIR_GOAL_LINE - PLAYER_RADIUS;

  else if (point.xPos < OUR_GOAL_LINE + PLAYER_RADIUS)
    point.xPos = OUR_GOAL_LINE + PLAYER_RADIUS;

  if (point.yPos > LEFT_SIDE_LINE - PLAYER_RADIUS)
    point.yPos = LEFT_SIDE_LINE - PLAYER_RADIUS;
  else if (point.yPos < RIGHT_SIDE_LINE + PLAYER_RADIUS)
    point.yPos = RIGHT_SIDE_LINE + PLAYER_RADIUS;

}
//==========================================================================================================

