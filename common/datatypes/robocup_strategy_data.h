#ifndef ROBOCUP_STRATEGY_MODULE_H
#define ROBOCUP_STRATEGY_MODULE_H

class BasePlay;
class RobocupStrategyData;
class Playbook;

#include "StrategyModule.h"
#include "PassData.h"
#include "plays/plays.h"
#include "assert.h"
#include "info/ball/possession.h"
#include "common/datatypes/SystemParameters.h"

// Define the index of each position in gameState.positions[]
enum Position
{
  NO_POSITION = -1,
  //---------------------
  //2002 Positions
  BLOCKER,
  DEFENDER,
  AGGRESSOR,
  CREATOR,
  SPECIAL_OP_DEFENDER,
  SPECIAL_OP_AGGRESSOR,
  SPECIAL_OP_CREATOR,
  //---------------------
  NUM_POSITIONS,
};

typedef void (*ActionFunction)(RobotIndex ID,
                               BasePlay* play,
                               const VisionData& field,
                               RobocupStrategyData* sd);

// use this as the action function when the robot isn't using an action function
inline void emptyActionFunction(RobotIndex ID, BasePlay* play, 
                                const VisionData& field, RobocupStrategyData* sd) 
{

}

/*********************
* Strategy Data      *
*********************/
class RobocupStrategyData : public StrategyData
{
public:
  RobocupStrategyData();

  void initialize();

  void Initialize2002Positions(const VisionData& vision);

  void ResetPositions();

  const SystemParameters& getSystemParams();

  //same as above, but lets you modify the parameters:
  SystemParameters& getMutableSystemParams();
  RoboCupStrategyModule& getStrategyModule();

  void setSystemParams (SystemParameters* sp);
  void setStrategyModule(RoboCupStrategyModule* sm); 
  

  const RobotIndex getRobotByPosition (Position role);

  //returns true if a robot is assigned to "position" and is not lost by vision
  bool robotExists(Position position, const VisionData& field);

  //returns NO_ROBOT if the robot assigned to "role" is lost.
  const RobotIndex getRobotIfFound (Position role, const VisionData& field);

  const Position getPositionOfRobot (RobotIndex index);
  
  void setRobotAtPosition (RobotIndex robot, Position newRole);
  
  void swapRobots(RobotIndex robot1, RobotIndex robot2);
  
  void swapPositions(Position robot1, Position robot2);
  
  const Pair getPassDest (RobotIndex index)
  { 
    return passData[index].getPassPos(); 
  }

  void setPassDest (RobotIndex index, const Pair &pd) 
  { 
    passData[index].setPassPos(pd); 
  }

  int getMoveCounter(RobotIndex index) 
  { 
    return moveCounter[index];
  }

  void updateMoveCounter (RobotIndex index) 
  {
    moveCounter[index]++;
  }

  void resetMoveCounter (RobotIndex index) 
  { 
    moveCounter[index]=0;
  }

  const bool getPassValue (RobotIndex index)
  { 
    return passData[index].getPassValue(); 
  }

  void setPassValue (RobotIndex index, bool v) 
  { 
    passData[index].setPassValue(v); 
  }

  Pass* getPassData (RobotIndex index) 
  { 
    return &passData[index]; 
  }

  const Play getCurrentPlay (void)
  { 
    return playIndex; 
  }

  const Play getNextPlay (void)
  { 
    return nextPlayIndex;
  }

  void setNewPlay (Play p) 
  { 
    changePlay = true;  nextPlayIndex = p;  
  }

  void resetChangePlay () 
  { 
    changePlay = false;  nextPlayIndex = playIndex;  
  }

  const bool playChanged (void)
  { 
    return changePlay; 
  }

  //called once per frame to set the new play, if neccessary.
  void beginNewPlay (const VisionData& vision, const Playbook& playbook);

  ActionFunction getActionFunction (RobotIndex index) const 
  { 
    return action[index]; 
  }

  void setActionFunction (RobotIndex index, ActionFunction a) 
  { 
    action[index] = a; 
  }

  void setActionFunction (Position role, ActionFunction a) 
  {
    if (positions[role] != NO_ROBOT)
      action[positions[role]] = a;
  }

  void operator=(const RobocupStrategyData& old) 
  {
    memcpy(this, &old, sizeof(RobocupStrategyData));

    for(int i=0; i<NUM_ROBOTS; i++)
    {
      //reset robot messages
      setMessage((RobotIndex)i, "-----");

      //decrement pass values
      passData[i].degrade();
    }
  }

  //calculate offensive and defense ball locations for current frame
  void calculateBallLocations(VisionData* vision, 
	                            RawVisionData& field, 
	                            SystemParameters* params);

  //get offensive ball location
  const Pair getOffensiveBallLocation();

  RobotIndex getOffensiveBallRobot();
  RobotIndex getDefensiveBallRobot();

  //get defensive ball location
  const Pair getDefensiveBallLocation();



 private:
  RoboCupStrategyModule* sm;
  SystemParameters* sp;

  Position    robots[NUM_ROBOTS];       // index to individual position (or NO_POSITION)
  RobotIndex  positions[NUM_POSITIONS]; // index to individual robot (or NO_ROBOT)

  //For passes:
  Pass  passData[NUM_ROBOTS];
  int   moveCounter[NUM_ROBOTS];
  Play playIndex;  // The current play
  Play nextPlayIndex; //Next Play's frame, if set by the AI.
  bool changePlay;
  ActionFunction  action[NUM_ROBOTS];
  SwitchCommand switchCommand;

  //ball position heursitics
  Pair offensiveBall, defensiveBall, lastFoundBall, lastUnpredictedBall;
  RobotIndex closestOpponent, closestFriend;

};

#endif // ROBOCUP_STRATEGY_MODULE_H

