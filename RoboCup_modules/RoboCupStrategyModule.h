#ifndef RC_STRATEGY_MODULE_H
#define RC_STRATEGY_MODULE_H

class RoboCupStrategyModule;

#include "StrategyModule.h"
#include "robocup_strategy_data.h"
#include "plays/Playbook.h"
#include "plays/basic_actions.h"
#include "../Skillz/SkillSet.h"

#include "positions/positionSwitching/Weather.h"
#include "plays/playTransition/playTransition.h"


/*****************************
* RoboCup Strategy Module    *
*****************************/
class RoboCupStrategyModule : public StrategyModule
{
	
public:

	// constructors 
	RoboCupStrategyModule(SystemParameters* SP, 
                          VisionModule* vision);


  //deletes all the skill sets, play transition and position swtiching boxes
  ~RoboCupStrategyModule();

	// retrieves current frame
	StrategyData* getCurrentFrame(int frame=-1) 
  { 
    //if frame # unspecified, take current frame
    if(frame==-1)  
      return &RCstrategyData[currentFrame];
    //else, select specified frame
    else
      return &RCstrategyData[frame];
  }

	RobocupStrategyData* getCurrentRoboCupFrame(int frame=-1);

  RobocupStrategyData* getRelativeRoboCupFrame(int frameOffset);

  virtual void executeStrategy();
  virtual void executeNoStrategy();
  
  virtual void Initialize2002Positions();
  virtual void ResetPositions();

  // copies current frame to last frame
  void copyIntoLastFrame();

  SkillSet* getSkillSet(RobotIndex id);

  Playbook *playbook;

  PlayTransition* pt;

  //Checks and preforms position switching
  Weather* weather;

  bool checkTransitions;
  bool checkPositions;
protected:
	///Array of SkillSets, one for each robot
  SkillSet* SkillSets[NUM_ROBOTS];
	
  //I don't think we can safely assign "new RobocupStrategyData[MAX_LOG]" to strategyData
	RobocupStrategyData* RCstrategyData;

  // Switches positions
  void handleSwitchCommand(RobotIndex ID, SwitchCommand switchCommand);

  //Checks and preforms play transitions

};

void initializeDestination(RobotIndex index, 
						   const VisionData& field, 
						   const SystemParameters& params,
						   Destination* command);


#endif // STRATEGY_MODULE_H
