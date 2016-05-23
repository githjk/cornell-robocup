/*
 * screen.h
 */

#ifndef ANNOY_SCREEN_H
#define ANNOY_SCREEN_H


#include "ai.h"
#include "robocup_strategy_data.h"


//Tells "this robot" to go to a position that will screen 
//"scared robot" as s/he moves up the field
void offensiveScreen(const VisionData& field,
                     const SystemParameters &params,
                     RobotIndex thisRobot,
                     //Indentification of our friend we are trying to help out =-)
                     RobotIndex scaredRobot,
                     Destination* command);


//-----------------------------------------------------------------------------------
//Use this algorithm to send robots into forward-pass lanes and also screen opponents
//-----------------------------------------------------------------------------------
void defensiveScreen(const VisionData& field,
                     RobocupStrategyData* sd,
                     RobotIndex thisRobot,
                     //ID's for other friends
                     //also going into pass lanes
                     RobotIndex otherFriendlyRobots[4]);
#endif // ANNOY_SCREEN_H
