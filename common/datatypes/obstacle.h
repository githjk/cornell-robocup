/*
 * obstacle.h
 */

#ifndef DATATYPES_OBSTACLE_H
#define DATATYPES_OBSTACLE_H

#include "ai.h"

//This needs to be made less RoboCup-specific
const int MAX_OBSTACLES = 2*NUM_PLAYERS_ON_TEAM - 1;
const int NO_SUITABLE_OBSTACLE = -1;  

class Obstacles 
{
public:

  Pair getObstacleLoc(int obst) const { return obstacleArray[obst]; }

  void ignore(int botToIgnore) 
  { 
    ignoreArray[botToIgnore] = true; 
    obstacleArray[botToIgnore].setX( -32000 );
    obstacleArray[botToIgnore].setY( -32000 );
  }

  //specify a friend from 0 -> (num - 1) and it will remove him =-)
  void ignoreFriend(int friendToIgnore) 
  { 
    ignore(NUM_PLAYERS_ON_TEAM + friendToIgnore);
  }

  void ignoreUs() 
  {
    //Remove our robots, they are not potential threats
    for(int i=NUM_PLAYERS_ON_TEAM; i<MAX_OBSTACLES; i++)
    {
      ignoreArray[i] = true;
    }
  }

  void ignoreLost() 
  {
    //Remove any robots that can't be found
    for(int i=0; i<MAX_OBSTACLES; i++)
    {
      if( 
         (obstacleArray[i].getX() == -32000) ||
         (obstacleArray[i].getY() == -32000)
        )
      {
        ignoreArray[i] = true;
      }
    }
  }

  void ignoreIfBehindX(float x) 
  {
    //Remove any robots that can't be found
    for(int i=0; i<MAX_OBSTACLES; i++)
    {
      if(obstacleArray[i].getX() < x)
      {
        ignoreArray[i] = true;
      }
    }
  }

  // returns the index of the closest non-ignored obstacle that's
  // at least minDistance away.
  int findClosestObstacle (const Pair& location, float minDistance = 0.0f) const {
    int currentClosest = NO_SUITABLE_OBSTACLE;
    float currentDistance = 1000.0f;
    for (int i = 1; i < MAX_OBSTACLES; i++) {
      if (!ignoreArray[i]) {
        float distance = location.distanceTo(obstacleArray[i]);
        if((distance >= minDistance) && (distance < currentDistance)){
          currentDistance = distance;
          currentClosest = i;
        }
      }
    }
    return currentClosest;
  }

  //get a pointer to array of obstacles so we can pass it into findObstacles
  Pair* getObstacleArray()
  {
    return obstacleArray;
  }

  //Fetches all obstacles that aren't being ignored, and return the number of them
  int copyObstacles(Pair newObstacleArray[MAX_OBSTACLES])
  {
	int numObs = 0;
    for(int i=0; i<MAX_OBSTACLES; i++)
    {
      if(!ignoreArray[i])
      {
        newObstacleArray[numObs] = obstacleArray[i];
        numObs++;
      }
    }
    return numObs;
  }

private:
  Pair obstacleArray[MAX_OBSTACLES];
  bool ignoreArray[MAX_OBSTACLES];
};


#endif
