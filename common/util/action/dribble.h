/*
 *	dribble.h
 *
 */

#ifndef ACTION_DRIBBLE_H
#define ACTION_DRIBBLE_H

#include "ai.h"
#include "VisionModule.h"

//-----------------------------------------------------------
//Instructs This_Robot to DRIBBLE UP THE FIELD
//If there is nothing between This_Robot and the goal post
//on the current side of the field, it heads right at that
//goal post.
void dribbleBallUpfield(const VisionData& field,
                        const SystemParameters& params,
                        RobotIndex thisRobot, 
                        Destination* command,
                        RobotIndex screeningRobot=NO_ROBOT); 
//-----------------------------------------------------------
//Instructs This_Robot to DRIBBLE BACK DOWN THE FIELD
//If there is nothing between This_Robot and the center of 
//the field current side of the field, it heads right at 
//the center of the field.
void dribbleBallBack(const VisionData& field,
                     const SystemParameters& params,
                     RobotIndex thisRobot,   
                     Destination* command,
                     RobotIndex screeningRobot=NO_ROBOT); 
//-----------------------------------------------------------
//This is the main dribble algorithm that the sugar coated versions
//above all call. Takes a point and dribbles to it as best it can
//Using the find_open_space and spin_around_the_ball algorithms =-)
void dribbleBall(const VisionData& field,
                 const SystemParameters& params,
                 RobotIndex      thisRobot,
                 const Pair& ultimateDestination,
                 Destination* command,
                 RobotIndex screeningRobot=NO_ROBOT,
                 bool backwards = false); 


//Spins around the ball with dribbling without 
//putting any pressure on the ball.
void spinAroundBall(
            const SystemParameters& params,
            const VisionData& field,
            RobotIndex        thisRobot,     
            const Pair&       ultimateDestination,
            Destination*      command);

//Spins around the ball with dribbling without 
//putting any pressure on the ball.
void spinAroundBall(const SystemParameters& params,
                    const VisionData& field,
                    RobotIndex  thisRobot,
                    const Pair& position, 
                    const Pair& ultimateDestination,
                    Destination* command);
//-------------------------------------------------
//Dribbles the ball away from the wall
void dribbleBallAwayFromWall(const VisionData& field,
                             const SystemParameters& params,
                             RobotIndex thisRobot,  //Identification of this robot
                             Destination* command);
//-------------------------------------------------



#endif // ACTION_DRIBBLE_H