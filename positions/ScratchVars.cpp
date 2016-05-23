#include "ScratchVars.h"

namespace ScratchVars {
	SwitchCommand  action;  //This is the switch command, not the action function.

    float  xtarget, ytarget;

    float	xball, yball, ballvx, ballvy;
    float	xRobot, yRobot, thetaRobot;
    float xEnemy, yEnemy;
    float	xGoalie, yGoalie, thetaGoalie;

    float	newtheta;
    float	theta;

    float	xnextball;
    float	ynextball;

    float velocity_squared;
    float distance;
    
    float	singleDefenderLine;

    RobotIndex  robotID;
    RobotIndex  robot1, robot2;

    Line	left;
    Line	right;
    Line	backLine;

    Pair ballVelocity;
    Pair leftIntersection;
    Pair rightIntersection;

    Pair nextDestination;

    bool useObstacleAvoidance; //turn off for stripping the ball!

    bool cornerPass;//used for corner kick play...

	//other random variables:
	float xleft, xright, yleft, yright;  //from Goalie::defense
};
