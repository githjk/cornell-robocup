/*
 *	robot.h
 *
 */
#ifndef UTIL_ROBOT_H
#define UTIL_ROBOT_H

#include <math.h>
#include "ai.h"
#include "common/datatypes/SystemParameters.h"
#include "VisionModule.h"

		//*********OUR TEAM*************************
//Returns true if this robot exists
__inline bool robotFound(RobotIndex index, const VisionData& field, const SystemParameters& p){

	return (index != NO_ROBOT) && (field.isRobotFound(p.general.TEAM, index));

}

// return the number of our robots currently on the field 
__inline int numRobotFound(const VisionData& field, const SystemParameters& p){

  int numRobots = 0;

  for(RobotIndex r = ROBOT0; r < NUM_ROBOTS; r++)
    if(robotFound(r,field, p)) numRobots++; 

  return numRobots;

}

// Returns Robot's MovingObject data
__inline const MovingObject& getRobot(const RobotIndex index, const VisionData& field, const SystemParameters& params){

	return field.getRobot(params.general.TEAM, index );

}


// Returns Robot location
__inline Pair getLocation(const RobotIndex ID, const VisionData& field, const SystemParameters& params){

	return field.getRobot(params.general.TEAM, ID ).getPos();

}


// Returns Robot X position by RobotIndex
__inline float getXLocation(const RobotIndex ind, const VisionData& field, const SystemParameters& params){
	return float(field.getRobot(params.general.TEAM, ind ).getXPos());
}

//Returns Robot Y position by RobotIndex
__inline float getYLocation(const RobotIndex ind, const VisionData& field, const SystemParameters& params){
	return float(field.getRobot(params.general.TEAM, ind ).getYPos());
}

//Returns Robot Rotation *NB: there is only Rotation Data for Our Team's Robots
__inline float getRotation(const RobotIndex index, const VisionData& field, const SystemParameters& params){

	return float(field.getRobot(params.general.TEAM, index ).getRotation());

}

// Returns Robot X velocities
__inline float getXVel(const RobotIndex index, const VisionData& field, const SystemParameters& params){

	return float(field.getRobot(params.general.TEAM, index ).getXVel());

}

//Returns Robot Y velocities
__inline float getYVel(const RobotIndex index, const VisionData& field, const SystemParameters& params) {
	return float(field.getRobot(params.general.TEAM, index ).getYVel());
}

//Returns Robot Y velocities
__inline float getRotVel(const RobotIndex index, const VisionData& field, const SystemParameters& params) {
	return float(field.getRobot(params.general.TEAM, index ).getRotVel());
}

//Returns Robot's speed (magnitude of its velocity)
__inline float getSpeed(const RobotIndex index, const VisionData& field, const SystemParameters& params) {
  float xVel = float(field.getRobot(params.general.TEAM, index ).getXVel());
  float yVel = float(field.getRobot(params.general.TEAM, index ).getYVel());

  return SQRT(xVel*xVel + yVel*yVel);
}



//*************BOTH TEAMS **************************

//Returns true if this robot exists
__inline bool robotFound(const int team, RobotIndex index, const VisionData& field, const SystemParameters& p){
	return (index != NO_ROBOT) && (field.isRobotFound(team, index));
}



// Returns Robot X position
__inline float getXLocation(const int team, RobotIndex index, const VisionData& field){
	return float(field.getRobot(team, index ).getXPos());
}

//Returns Robot Y position
__inline float getYLocation(const int team, RobotIndex index, const VisionData& field){

	return float(field.getRobot(team, index ).getYPos());

}

//Returns Robot position
__inline Pair getLocation(const int team, RobotIndex index, const VisionData& field){

	return Pair(field.getRobot(team, index ).getPos());

}

//Returns Robot Rotation *NB: there is only Rotation Data for Our Team's Robots
__inline float getRotation(const int team, RobotIndex index, const VisionData& field, const SystemParameters& params){
	if (team == params.general.TEAM)
		return getRotation(index, field, params);
	else
		return 0;
}


// Returns Robot X velocities
__inline float getXVel(int team, RobotIndex index, const VisionData& field){

	return float(field.getRobot(team, index ).getXVel());

}

//Returns Robot Y velocities
__inline float getYVel(int team, RobotIndex index, const VisionData& field) {
	return float(field.getRobot(team,index ).getYVel());
}

//Returns Robot Rot velocities
__inline float getRotVel(int team, RobotIndex index, const VisionData& field) {
	return float(field.getRobot(team,index ).getRotVel());
}



			//*******************OTHER TEAM*************************
//Returns true if this robot exists
__inline bool theirRobotFound(RobotIndex index, const VisionData& field, const SystemParameters& p){
	return field.isRobotFound(p.general.OTHER_TEAM, index);
}


// Returns Other team Robot X position -- RobotIndex version
// Returns Other team Robot X position -- RobotIndex version
__inline Pair getTheirRobotLocation(RobotIndex index, const VisionData& field, const SystemParameters& params){
	return field.getRobot(params.general.OTHER_TEAM, index).getPos();
}

// Returns Other team Robot X position
// Returns Other team Robot X position
__inline float getTheirXLocation(RobotIndex index, const VisionData& field, const SystemParameters& params){
	return float(field.getRobot(params.general.OTHER_TEAM,index ).getXPos());
}

//Returns Other Team Robot Y position
//Returns Other Team Robot Y position
__inline float getTheirYLocation(RobotIndex index, const VisionData& field, const SystemParameters& params){
	return float(field.getRobot(params.general.OTHER_TEAM,index ).getYPos());
}


// Returns Other team Robot X velocity
// Returns Other team Robot X velocity
__inline float getTheirXVel(RobotIndex index, const VisionData& field, const SystemParameters& params){
	return float(field.getRobot(params.general.OTHER_TEAM,index ).getXVel());
}


//Returns Other Team Robot Y velocity
//Returns Other Team Robot Y velocity
__inline float getTheirYVel(RobotIndex index, const VisionData& field, const SystemParameters& params){
	return float(field.getRobot(params.general.OTHER_TEAM,index ).getYVel());
}


//Returns Other Team's Goalie ID (or closest player to goal) as goalie
//Returns false if no goalie is found
bool getTheirGoalie(const VisionData& field, 
					const SystemParameters& params, 
					RobotIndex* goalie);

//Returns Other Team's Goalie ID (or closest player to goal) as goalie
//Returns false if no goalie is found
//Differs from above function in that it first considers only guys in the box, and 
// after that people outside it.
bool getTheirKeeper(const VisionData& field, 
					const SystemParameters& params, 
					RobotIndex* goalie);

//Returns True if there are two opponent players in the penalty Area
//Returns false otherwise
bool twoOpponentsInTheirBox(const VisionData& field, const SystemParameters& params);

bool inTheirGoalieBox(const Pair& pos, const SystemParameters& params);
bool nearTheirGoalieBox(const Pair& pos, const SystemParameters& params, float margin);
bool inOurGoalieBox(const Pair& pos, const SystemParameters& params);
bool nearOurGoalieBox(const Pair& pos, const SystemParameters& params, float margin);

bool isLocationInField(const Pair& pos, const SystemParameters& params);
void constrainToField(const SystemParameters& params, Pair* pos);
void constrainOutOfOurBox(const SystemParameters params, Pair* pos);
void constrainOutOfTheirBox(const SystemParameters params, Pair* pos);
                          

//Algorithm which fills array of pairs with
//positions of all obsticals on the filed
//does not include position of this robot
void findObstacles(const VisionData& field,
                   const SystemParameters& params,
                   RobotIndex robotID,
                   Pair obstacleArray[2*NUM_PLAYERS_ON_TEAM - 1]);


#endif // UTIL_ROBOT_H
