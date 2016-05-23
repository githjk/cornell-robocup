/****************************
 *  SHOOT.H             *
 ****************************/
#ifndef SHOOT_H
#define SHOOT_H

#include <stdio.h>
#include <math.h>

#include "ai.h"
#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "VisionModule.h"
#include "../datatypes/robocup_strategy_Data.h"
#include "common/datatypes/SystemParameters.h"


// Determines if there is an obstacle-free lane between robotLoc and point
// within a path of width 2*bufferHalfWidth, while ignoring a robot of ours with
// position ignore.
// If there is, return true, else, return false
//
//Note:  This is somewhat different from the 2000 isLane.  You may need to tweak
// the buffer size.
bool isLane(
        Pair robotLoc,           // Position of the robot.
        const Pair &point,       // location of the point
        const Pair &ignore,      // point to ignore
        float bufferHalfWidth,   // one-half the width of the lane
        const VisionData &field, // where everyone else is now
        const SystemParameters &p,     // contains TEAM, FRAME_RATE, wall locations, etc
        bool addPlayerRadiusToBuf// set this true to behave like 2000's isShot()
);

bool isLane(
        Pair robotLoc,           // Position of the robot.
        const Pair &point,       // location of the point
        float bufferHalfWidth,   // one-half the width of the lane
        const VisionData &field, // where everyone else is now
        const SystemParameters &p,     // contains TEAM, FRAME_RATE, wall locations, etc
        bool addPlayerRadiusToBuf// set this true to behave like 2000's isShot()
);

bool getBlockingRobot(
        Pair robotLoc,           // Position of the robot.
        const Pair &point,       // location of the point
        float bufferHalfWidth,   // one-half the width of the lane
        const VisionData &field, // where everyone else is now
        const SystemParameters &p,     // contains TEAM, FRAME_RATE, wall locations, etc
        RobotIndex &blockerID,
        int &team
);

// Returns true if the robot has a clear shot to the goal in the direction it is facing
//   Assumes that the robot has the ball
bool goodCurrentShot(Pair robotLoc,             // current robot position
                     float robotRotation,       // current robot orientation
                     const VisionData &field,   // vision info -- all other robot positions
                     const SystemParameters &p,// parameter file
                     float minLaneWidth = 0.05f
);


bool isShot(Pair robotLoc,       // Position of the robot.
        const Pair& point,       // point we want to go to
        float bufferHalfWidth,   // one-half the width of the lane
        const VisionData &field, // where everyone else is now
        const SystemParameters &p      // contains TEAM, FRAME_RATE, wall locations, etc
);

// calls isLane with addPlayerRadiusToBuf = true, bufferHalfWidth = p.BALL_RADIUS.
bool isShot(Pair robotLoc,       // Position of the robot.
        const Pair &point,       // location of the point
        const Pair &ignore,      // point to ignore
        const VisionData &field, // where everyone else is now
        const SystemParameters &p      // contains TEAM, FRAME_RATE, wall locations, etc
);

bool isPassLane(Pair robotLoc,       // Position of the passer.
        const Pair &target,       // location of the target
        const Pair &ignore,      // point to ignore
        const VisionData &field, // where everyone else is now
        const SystemParameters &p      // contains TEAM, FRAME_RATE, wall locations, etc
);

// Returns true if there is a clear shot to the post closest to player
// and places target for shot in (x,y)
bool clearShotClosePost(RobotIndex ID,  //position of player with possession
            const VisionData &field,      //where objects on the field are
            const SystemParameters& params,         //present state of the game
            Pair* target // target we should shoot at
);

// Returns true if there is a clear shot to far post from player
// and places target for shot in (x,y)
bool clearShotFarPost(RobotIndex ID,  //position of player with possession
              //float&      x,         //x of target we should shoot at
              //float&      y,         //y of target we should shoot at
              const VisionData &field,      //where objects on the field are
              const SystemParameters& params,         //present state of the game
              Pair* target // where we should shoot at 
);

// Returns true if there is a clear shot to the right
// and places target for shot in (x,y)
bool clearShotRightPost(RobotIndex ID,  //position of player with possession
            const VisionData &field,      //where objects on the field are
            const SystemParameters& params,         //present state of the game
            Pair* target // where we should shoot at 
);
// Returns true if there is a clear shot to the right
// and places target for shot in (x,y)
bool clearShotRightPost(RobotIndex ID,  //position of player with possession
            const VisionData &field,      //where objects on the field are
            const SystemParameters& params,         //present state of the game
            Pair* target // where we should shoot at 
);

// Returns true if there is a clear shot to the left post
// and places target for shot in (x,y)
bool clearShotLeftPost(RobotIndex ID,  //position of player with possession
               const VisionData &field,      //where objects on the field are
               const SystemParameters& params,        //present state of the game
               Pair* target // where we should shoot at
);

// Returns true if there is a clear shot within a path of buffer width
// from the robot to x,y with preference to side_position
// and places target for shot in (x,y)
bool clearShot(int sidePreference,  // 1=RIGHT, -1=LEFT
           RobotIndex ID,      //position of bot with possession
           float buffer, 
           const VisionData &field, 
           const SystemParameters& params,
           Pair* target // where we should shoot at
);

// Returns true if there is a clear shot within a path of buffer width
// from the robot to x,y
// and places target for shot in (x,y)
bool clearShot(RobotIndex ID,         //position of bot with possession
           const VisionData &field, 
           const SystemParameters& params,
           Pair* target // where we should shoot at
);

// Returns true if there is a clear shot within a path of buffer width
// from the robot to x,y
bool clearShot(RobotIndex ID,         //position of bot with possession
           const VisionData &field, 
           const SystemParameters& params
);



//Makes the robot move a little forward, just in case we have possession but the ball
//is not really touching the dribbler... and then shoots!
void moveForwardAndShoot(RobotIndex ID, const VisionData& field, RobocupStrategyData* sd,
                   Destination* command, KickSpeed kick);

//This function tries to find the best possible shot on goal.  It 
//returns true if the shot lane width is greater than a threshold.
//The target is returned through the reference &target.
bool findShot(RobotIndex ID, 
              const VisionData &field,
              const SystemParameters &rp, 
              Pair* target);


//This function tries to calculate, not approximate, the best possible shot on goal.  It 
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
              Pair* target);

//This function tries to calculate, not approximate, the best possible shot on goal.  It 
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
              Pair* target);

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
               float* lane_Half_Width);
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
               Pair* target); 


//This function tries to calculate, not approximate, the best possible shot on goal.  It 
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
               Pair* target);

//This function tries to calculate, not approximate, the best possible shot on goal.  It 
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
               float* laneHalfWidth);
//================================================================================
// Finds the "shadow" cast by one of our robots on a given line.  It returns the
// left and right points of that shadow, such that y1<y2
void findShadow(Pair ball, Pair robot, float lineX, const SystemParameters &params, float &y1, float &y2);

//================================================================================
// Finds the "shadow" cast by one of our robots on a given line.  It returns the
// left and right points of that shadow, such that x1<x2
void findShadowY(Pair ball, Pair robot, float lineY, const SystemParameters &params, float &x1, float &x2);

// This function finds the split angle between the two points and adds an offset to it if required
float findshootAngle(Pair pivotPoint, Pair point1, Pair point2, float bias);


#endif // SHOOT_H








