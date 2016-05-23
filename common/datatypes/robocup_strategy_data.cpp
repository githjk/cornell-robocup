/*
 *    Functions for RobocupStrategyData
 */
#include "robocup_strategy_data.h"
#include "info/robot/robot.h"
#include "pair/pair.h"
#include "plays/basic_actions.h"
#include "plays/playbook.h"
#include "plays/noplay.h"

//=======================================================================================
//=======================================================================================
RobocupStrategyData::RobocupStrategyData() 
{
  sp = NULL;
  initialize();
}
//=======================================================================================
//=======================================================================================
const SystemParameters& RobocupStrategyData::getSystemParams()
{ 
  return *sp; 
}
//=======================================================================================
//=======================================================================================
//same as above, but lets you modify the parameters:
SystemParameters& RobocupStrategyData::getMutableSystemParams() 
{ 
  return *sp; 
}
//=======================================================================================
//=======================================================================================
//same as above, but lets you modify the parameters:
RoboCupStrategyModule& RobocupStrategyData::getStrategyModule() 
{ 
  return *sm; 
}
//=======================================================================================
//=======================================================================================
void RobocupStrategyData::setSystemParams (SystemParameters* params)
{ 
  sp = params; 
}
//=======================================================================================
//=======================================================================================
void RobocupStrategyData::setStrategyModule (RoboCupStrategyModule* sm)
{ 
  this->sm = sm;
}
//=======================================================================================
//=======================================================================================
const RobotIndex RobocupStrategyData::getRobotByPosition(Position role)
{ 
  return (role == NO_POSITION) ? NO_ROBOT : positions[role]; 
}
//=======================================================================================
//=======================================================================================
//returns true if a robot is assigned to "position" and is not lost by vision
bool RobocupStrategyData::robotExists(Position position, 
                                      const VisionData& field)
{
  return (
           (positions[position] != NO_ROBOT) && 
           field.isRobotFound(sp->general.TEAM, positions[position])
         );
} 
//=======================================================================================
//=======================================================================================
//returns NO_ROBOT if the robot assigned to "role" is lost.
const RobotIndex RobocupStrategyData::getRobotIfFound(Position role, 
                                                      const VisionData& field)
{
  if ((role == NO_POSITION) || ! field.isRobotFound(sp->general.TEAM, positions[role]))
    return NO_ROBOT;
  return positions[role]; 
}
//=======================================================================================
//=======================================================================================
const Position RobocupStrategyData::getPositionOfRobot (RobotIndex index)    
{ 
  return (index == NO_ROBOT) ? NO_POSITION : robots[index]; 
}
//=======================================================================================
//=======================================================================================
void RobocupStrategyData::setRobotAtPosition (RobotIndex robot, 
                                              Position newRole) 
{
  if(robot == NO_ROBOT) 
  {
    cout << "NO_ROBOT passed to setRobotAtPosition()" << endl; 
    return;
  }

  Position oldRole = robots[robot];
  robots[robot] = newRole;

  if(oldRole != NO_POSITION)
  {
    positions[oldRole] = NO_ROBOT;
  }

  if(newRole != NO_POSITION) 
  {
    // If this position was already assigned, set old robot to NO_POSITION
    RobotIndex tempIndex = positions[newRole];
    if ((tempIndex != NO_ROBOT) && (tempIndex != robot))
    {
      robots[tempIndex] = NO_POSITION;
    }

    positions[newRole] = robot;
  }
}
//=======================================================================================
//=======================================================================================
void RobocupStrategyData::swapRobots(RobotIndex robot1, 
                                     RobotIndex robot2) 
{
  if((robot1 == NO_ROBOT) || (robot2 == NO_ROBOT))
  {
    cout << "NO_ROBOT passed to swapRobots()" << endl; 
    return;
  }

  Position oldPos1 = robots[robot1];
  Position oldPos2 = robots[robot2];
  ActionFunction tempAction;
  tempAction = action[robot1];
  robots[robot1] = oldPos2;
  action[robot1] = action[robot2];
  robots[robot2] = oldPos1;
  action[robot2] = tempAction;

  if(oldPos1 != NO_POSITION)
  {
    positions[oldPos1] = robot2;
  }

  if(oldPos2 != NO_POSITION)
  {
    positions[oldPos2] = robot1;
  }
}
//=======================================================================================
//=======================================================================================
void RobocupStrategyData::swapPositions(Position robot1, 
                                        Position robot2) 
{
  if((robot1 == NO_POSITION) || (robot2 == NO_POSITION))
  {
    cout << "NO_POSITION passed to swapPositions()" << endl; 
    return;
  }

  RobotIndex oldIndex1 = positions[robot1];
  RobotIndex oldIndex2 = positions[robot2];
  ActionFunction tempAction = NULL;
  positions[robot1] = oldIndex2;
  positions[robot2] = oldIndex1;

  if(oldIndex1 != NO_ROBOT)
  {
    robots[oldIndex1] = robot2;
    tempAction = action[oldIndex1];

    if (oldIndex2 != NO_ROBOT)
      action[oldIndex1] = action[oldIndex2];
  }

  if(oldIndex2 != NO_ROBOT)
  {
    robots[oldIndex2] = robot1;

    if (tempAction != NULL)
      action[oldIndex2] = tempAction;
  }
}
//=======================================================================================
//=======================================================================================
//called once per frame to set the new play, if neccessary.
void RobocupStrategyData::beginNewPlay (const VisionData& vision, const Playbook& playbook)
{
  if(changePlay)
  {
    playbook.getPlay(playIndex)->endPlay(this);
    playIndex = nextPlayIndex;
    playbook.getPlay(playIndex)->initializePlay(vision, this);
    changePlay = false;
  }
}
//=======================================================================================
//=======================================================================================
void RobocupStrategyData::initialize() 
{
    for (int position = 0; position < NUM_POSITIONS; position++)
    {
      positions[position] = NO_ROBOT;
    }
    for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++)
    {
      robots[robot] = NO_POSITION;
      action[robot] = emptyActionFunction;
      passData[robot].initialize();
      moveCounter[robot]=0;
    }
    switchCommand = NO_ACTION;

    playIndex = NoPlay::playNum;

    //no new play set
    changePlay = false; 

    //no previously determined closest robots to the ball
    closestFriend = NO_ROBOT;
    closestOpponent = NO_ROBOT;
}
//=======================================================================================
//=======================================================================================
void RobocupStrategyData::Initialize2002Positions(const VisionData& vision)
{
  // Positions in order of (approximate) priority
  Position positions[] = {BLOCKER,
                          AGGRESSOR,
                          DEFENDER, 
                          SPECIAL_OP_DEFENDER, 
                          CREATOR};

  int positionIndex=0;

  int numRobotsFound = 0;
  for(int i=0; i<NUM_ROBOTS; i++)
  {
    if(robotFound((RobotIndex)i, vision, *sp))
      numRobotsFound++;
  }

  //skip the blocker
  if(numRobotsFound == 1)
    positionIndex++;

  // If possible, assign the correct robot to be the goalie. If
  // only one robot availble, don't do it, since we want an aggressor
  RobotIndex blockerID = (RobotIndex)sp->general.GOALIE_INDEX;
  if(robotFound(blockerID, vision, *sp) && numRobotsFound > 1)
  {
      setRobotAtPosition(blockerID, BLOCKER);
      setActionFunction(blockerID, stopRobot);
      positionIndex++;
  }
  else
  {
    blockerID = NO_ROBOT;
  }

  // Assign the rest of the robots
  for (RobotIndex robotID = ROBOT0; robotID < NUM_ROBOTS; robotID++)
  {
    //if robot found and not already the blocker
    if(
        robotFound(robotID, vision, *sp) && 
        robotID != blockerID
      )
    {
      setRobotAtPosition(robotID, positions[positionIndex]);
      positionIndex++;
      setActionFunction(robotID, stopRobot);
    }
    //Otherwise if not already the blocker, set to no position
    else if(robotID != blockerID)
    {
        setRobotAtPosition(robotID, NO_POSITION);
        setActionFunction(robotID, stopRobot);
    }
  }
}
//=======================================================================================
//=======================================================================================
void RobocupStrategyData::ResetPositions()
{
  // Set all robots to NO_POSITION and stopRobot
  for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++){
    setRobotAtPosition(robot, NO_POSITION);
    setActionFunction(robot, stopRobot);
  }
}
//=======================================================================================
//=======================================================================================
//calculate offensive and defense ball locations for current frame
void RobocupStrategyData::calculateBallLocations(VisionData* vision, 
                                                 RawVisionData& field,    
                        												 SystemParameters* params)
{
  //max dist a robot can be from the ball to take it
  float maxTheirTakingDist = 2.0f*params->general.OPPONENT_RADIUS;
  
  //find closest friendly and opponent
  float dist;
  float closestOpponentDist, closestFriendDist;
  lastUnpredictedBall.set(((StrategyModule*)sm)->getLastUnpredictedBall());

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //=================================
  //=========== Ball Found ==========
  //=================================
  //if ball is found, use its location for both offensive and defensive balls
  if(field.isBallFound(ROBOCUP_BALL_TEAM,ROBOCUP_BALL_INDEX))
  {
    lastFoundBall.set(vision->getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos());
    offensiveBall.set(vision->getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos());
    defensiveBall.set(vision->getBall(ROBOCUP_BALL_TEAM, ROBOCUP_BALL_INDEX).getPos());
    //field.getBall(ROBOCUP_BALL_TEAM,ROBOCUP_BALL_INDEX).getPos());

    //keep initing these variables. this way
    //when ball first becomes lost, it has to do some work to determine
    //if anybody has it
    closestFriend = NO_ROBOT;
    closestOpponent = NO_ROBOT;

  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //=================================
  //=========== Ball Lost ===========
  //=================================
  //else, apply some fancy heuristics to both balls :)
  else
  {
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    //Handle Offensive Ball
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    //if no friendly tagged
    if(closestFriend == NO_ROBOT)
    {
      int tagFriendlyTemp = -1;
      closestFriendDist = -1.0f;
  
      for(RobotIndex i=ROBOT0; i < NUM_ROBOTS; i++)
      {
        //find closest friendly robot
        if(vision->isRobotFound(sp->general.TEAM, i))
        {
          dist = lastUnpredictedBall.distanceTo(field.getRobot
            (params->general.TEAM,i).getPos());
	  
          if(
              closestFriendDist < 0.0f ||
              dist < closestFriendDist
            )
          {
            closestFriendDist = dist;
            tagFriendlyTemp = i;
          }
        }      
      }
      //if that robot has possession, then set mark that robot
      if(tagFriendlyTemp != -1 &&
         closestFriendDist < 2.0f*params->general.PLAYER_RADIUS)
      {
        closestFriend = (RobotIndex)tagFriendlyTemp;
      }
    }

    //if friendly robot tagged, place offensive ball in front of our robot that last had it
    if(closestFriend != NO_ROBOT)
    {
      offensiveBall.set(frontOfRobot(field.getRobot(params->general.TEAM,closestFriend).getPos(),
                                     field.getRobot(params->general.TEAM,closestFriend).getRotation(),
					        									 *sp));
    }
    //else place offensive ball where ball was last seen.
    else
    {
      offensiveBall.set(lastUnpredictedBall);
    }
   
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    //Handle Defensive Ball
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    //if no opponent tagged
    if(closestOpponent == NO_ROBOT)
    {
      int tagOpponentTemp = -1;
      closestOpponentDist = -1.0f;

      for(RobotIndex i=ROBOT0; i < NUM_ROBOTS; i++)
      {
        //find closest friendly robot
        if(vision->isRobotFound(sp->general.OTHER_TEAM, i))
        {
          dist = lastUnpredictedBall.distanceTo(field.getRobot(params->general.OTHER_TEAM,i).getPos());
          if(
              dist < maxTheirTakingDist &&
              (
                closestOpponentDist < 0.0f ||
                dist < closestOpponentDist
              )
            )
          {
            closestOpponentDist = dist;
            tagOpponentTemp = i;
          }
        }      
      }
      //tagged somebody?
      if(tagOpponentTemp != -1)
      {
        closestOpponent = (RobotIndex)tagOpponentTemp;
      }
    }

    //if friendly robot tagged, place offensive ball in front of our robot that last had it
    if(closestOpponent != NO_ROBOT)
    {
      //draw a line from the center of the opponent to the center of our goal
      //extend along the line by robot radius + ball radius
      extendPoint(Pair(sp->field.OUR_GOAL_LINE, sp->field.SPLIT_LINE),
                  vision->getRobot(sp->general.OTHER_TEAM, closestOpponent).getPos(),
                  -(sp->general.OPPONENT_RADIUS + sp->general.BALL_RADIUS),
                  defensiveBall);
    }
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
}
//=======================================================================================
//=======================================================================================
//get offensive ball location
const Pair RobocupStrategyData::getOffensiveBallLocation()
{
  return offensiveBall;
}
//=======================================================================================
//=======================================================================================
//get defensive ball location
const Pair RobocupStrategyData::getDefensiveBallLocation()
{
  return defensiveBall;
}
//=======================================================================================
//=======================================================================================
//get friendly who has the ball
RobotIndex RobocupStrategyData::getOffensiveBallRobot()
{
  return closestFriend;
}
//=======================================================================================
//=======================================================================================
//get opponent who has the ball
RobotIndex RobocupStrategyData::getDefensiveBallRobot()
{
  return closestOpponent;
}
//=======================================================================================
//=======================================================================================

