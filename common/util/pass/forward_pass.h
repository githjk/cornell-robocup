#ifndef FORWARD_PASS_H
#define FORWARD_PASS_H

#include "pass.h"
#include "ai.h"
#include "info/ball/ball.h"
#include "info/ball/possession.h"
#include "info/robot/robot.h"
#include "intersect/intersect.h"
#include "math/angle.h"
#include "math/dist.h"
#include "shoot/shoot.h"
#include "passData.h"
#include "robocup_strategy_data.h"

//******************************************
//******************************************
//*************Forward Pass*****************
//******************************************
//******************************************
//---------------------------------------------------------
//When you are into the forward passing mode, that is, the
//passer has kicked the ball, have the receiving robot call
//this procedure to carefully receive the pass
//---------------------------------------------------------
bool trapForwardPass(const VisionData& field,
                     RobocupStrategyData* sd,
                     //robots Position
                     RobotIndex robotID,
                     Destination* sendTo);

//---------------------------------------------------------
//Finally, receiveForwardPass calls forwardPassLane to do
//all the work. It gets back the results and sets the robot
//to that position
//---------------------------------------------------------

void findForwardPassLane(const VisionData& field,
                         RobocupStrategyData* sd,
                         const Pair& ballLocation,
                         Pair primitiveObstacle[2*NUM_PLAYERS_ON_TEAM - 1],
                         const float& optimalDistance,
                         //So we don't see outself as an obstacle
                         const Pair& assigningRobot,
                         //------------------------------------------
                         // Return pair here
                         //------------------------------------------
                         Pair* passlane);

#endif

