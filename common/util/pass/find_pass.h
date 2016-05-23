#ifndef FIND_PASS_H
#define FIND_PASS_H

#include "VisionModule.h"
#include "robocup_strategy_data.h"
#include "..\info\robot\robot.h"
#include "..\math\dist.h"
#include "..\lane\lane.h"


//Constants.  NEVER INITIALIZE THEM TO ZERO!
const int sidelineNumSearchPoints = 25;
const int centerfieldNumSearchPoints = 25;
const int halflineNumSearchPoints = 50;
const float sideBuffer = 0.1f;  //Upfield pass: dont pass too close to sideline!
//Upfield pass: one robot goes far, another comes forward by some amount:
const float passLengthDiff = 0.2f; 


//This function returns to best (farthest forward) pass destination to a sideline 
//from one robot to another.  It will only pass if the passing lane angle is 
//within the defined threshhold.
//
//@param passerPos:	  position of passing robot
//@param receiverPos: Position of receiving robot
//@param passDest:	  pointer to a Pair object indicating the pass destination
//
//RETURNS: a bool indicating if a good pass is found.
bool findSidelinePass(Pair passerPos,
						 Pair receiverPos,
						 Pair *passDest,
						 const VisionData& field,
						 const SystemParameters &rp);



//This function returns to best (farthest forward) pass destination to the center 
//(x-axis) from one robot to another.  It will only pass if the passing lane angle 
//is within the defined threshhold.
//
//@param passerPos:	position of passing robot
//@param passDest:	pointer to a Pair object indicating the pass destination
//
//RETURNS: a bool indicating if a good pass is found.
bool findCenterfieldPass(Pair passerPos,
						 Pair *passDest,
						 const VisionData& field,
						 const SystemParameters &rp);



//This function finds up to two passes upfield to pass to.  The passes with the 
//largest pass lane angle are chosen.  The pass distance (in x direction) 
//is a fixed percentage of the distance from ball to enemy goal line.  This allows 
//us to pass farther upfield if we have possession farther downfield. The first pass 
//will be closer than the second pass.
//
//@param passerPos: position of passing robot
//@param passDest1: pointer to a Pair object indicating one pass destination
//@param passDest2: pointer to a Pair object indicating a 2nd pass destination
//
//RETURNS: an integer indicating how many passes were found (0,1,2)
int findUpfieldPass(Pair passerPos,
						Pair *passDest1,
						Pair *passDest2,
						const VisionData& field,
						const SystemParameters &rp);

// Used in CLEAR UPFIELD
// Determines if a forward is open for a pass in it's current position,
// rather than searching for an open pass.
bool openForPass(RobotIndex ID,
						const VisionData& field,
						const SystemParameters &rp);



//Used to find a sideline pass to a sideline nearest our goal.  This is
//especially useful for the goalie to pass the ball, getting it away from 
//the goal and out of the center of the field.
bool findNearSidelinePass(Pair passerPos,
						 Pair *passDest,
						 const VisionData& field,
						 const SystemParameters &rp,
             bool chooseRightSide);
bool findNearSidelinePass(Pair passerPos,
						  Pair *passDest,
						  const VisionData& field,
						  const SystemParameters &rp,
                          bool chooseRightSide);


//finds a location for the passForward during the corner kick play...
bool isGoalBoxPassLocation(const VisionData& field,
						   const SystemParameters &rp,
                           Pair  &passDest,
                           bool leftSide);


#endif //FIND_PASS_H