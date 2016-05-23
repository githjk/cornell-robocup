
#ifndef SCRATCH_VARS_H
#define SCRATCH_VARS_H

#include "ai.h"
#include "ActionCodes.h"

namespace ScratchVars {
    extern SwitchCommand  action;  //This is the switch command, not the action function.

    extern float  xtarget, ytarget;

    extern float  xball, yball, ballvx, ballvy;
    extern float  xRobot, yRobot, thetaRobot;
    extern float  xEnemy, yEnemy;
    extern float  xGoalie, yGoalie, thetaGoalie;

    extern float  newtheta;
    extern float  theta;

    extern float  xnextball;
    extern float  ynextball;

    extern float  velocity_squared;
    extern float  distance;

    extern float  singleDefenderLine;

    extern RobotIndex robotID;
    extern RobotIndex robot1, robot2;

    extern Line	left;
    extern Line	right;
    extern Line	backLine;

    extern Pair ballVelocity;
    extern Pair leftIntersection;
    extern Pair rightIntersection;

    extern Pair nextDestination;

    extern bool useObstacleAvoidance; //turn off for stripping the ball!

    extern bool cornerPass;//used for corner kick play...

    //other random variables:
    extern float xleft, xright, yleft, yright;  //from Goalie::defense
};

#endif