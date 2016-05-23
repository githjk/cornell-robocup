/*
 *	robot.cpp
 */

#include "robot.h"

/********************Begin Field Data Access functions********************************/

static float goalieDistFromGoal;
static float currentDistFromGoal;
static Pair currentRobotPos;

//Returns Other Team's Goalie ID (or closest player to goal in goalie box) as "goalie"
//Returns false if no goalie is found within the penalty box
bool getTheirGoalie(const VisionData& field, const SystemParameters& params, RobotIndex* goalie){

	*goalie = NO_ROBOT;

	for (RobotIndex i = ROBOT0; i<NUM_ROBOTS; i++){
	
		if(theirRobotFound(i, field, params)){
			currentRobotPos.set( getTheirRobotLocation(i, field, params) );

			//check if this robot is in the goalie box
			if (nearTheirGoalieBox (currentRobotPos, params, params.general.OPPONENT_RADIUS) ) {

				currentDistFromGoal = currentRobotPos.distanceTo(params.field.THEIR_GOAL_LINE, 0.0f);

				//Set goalie to the closest robot to the center of opponent goal
				if(currentDistFromGoal < params.field.FIELD_WIDTH/2 
					 && ((*goalie == NO_ROBOT) || (currentDistFromGoal < goalieDistFromGoal)) ){
					*goalie = i;
					goalieDistFromGoal = currentDistFromGoal;
				}
			}
		}
	}

	if(*goalie == NO_ROBOT)
		//There was no valid goalie found
		return false;

	//We found a valid goalie
	else
		return true;

}
//Returns Other Team's Goalie ID (or closest player to goal) as goalie
//Returns false if no goalie is found
//Differs from above function in that it first considers only guys in the box, and 
// after that people outside it.
bool getTheirKeeper(const VisionData& field, 
					const SystemParameters& params, 
          RobotIndex* goalie){
  
	*goalie = NO_ROBOT;

	for (RobotIndex i = ROBOT0; i<NUM_ROBOTS; i++){
	
		if(theirRobotFound(i, field, params)){
			currentRobotPos.set( getTheirRobotLocation(i, field, params) );

			//check if this robot is in the goalie box
			if (inTheirGoalieBox (currentRobotPos, params) ) {

				currentDistFromGoal = currentRobotPos.distanceTo(params.field.THEIR_GOAL_LINE, 0.0f);

				//Set goalie to the closest robot to the center of opponent goal
				if(currentDistFromGoal < params.field.FIELD_WIDTH/2 
					 && ((*goalie == NO_ROBOT) || (currentDistFromGoal < goalieDistFromGoal)) ){
					*goalie = i;
					goalieDistFromGoal = currentDistFromGoal;
				}
			}
		}
	}

  if(*goalie == NO_ROBOT){
		//There was no valid goalie found.
    //let's look in the rest of the field.

	  for (RobotIndex i = ROBOT0; i<NUM_ROBOTS; i++){
	  
		  if(theirRobotFound(i, field, params)){
			  currentRobotPos.set( getTheirRobotLocation(i, field, params) );

        currentDistFromGoal = currentRobotPos.distanceTo(params.field.THEIR_GOAL_LINE, 0.0f);

				//Set goalie to the closest robot to the center of opponent goal
				if(currentDistFromGoal < params.field.FIELD_WIDTH/2 
					 && ((*goalie == NO_ROBOT) || (currentDistFromGoal < goalieDistFromGoal)) ){
					*goalie = i;
					goalieDistFromGoal = currentDistFromGoal;
				}
		  }
	  }
  }
	//We found a valid goalie
	if(*goalie == NO_ROBOT)
    return false; //this means we found no robots at all.
	else
		return true;

}

//Returns True if there are two opponent players in the penalty Area
//Returns false otherwise
bool twoOpponentsInTheirBox(const VisionData& field, 
							const SystemParameters& params)
{
  int botsin = 0;
  
  for (RobotIndex i = ROBOT0; i<NUM_ROBOTS; i++){
    
    if(theirRobotFound(i, field, params)){
      
      currentRobotPos.set( getTheirRobotLocation(i, field, params) );

      if (nearTheirGoalieBox (currentRobotPos, params, (params.general.OPPONENT_RADIUS*0.5f) ) )
      {
        botsin++;
      }
    }
  }
  
  //If there are more than 1 bots in their goal box
  return (botsin > 1);
}
bool inTheirGoalieBox(const Pair& position, const SystemParameters& params) {
  return (position.getX() > params.field.THEIR_GOALIE_BOX)
      && (position.getY() < params.field.LEFT_GOALIE_BOX)
      && (position.getY() > params.field.RIGHT_GOALIE_BOX);
}

//true if within params.PLAYER_RADIUS of their goalie box
bool nearTheirGoalieBox(const Pair& position, 
						const SystemParameters& params, 
						float margin) 
{
  return (position.getX() > (params.field.THEIR_GOALIE_BOX - margin))
      && (position.getY() < (params.field.LEFT_GOALIE_BOX + margin))
      && (position.getY() > (params.field.RIGHT_GOALIE_BOX - margin));
}

bool inOurGoalieBox(const Pair& position, const SystemParameters& params) {
  return (position.getX() < params.field.OUR_GOALIE_BOX)
      && (position.getY() < params.field.LEFT_GOALIE_BOX)
      && (position.getY() > params.field.RIGHT_GOALIE_BOX);
}

//true if within params.PLAYER_RADIUS of our goalie box
bool nearOurGoalieBox(const Pair& position, const SystemParameters& params, float margin) {
  return (position.getX() < (params.field.OUR_GOALIE_BOX + margin))
      && (position.getY() < (params.field.LEFT_GOALIE_BOX + margin))
      && (position.getY() > (params.field.RIGHT_GOALIE_BOX - margin));
}

bool isLocationInField(const Pair& position, const SystemParameters& params) {
  return (position.getX() > params.field.OUR_GOAL_LINE)
      && (position.getX() < params.field.THEIR_GOAL_LINE)
      && (position.getY() > params.field.RIGHT_SIDE_LINE)
      && (position.getY() < params.field.LEFT_SIDE_LINE);
}

void constrainToField(const SystemParameters& params, 
					            Pair* pos) 
{
  if(pos->getX() <= (params.field.OUR_GOAL_LINE + params.general.PLAYER_RADIUS))
    pos->setX(params.field.OUR_GOAL_LINE + params.general.PLAYER_RADIUS);
  
  if(pos->getX() >= (params.field.THEIR_GOAL_LINE - params.general.PLAYER_RADIUS))
    pos->setX(params.field.THEIR_GOAL_LINE - params.general.PLAYER_RADIUS);
  
  if(pos->getY() >= (params.field.LEFT_SIDE_LINE - params.general.PLAYER_RADIUS))
    pos->setY(params.field.LEFT_SIDE_LINE - params.general.PLAYER_RADIUS);
  
  if(pos->getY() <= (params.field.RIGHT_SIDE_LINE + params.general.PLAYER_RADIUS))
    pos->setY(params.field.RIGHT_SIDE_LINE + params.general.PLAYER_RADIUS); 
}

void constrainOutOfOurBox(const SystemParameters params, 
                          Pair* pos)
{
  //if destination EVER inside our goalie box or goal, push out.
  if(
      pos->getX() < (params.field.OUR_GOALIE_BOX + params.general.PLAYER_RADIUS) &&
      pos->getY() < (params.field.LEFT_GOALIE_BOX + params.general.PLAYER_RADIUS) &&
      pos->getY() > (params.field.RIGHT_GOALIE_BOX - params.general.PLAYER_RADIUS)
    )
  {
    float frontDiff = ABS(params.field.OUR_GOALIE_BOX + 
                          params.general.PLAYER_RADIUS - 
                          pos->getX());

    float leftDiff = ABS(params.field.LEFT_GOALIE_BOX + 
                         params.general.PLAYER_RADIUS -
                         pos->getY());

    float rightDiff = ABS(params.field.RIGHT_GOALIE_BOX - 
                          params.general.PLAYER_RADIUS + 
                          pos->getY());

    //project to closest edge

    //front closest
    if(frontDiff < leftDiff &&
       frontDiff < rightDiff)
    {
      pos->setX(params.field.OUR_GOALIE_BOX + params.general.PLAYER_RADIUS);
    }   
    //left closest
    else if(leftDiff < frontDiff &&
            leftDiff < rightDiff)
    {
      pos->setY(params.field.LEFT_GOALIE_BOX + params.general.PLAYER_RADIUS);
    }
    //right closest
    else if(rightDiff < frontDiff &&
            rightDiff < leftDiff)
    {
      pos->setY(params.field.RIGHT_GOALIE_BOX - params.general.PLAYER_RADIUS);
    }
  }
}

void constrainOutOfTheirBox(const SystemParameters params, 
                          Pair* pos)
{
  //if destination EVER inside our goalie box or goal, push out.
  if(
      pos->getX() > (params.field.THEIR_GOALIE_BOX - params.general.PLAYER_RADIUS) &&
      pos->getY() < (params.field.LEFT_GOALIE_BOX + params.general.PLAYER_RADIUS) &&
      pos->getY() > (params.field.RIGHT_GOALIE_BOX - params.general.PLAYER_RADIUS)
    )
  {
    float frontDiff = ABS(params.field.THEIR_GOALIE_BOX - 
                          params.general.PLAYER_RADIUS - 
                          pos->getX());

    float leftDiff = ABS(params.field.LEFT_GOALIE_BOX + 
                         params.general.PLAYER_RADIUS -
                         pos->getY());

    float rightDiff = ABS(params.field.RIGHT_GOALIE_BOX - 
                          params.general.PLAYER_RADIUS + 
                          pos->getY());

    //project to closest edge

    //front closest
    if(frontDiff < leftDiff &&
       frontDiff < rightDiff)
    {
      pos->setX(params.field.THEIR_GOALIE_BOX - params.general.PLAYER_RADIUS);
    }   
    //left closest
    else if(leftDiff < frontDiff &&
            leftDiff < rightDiff)
    {
      pos->setY(params.field.LEFT_GOALIE_BOX + params.general.PLAYER_RADIUS);
    }
    //right closest
    else if(rightDiff < frontDiff &&
            rightDiff < leftDiff)
    {
      pos->setY(params.field.RIGHT_GOALIE_BOX - params.general.PLAYER_RADIUS);
    }
  }
}

//Algorithm which fills array of pairs with
//positions of all obsticals on the filed
//does not include position of this robot
//
void findObstacles(const VisionData& field,
                   const SystemParameters& params,
                   RobotIndex robotID,
                   Pair obstacleArray[2*NUM_PLAYERS_ON_TEAM - 1])
{
  //Get array of enemy robots
  const MovingObject* enemies = field.getTeam(params.general.OTHER_TEAM);

  //Get array of our robots
  const MovingObject* friends = field.getTeam(params.general.TEAM);


  //enemy robots
  for(int i=0; i<NUM_PLAYERS_ON_TEAM; i++)
  {	
    if(!field.isRobotFound(params.general.OTHER_TEAM, i))
    {
      obstacleArray[i].setX( -32000 );
      obstacleArray[i].setY( -32000 );
    }
    else
    {
      obstacleArray[i].setX( (enemies[i].getPos()).getX() );
      obstacleArray[i].setY( (enemies[i].getPos()).getY() );
    }
  }

  //our robots with the exception of little ol me robot =-) (this robot)
  for(int j=0; j<NUM_PLAYERS_ON_TEAM; j++)
  {
    if(j < robotID)
    {
      if(!field.isRobotFound(params.general.TEAM, j))
      {
        obstacleArray[NUM_PLAYERS_ON_TEAM + j].setX( -32000 );
        obstacleArray[NUM_PLAYERS_ON_TEAM + j].setY( -32000 );
      }
      else
      {
        obstacleArray[NUM_PLAYERS_ON_TEAM + j].setX( (friends[j].getPos()).getX() );
        obstacleArray[NUM_PLAYERS_ON_TEAM + j].setY( (friends[j].getPos()).getY() );
      }  
    }
    else if(j > robotID)
    {
      if(!field.isRobotFound(params.general.TEAM, j))
      {
        obstacleArray[NUM_PLAYERS_ON_TEAM - 1 + j].setX( -32000 );
        obstacleArray[NUM_PLAYERS_ON_TEAM - 1 + j].setY( -32000 );
      }
      else
      {
        obstacleArray[NUM_PLAYERS_ON_TEAM - 1 + j].setX( (friends[j].getPos()).getX() );
        obstacleArray[NUM_PLAYERS_ON_TEAM - 1 + j].setY( (friends[j].getPos()).getY() );
      }  
    }
  }

}


/*End Field Data Access functions*/

