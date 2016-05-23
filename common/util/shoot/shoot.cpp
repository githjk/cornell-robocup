/*
 *  SHOOT.CPP
 */

#include "shoot.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "../datatypes/robocup_strategy_Data.h"
#include "math/angle.h"
#include "lane/lane.h"
#include "GUI/GUI.h"


/****************Begin Lane Determination Algorithms***********************/
//===================================================================================


static float m, b;
static float xA, yA;
static float xBot, yBot;

// Determines if there is an obstacle-free lane between robotLoc and point
// within a path of width 2*bufferHalfWidth, while ignoring a robot of ours with
// position ignore.
// If there is, return true, else, return false
bool isLane(
        Pair robotLoc,           // Position of the robot.
        const Pair &point,       // location of the point
        const Pair &ignore,      // point to ignore
        float bufferHalfWidth,   // one-half the width of the lane
        const VisionData &field, // where everyone else is now
        const SystemParameters &p,     // contains TEAM, FRAME_RATE, wall locations, etc
        bool addPlayerRadiusToBuf// set this true to behave like 2000's isShot()
)
{
  xBot = robotLoc.getX();
  yBot = robotLoc.getY();
  xA = point.getX();
  yA = point.getY();

  float friendlyBufferHalfWidth = bufferHalfWidth;
  float enemyBufferHalfWidth = bufferHalfWidth;
  if(addPlayerRadiusToBuf){
    friendlyBufferHalfWidth += p.general.PLAYER_RADIUS;
    enemyBufferHalfWidth += p.general.OPPONENT_RADIUS;
  }

  if(xA == xBot) {
  // We are trying to shoot from their wall
    xA += 0.0001f;
  }

  m = (yA - yBot)/(xA - xBot);
  b = yBot - xBot*m;
  float xLowerBound = MIN(xBot, xA);
  float xUpperBound = MAX(xBot, xA);
  Pair other;

  //Check to make sure no robots are in the way (excluding passer and receiver)
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    //If friendly robot is found, check to see if it is in the way
    if(robotFound(i, field, p))
    {
      other = field.getRobot(p.general.TEAM, i).getPos();

      //Check to see if any of our friends are in the way
      if ((xLowerBound < other.getX())  //Use strict inequality (rather than <=)
        &&(xUpperBound > other.getX())  // to ensure that robotLoc itself isn't considered an obstacle
        &&(distFromPointToLine(m, b, other) < friendlyBufferHalfWidth)
        &&(ignore != other)){
            return false;
      }
      
    }

    //If enemy robot is found, check to see if it is in the way
    if (theirRobotFound(i, field, p))
    {
      other = field.getRobot(p.general.OTHER_TEAM, i).getPos();

      //Check to see if any of our enemies are in the way
      if ((xLowerBound < other.getX())  //Use strict inequality (rather than <=)
        &&(xUpperBound > other.getX())  // to ensure that robotLoc itself isn't considered an obstacle
        &&(distFromPointToLine(m, b, other) < enemyBufferHalfWidth)
        &&(ignore != other)){
            return false;
      }
    }
  }

  return true;
}
//===================================================================================
bool getBlockingRobot(
        Pair robotLoc,           // Position of the robot.
        const Pair &point,       // location of the point
        float bufferHalfWidth,   // one-half the width of the lane
        const VisionData &field, // where everyone else is now
        const SystemParameters &p,     // contains TEAM, FRAME_RATE, wall locations, etc
        RobotIndex &blockerID,
        int &team
        )
{
  xBot = robotLoc.getX();
  yBot = robotLoc.getY();
  xA = point.getX();
  yA = point.getY();

  float friendlyBufferHalfWidth = bufferHalfWidth;
  float enemyBufferHalfWidth = bufferHalfWidth;
    friendlyBufferHalfWidth += p.general.PLAYER_RADIUS;
    enemyBufferHalfWidth += p.general.OPPONENT_RADIUS;

  if(xA == xBot) {
  // We are trying to shoot from their wall
    xA += 0.0001f;
  }

  m = (yA - yBot)/(xA - xBot);
  b = yBot - xBot*m;
  float xLowerBound = MIN(xBot, xA);
  float xUpperBound = MAX(xBot, xA);
  Pair other;

  //Check to make sure no robots are in the way (excluding passer and receiver)
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    //If friendly robot is found, check to see if it is in the way
    if(robotFound(i, field, p))
    {
      other = field.getRobot(p.general.TEAM, i).getPos();

      //Check to see if any of our friends are in the way
      if ((xLowerBound < other.getX())  //Use strict inequality (rather than <=)
        &&(xUpperBound > other.getX())  // to ensure that robotLoc itself isn't considered an obstacle
        &&(distFromPointToLine(m, b, other) < friendlyBufferHalfWidth)
        ){
        blockerID=i;
        team=p.general.TEAM;
        return false;
      }
      
    }

    //If enemy robot is found, check to see if it is in the way
    if (theirRobotFound(i, field, p))
    {
      other = field.getRobot(p.general.OTHER_TEAM, i).getPos();

      //Check to see if any of our enemies are in the way
      if ((xLowerBound < other.getX())  //Use strict inequality (rather than <=)
        &&(xUpperBound > other.getX())  // to ensure that robotLoc itself isn't considered an obstacle
        &&(distFromPointToLine(m, b, other) < enemyBufferHalfWidth)
        ){
        blockerID=i;
        team=p.general.OTHER_TEAM;
        return false;
      }
    }
  }

  return true;
}

//===================================================================================
bool isLane(
        Pair robotLoc,           // Position of the robot.
        const Pair &point,       // location of the point
        float bufferHalfWidth,   // one-half the width of the lane
        const VisionData &field, // where everyone else is now
        const SystemParameters &p,     // contains TEAM, FRAME_RATE, wall locations, etc
        bool addPlayerRadiusToBuf// set this true to behave like 2000's isShot()
) 
{
  //use an "ignore" value that will never correspond to an actual robot.
  Pair dummyIgnore(99999.0f, 99999.0f);
  return isLane(robotLoc, point, dummyIgnore, bufferHalfWidth, field, p, addPlayerRadiusToBuf);
}
//===================================================================================
bool goodCurrentShot(Pair robotLoc,
                     float robotRotation,
                     const VisionData &field,
                     const SystemParameters &p,
                     float minLaneWidth
                     ) {
  //find the y-value where the ball would intersect the goal line
  float distToGoalLine = p.field.THEIR_GOAL_LINE - robotLoc.getX();
  float yGoalIntersect = (float)TAN(robotRotation)*distToGoalLine + robotLoc.getY();
  
  
  Pair point(p.field.THEIR_GOAL_LINE,yGoalIntersect);
  
  // Is our current shot towards the goal?
  if (
       (yGoalIntersect < (p.field.LEFT_GOAL_POST - (3*p.general.BALL_RADIUS + minLaneWidth/2))) &&
       (yGoalIntersect > (p.field.RIGHT_GOAL_POST + (3*p.general.BALL_RADIUS + minLaneWidth/2))) &&
       ((robotRotation < PI/2) && (robotRotation > -PI/2)) 
     )
  {
    // Is there a clear lane?
    if (laneHalfWidth(robotLoc,point,field,p,true) > minLaneWidth )
      return true;

    if ( twoOpponentsInTheirBox(field, p ) )
      return true;
  }

  return false;
}


//===================================================================================
bool isShot(RobotIndex ID,           // ID of traveling robot
        const Pair& point,       // point we want to go to
        float bufferHalfWidth,   // one-half the width of the lane
        const VisionData &field, // where everyone else is now
        const SystemParameters &p      // contains TEAM, FRAME_RATE, wall locations, etc
)
{
  //use an "ignore" value that will never correspond to an actual robot.
  Pair dummyIgnore(99999.0f, 99999.0f);
  return isLane(getLocation(ID, field, p), point, dummyIgnore, bufferHalfWidth, field, p, true);
}

//===================================================================================
//Uses the ball radius as the buffer size.
bool isShot(Pair robotLoc,       // Position of the robot.
        const Pair &point,       // location of the point
        const Pair &ignore,      // point to ignore
        const VisionData &field, // where everyone else is now
        const SystemParameters &p      // contains TEAM, FRAME_RATE, wall locations, etc
)
{
  return isLane(robotLoc, point, ignore, p.general.BALL_RADIUS, field, p, true);
}

//===================================================================================
//not sure where this buffer size comes from - mth13
bool isPassLane(Pair robotLoc,       // Position of the passer.
        const Pair &target,       // location of the target
        const Pair &ignore,      // point to ignore
        const VisionData &field, // where everyone else is now
        const SystemParameters &p      // contains TEAM, FRAME_RATE, wall locations, etc
)
{
  return isLane(robotLoc, target, ignore, p.general.PLAYER_RADIUS + p.general.OPPONENT_RADIUS, field, p, false);
}


//===================================================================================
//===================================================================================

//Define for shot functions
#define LEFT   1
#define RIGHT  -1

//Returns true if there is a clear shot to the post closest to player
// and places target for shot in (x,y)
bool clearShotClosePost(RobotIndex ID,
            const VisionData &field,
            const SystemParameters& params,
            Pair* target
)
{
  if(getYLocation(ID, field, params) > 0.0f)
    return clearShotLeftPost(ID, field, params, target);
  else 
    return clearShotRightPost(ID, field, params, target);
}

//===================================================================================
//Returns true if there is a clear shot to far post from player
// and places target for shot in (x,y)
bool clearShotFarPost(RobotIndex ID,
            const VisionData &field,
            const SystemParameters& params,
            Pair* target
)
{
  if(getYLocation(ID, field, params) > 0.0f)
    return clearShotRightPost(ID, field, params, target);
  else 
    return clearShotLeftPost(ID, field, params, target);
}


//===================================================================================

static RobotIndex theirGoalie;
static float yRightTarget;
static float yLeftTarget;

//Returns true if there is a clear shot to the right
// and places target for shot in (x,y)
bool clearShotRightPost(RobotIndex ID,
            const VisionData &field,
            const SystemParameters& params,
            Pair* target
)
{

  theirGoalie = NO_ROBOT;

  yRightTarget = params.field.RIGHT_GOAL_POST;

  if(getTheirGoalie(field, params, &theirGoalie))
  {
    yLeftTarget = getTheirYLocation(theirGoalie, field, params) - params.general.OPPONENT_RADIUS;
  }
  else
    yLeftTarget = params.field.LEFT_GOAL_POST; 
  
  if(yRightTarget >= yLeftTarget)
    yLeftTarget = params.field.LEFT_GOAL_POST;

  Pair newTarget(params.field.THEIR_GOAL_LINE, (yRightTarget + yLeftTarget)/2);
  *target = newTarget;//Why don't we just store this in target?  I dunno

  return clearShot(RIGHT, ID, (yLeftTarget - yRightTarget), field, params, target);
  //return clearShot(RIGHT, ID, (yLeftTarget - yRightTarget), field, params, &newTarget);
}
//===================================================================================

// Returns true if there is a clear shot to the left post
// and places target for shot in (x,y)
bool clearShotLeftPost(RobotIndex ID,
            const VisionData &field,
            const SystemParameters& params,
            Pair* target
)
{

  theirGoalie = NO_ROBOT;
  yLeftTarget = params.field.LEFT_GOAL_POST;

  if(getTheirGoalie(field, params, &theirGoalie))
  {
    yRightTarget = getTheirYLocation(theirGoalie, field, params) + params.general.OPPONENT_RADIUS;
  }
  else  
    yRightTarget = params.field.RIGHT_GOAL_POST;
  
  if(yRightTarget >= yLeftTarget)
    yRightTarget = params.field.RIGHT_GOAL_POST; 
    
  Pair newTarget(params.field.THEIR_GOAL_LINE, (yRightTarget + yLeftTarget)/2);
  *target = newTarget;//Why don't we just store this in target?  I dunno

  return clearShot(LEFT, ID, (yLeftTarget - yRightTarget), field, params, target);
  //return clearShot(LEFT, ID, (yLeftTarget - yRightTarget), field, params, &newTarget);
}
//===================================================================================

//Returns true if there is a clear shot within a path of buffer width
// from the robot to x,y with preference to side_preference
// and places target for shot in (x,y)
bool clearShot(int sidePreference,  // -1=RIGHT, 1=LEFT
           RobotIndex ID,
           float buffer, 
           const VisionData &field, 
           const SystemParameters& params,
           Pair* target
)
{
  //A temporary target.  We don't modify *target until we like the changes
  Pair newTarget;

  //If ball cannot fit in path, there is no shot
  if(buffer < 2*params.general.BALL_RADIUS)
    return false;
    
  //else if there is a shot to (x,y) return true
  else if( isShot(ID, *target, buffer, field, params) )
    return true;
  
  else {
    //else try x to the sidePreference of target
    newTarget = *target;
    newTarget.setY(newTarget.getY() + sidePreference*buffer/4);
    if (clearShot(sidePreference, ID, buffer/2, field, params, &newTarget)) {
      *target = newTarget;
      return true;
    }
        
    //else try x opposite of sidePreference from the target
    newTarget = *target;
    newTarget.setY(newTarget.getY() - sidePreference*buffer/4);
    if(clearShot(sidePreference, ID, buffer/2, field, params, &newTarget)) {
      *target = newTarget;
      return true;
    }
  }

  //no shot
  return false;
}//===================================================================================


//Returns true if there is a clear shot within a path of buffer width
// from the robot to x,y
// and places target for shot in (x,y)
bool clearShot(RobotIndex ID,
           const VisionData &field, 
           const SystemParameters& params,
           Pair* target
)
{
    
  if (getYLocation(ID, field, params) < 0.0f) {
    return (
        clearShotRightPost(ID, field, params, target)
        ||
        clearShotLeftPost(ID, field, params, target)
        );
  } else {
    return (
        clearShotLeftPost(ID, field, params, target)
        ||
        clearShotRightPost(ID, field, params, target)
       );
  }
}
//===================================================================================


//Returns true if there is a clear shot within a path of buffer width
// from the robot to x,y
bool clearShot(RobotIndex ID,
           const VisionData &field, 
           const SystemParameters& params
)
{
  Pair target;
  if (getYLocation(ID, field, params) < 0.0f) {
    return (
        clearShotRightPost(ID, field, params, &target)
        ||
        clearShotLeftPost(ID, field, params, &target)
        );
  } else {
    return (
        clearShotLeftPost(ID, field, params, &target)
        ||
        clearShotRightPost(ID, field, params, &target)
       );
  }

}

//==================================================================================
//Makes the robot move a little forward, just in case we have possession but the ball
//is not really touching the dribbler... and then shoots!
void moveForwardAndShoot(RobotIndex ID, const VisionData& field, RobocupStrategyData* sd,
                   Destination* command, KickSpeed kick) {

  command->setKick(kick);
  command->setControl(OMNI_NO_AVOID);
  command->setPos(frontOfRobot(ID,field,sd->getSystemParams()));
  command->setRotation(getRotation(ID,field,sd->getSystemParams()));

}


//==================================================================================
//This function tries to find the best possible shot on goal.  It 
//returns true if the shot lane width is greater than a threshold.
//The target is returned through the reference &target.
bool findShot(RobotIndex ID, 
              const VisionData &field,
              const SystemParameters &rp, 
              Pair* target) 
{   
    Pair leftPost, rightPost;
  leftPost.set(rp.field.THEIR_GOAL_LINE, rp.field.LEFT_GOAL_POST);
  rightPost.set(rp.field.THEIR_GOAL_LINE, rp.field.RIGHT_GOAL_POST);
  
  //we will search the area in front of the goal for a good shot
  const int numSearchPoints = 20;
    
  //Start looking at shots from left to right
    float leftShot = ABS(rp.field.LEFT_GOAL_POST - (2*rp.general.BALL_RADIUS));
    
  //Distance between each shot that we consider
    float step = (leftShot+ABS(rp.field.RIGHT_GOAL_POST)-(2*rp.general.BALL_RADIUS)) / numSearchPoints;
    
  Pair tempTarget;
    float tempWidth, distToPost, maxWidth = 0.0f;

    //Consider the targets from left to right.  Also consider goal posts as obstacles
    for(int i=0; i<=numSearchPoints; ++i) {
        //Consider the next target
        tempTarget.set( rp.field.THEIR_GOAL_LINE, leftShot - (i*step) );
        tempWidth = laneHalfWidth(getLocation(ID,field,rp),tempTarget,field,rp);
    
    //Check our distance from left goal post
    distToPost = distFromPointToLine(getLocation(ID,field,rp), tempTarget, leftPost);
    if(distToPost < tempWidth)  tempWidth = distToPost;
    //Check our distance from right goal post
    distToPost = distFromPointToLine(getLocation(ID,field,rp), tempTarget, rightPost);
    if(distToPost < tempWidth)  tempWidth = distToPost;

    //Save the temp target if it is better than the current best we have
        if(tempWidth > maxWidth) {
            target->set(tempTarget);
            maxWidth = tempWidth;
        }
    }
    
    //Found a good shot if the width is greater than the threshold
    if(maxWidth < rp.strategy2002.SHOOT_LANE_THRESH) 
    return false;
  else
  {
//    printf("Target: X:%f\tY:%f\n",target->getX(),target->getY());
    return true;
  }
}
//==================================================================================
//This function tries to calculate, rather than approximate, the best possible shot on goal.  It 
//returns true if the shot lane width is greater than a threshold.
//The target is returned through the reference &target.
bool calcShot(RobotIndex ID, 
              float line_X, 
              float lane_width, 
              float right_bound, 
              float left_bound, 
              RobotIndex ignoreID,
              const VisionData &field,
              const SystemParameters &rp, 
              Pair* target)
{   
  
  Pair robotLoc=getLocation(ID,field,rp);
  
  return calcShot(robotLoc,line_X,lane_width,right_bound,left_bound,ignoreID,
              field,
              rp, 
              target);
  
}
//==================================================================================
//This function tries to calculate, rather than approximate, the best possible shot on goal.  It 
//returns true if the shot lane width is greater than a threshold.
//The target is returned through the reference &target.
bool calcShot(Pair robotLoc, 
              float line_X, 
              float lane_width, 
              float right_bound, 
              float left_bound, 
              RobotIndex ignoreID,
              const VisionData &field,
              const SystemParameters &rp, 
              Pair* target)
{   
  if(right_bound>left_bound) {
    float temp=left_bound;
    left_bound=right_bound;
    right_bound=temp;
  }
  //array to hold the values for each shadow cast.
  float shadow[NUM_ROBOTS *2][2];
  int team;

  for(RobotIndex id=ROBOT0; id<NUM_ROBOTS; id++){

    //do both teams
    for(int i=id;i<NUM_ROBOTS *2;i=i+NUM_ROBOTS){
      if(i <NUM_ROBOTS) team = rp.general.TEAM;
      else team=rp.general.OTHER_TEAM;
      //initialize values
      shadow[i][0]=shadow[i][1]=32000.0f;
      //calculate shadows
      if(robotFound(team,id,field,rp) && !(id == ignoreID && team == rp.general.TEAM)){
        findShadow(robotLoc,getLocation(team,id,field),line_X,rp,shadow[i][0],shadow[i][1]);
      }
    }
  }
  //printf("shadows found\n");
  //******find the largest open shot*******
  float max,min;
  float final[2*NUM_ROBOTS][2];
  int n=0;
  //first, go through the shadows that start to the right of the right goalpost, and find the one that
  // extends the farthest to the right.

  max=right_bound;
  for(int j=0;j<NUM_ROBOTS*2;j++){
    if(shadow[j][0] < right_bound){
      if(shadow[j][1] > max) max=shadow[j][1];
      shadow[j][0]=shadow[j][1]=32000.0f;//set to dead values when processed
    }
  }
  
  final[n][0]=-32000.0f;
  final[n][1]=max;
  n++;
  //printf("first shadow found\n");
  // now go through the array, finding the smallest right edge of a shadow, and add those
  //shadows into the final array.
  int minI=0;
  while(final[n-1][1] < left_bound){
    //find next shadow
    min=32000.0f;
    for(int k=0;k<NUM_ROBOTS*2;k++){
      if(shadow[k][0]<min){
        minI=k;
        min=shadow[k][0];
      }
    }
    
    if(shadow[minI][0] > left_bound)shadow[minI][0] = left_bound;

    //see if it overlaps the previous shadow
    if(shadow[minI][0] < final[n-1][1]){
      if(shadow[minI][1] > final[n-1][1]){
        final[n-1][1]=shadow[minI][1];
      }
    }else{
      //new final shadow
      final[n][0]=shadow[minI][0];
      final[n][1]=shadow[minI][1];
      n++;
    }
    shadow[minI][0]=shadow[minI][1]=32000.0f;

  }
  //printf("final shadows found");
  //ok, now that we have the final set of ordered shadows, left go through and find the gap with
  //the largest lane.
  max=-1.0f;
  Pair maxPoint;
  Pair tPoint;
  tPoint.setX(line_X);

  xBot = robotLoc.getX();
  yBot = robotLoc.getY();
  
  for(int x=1; x<n;x++){
    tPoint.setY((final[x-1][1] + final[x][0])/2); // find middle of gap
    min=laneHalfWidth(robotLoc,tPoint,field,rp,true);
    if(min>max){
      max=min;
      maxPoint=tPoint;
    }
  }
  //printf("returning...");
  target->set(maxPoint);
  if(max*2 > lane_width) return true;
  else return false;
}


//==================================================================================







//==================================================================================
//This function tries to calculate, rather than approximate, the best possible shot on goal.  It 
//returns true if the shot lane width is greater than a threshold.
//The target is returned through the reference &target.
bool calcYShot(RobotIndex ID, 
               float line_Y, 
               float lane_width, 
               float right_bound, 
               float left_bound, 
               RobotIndex ignoreID,
               const VisionData &field,
               const SystemParameters &rp, 
               Pair* target){
  float width;
  return calcYShot(ID,line_Y,lane_width, right_bound,left_bound,ignoreID,
               field,rp,target,&width);
}
//==================================================================================
//This function tries to calculate, rather than approximate, the best possible shot on goal.  It 
//returns true if the shot lane width is greater than a threshold.
//The target is returned through the reference &target.
bool calcYShot(RobotIndex ID, 
               float line_Y, 
               float lane_width, 
               float right_bound, 
               float left_bound, 
               RobotIndex ignoreID,
               const VisionData &field,
               const SystemParameters &rp, 
               Pair* target,
               float* lane_Half_Width)
{   
  Pair robotLoc=getLocation(ID,field,rp);
  
  return calcYShot(robotLoc,line_Y,lane_width,right_bound,left_bound,ignoreID,
              field,
              rp, 
              target,
              lane_Half_Width);
  
}
//==================================================================================
//This function tries to calculate, rather than approximate, the best possible shot on goal.  It 
//returns true if the shot lane width is greater than a threshold.
//The target is returned through the reference &target.
bool calcYShot(Pair robotLoc, 
               float line_Y, 
               float lane_width, 
               float right_bound, 
               float left_bound, 
               RobotIndex ignoreID,
               const VisionData &field,
               const SystemParameters &rp, 
               Pair* target)
{     
  float width;
  return calcYShot(robotLoc,line_Y,lane_width, right_bound,left_bound,ignoreID,
               field,rp,target,&width);
}

//==================================================================================
//This function tries to calculate, rather than approximate, the best possible shot on goal.  It 
//returns true if the shot lane width is greater than a threshold.
//The target is returned through the reference &target.
bool calcYShot(Pair robotLoc, 
               float line_Y, 
               float lane_width, 
               float right_bound, 
               float left_bound, 
               RobotIndex ignoreID,
               const VisionData &field,
               const SystemParameters &rp, 
               Pair* target,
               float* lane_Half_Width)
{     
  if(right_bound>left_bound) {
    float temp=left_bound;
    left_bound=right_bound;
    right_bound=temp;
  }

  //array to hold the values for each shadow cast.
  float shadow[NUM_ROBOTS *2][2];
  int team;

  for(RobotIndex id=ROBOT0; id<NUM_ROBOTS; id++){

    //do both teams
    for(int i=id;i<NUM_ROBOTS *2;i=i+NUM_ROBOTS){
      if(i <NUM_ROBOTS) team = rp.general.TEAM;
      else team=rp.general.OTHER_TEAM;
      //initialize values
      shadow[i][0]=shadow[i][1]=32000.0f;
      //calculate shadows
      if(robotFound(team,id,field,rp) && !(id == ignoreID && team == rp.general.TEAM)){
        findShadowY(robotLoc,getLocation(team,id,field),line_Y,rp,shadow[i][0],shadow[i][1]);
        

      }
    }
  }
  //printf("shadows found\n");
  //******find the largest open shot*******
  float max,min;
  float final[2*NUM_ROBOTS][2];
  int n=0;
  //first, go through the shadows that start to the right of the right goalpost, and find the one that
  // extends the farthest to the right.

  max=right_bound;
  for(int j=0;j<NUM_ROBOTS*2;j++){
    if(shadow[j][0] < right_bound){
      if(shadow[j][1] > max) max=shadow[j][1];
      shadow[j][0]=shadow[j][1]=32000.0f;//set to dead values when processed
    }
  }
  
  final[n][0]=-32000.0f;
  final[n][1]=max;
  n++;
  //printf("first shadow found\n");
  // now go through the array, finding the smallest right edge of a shadow, and add those
  //shadows into the final array.
  int minI=0;
  while(final[n-1][1] < left_bound){
    //find next shadow
    min=32000.0f;
    for(int k=0;k<NUM_ROBOTS*2;k++){
      if(shadow[k][0]<min){
        minI=k;
        min=shadow[k][0];
      }
    }
    
    if(shadow[minI][0] > left_bound)shadow[minI][0] = left_bound;

    //see if it overlaps the previous shadow
    if(shadow[minI][0] < final[n-1][1]){
      if(shadow[minI][1] > final[n-1][1]){
        final[n-1][1]=shadow[minI][1];
      }
    }else{
      //new final shadow
      final[n][0]=shadow[minI][0];
      final[n][1]=shadow[minI][1];
      n++;
    }
    shadow[minI][0]=shadow[minI][1]=32000.0f;

  }
  //printf("final shadows found");
  //ok, now that we have the final set of ordered shadows, left go through and find the gap with
  //the largest lane.
  max=-1.0f;
  Pair maxPoint;
  Pair tPoint;
  tPoint.setY(line_Y);

  xBot = robotLoc.getX();
  yBot = robotLoc.getY();
  
  for(int x=1; x<n;x++){
    tPoint.setX((final[x-1][1] + final[x][0])/2); // find middle of gap
    min=laneHalfWidth(robotLoc,tPoint,field,rp,true);
    if(min>max){
      max=min;
      maxPoint=tPoint;
    }
  }
  //printf("returning...");
  target->set(maxPoint);
  *lane_Half_Width = max;
  if(max*2 > lane_width) return true;
  else return false;
}


//================================================================================
//used to calculate the X values for findShadowY
float calcX(float y, float m, float t,bool b){
  if (b){
    if (t > PI/2) return -32000.0f;
    else if (t < -PI/2) return 32000.0f;
  }else{
    if(ABS(t)<PI/2){
      if(t>0) return -32000.0f;
      else return 32000.0f;
    }
  }
  
  return y + m * TAN(t);
}


//================================================================================
//used to calculate the Y values for findShadow
float calcY(float y, float m, float t,bool b){
  if (b){
    if (t > PI/2) return -32000.0f;
    else if (t < -PI/2) return 32000.0f;
  }else{
    if(ABS(t)<PI/2){
      if(t>0) return -32000.0f;
      else return 32000.0f;
    }
  }
  
  return y + m * TAN(t);
}

//================================================================================
// Finds the "shadow" cast by one of our robots on a given line.  It returns the
// left and right points of that shadow, such that y1<y2
void findShadow(Pair ball, 
                Pair robot, 
                float lineX, 
                const SystemParameters &params, 
                float &y1, 
                float &y2)
{
  bool b=true;
  if (lineX > ball.getX()) b=false;
  if(ABS(lineX-ball.getX()) < ABS(ball.getX()-robot.getX())){
    y1=32000;
    y2=32000;
    return;
  }

 
  float rdist=ball.distanceTo(robot);
  if (rdist==0) return;
  float theta=ASIN(params.general.PLAYER_RADIUS/rdist);
  float base=angleBetween(robot, ball);
  if(b){
    y1=calcY(ball.getY(),(lineX-ball.getX()), base+theta,b);
    y2=calcY(ball.getY(),(lineX-ball.getX()), base-theta,b);
  }else{
    y1=calcY(ball.getY(),(lineX-ball.getX()), base-theta,b);
    y2=calcY(ball.getY(),(lineX-ball.getX()), base+theta,b);
  }
  //the following only happens if the robot is sitting behind the ball
  // and not covering anything, so this will shove the points off to the side.
  if (y1>y2) y1=y2;
}//==================================================================================
// Finds the "shadow" cast by one of our robots on a given horizontal line.  It returns the
// left and right points of that shadow, such that x1<x2
void findShadowY(Pair ball, 
                 Pair robot, 
                 float lineY, 
                 const SystemParameters &params, 
                 float &Y1, 
                 float &Y2)
{
  bool b=true;
  if (lineY > ball.getY()) b=false;
  if(ABS(lineY-ball.getY()) < ABS(ball.getY()-robot.getY())){
    Y1=32000;
    Y2=32000;
    return;
  }

 
  float rdist=ball.distanceTo(robot);
  if (rdist==0) return;
  float theta=ASIN(params.general.PLAYER_RADIUS/rdist);
  float base=-(angleBetween(robot, ball)-PI/2);
  if(b){
    Y1=calcY(ball.getX(),(lineY-ball.getY()), base+theta,b);
    Y2=calcY(ball.getX(),(lineY-ball.getY()), base-theta,b);
  }else{
    Y1=calcY(ball.getX(),(lineY-ball.getY()), base-theta,b);
    Y2=calcY(ball.getX(),(lineY-ball.getY()), base+theta,b);
  }
  //the following onlY happens if the robot is sitting behind the ball
  // and not covering anYthing, so this will shove the points off to the side.
  if (Y1>Y2) Y1=Y2;
}//==================================================================================

// This function finds the split angle between the two points and adds an offset to it if required
float findshootAngle(Pair pivotPoint, Pair point1, Pair point2, float bias)
{
    float deltaAngle,angle,baseAngle;

    baseAngle = angleBetween(pivotPoint,point1);
    deltaAngle = angleBetween(pivotPoint,point2) - angleBetween(pivotPoint,point1) ;
    angle = baseAngle + bias*deltaAngle;

    return angle;
}