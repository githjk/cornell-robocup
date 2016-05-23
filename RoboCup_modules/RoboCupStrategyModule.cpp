#include "RoboCupStrategyModule.h"
#include "info/robot/robot.h"
#include "common/util/info/ball/possession.h"

// constructor
RoboCupStrategyModule::RoboCupStrategyModule(SystemParameters* SP,
                                             VisionModule* vision)
	: StrategyModule()
	{
	//create a playbook!
    playbook = new Playbook(vision);

    //Checks and preforms play transitions
    pt = new PlayTransition(vision);
    //Checks and preforms position switching
    weather = new Weather(this);
    
    this->SP = SP;
    this->vision	= vision;
    
	currentFrame = VISION_FIRST_FRAME;

	strategyData = NULL;
	RCstrategyData = new RobocupStrategyData[MAX_LOG];

    //reset possession pointers
    resetPossessionCounters();

    // Set parameter pointers
    for (int i = 0; i < MAX_LOG; i++)
		{
			RCstrategyData[i].setSystemParams(SP);
			RCstrategyData[i].setStrategyModule(this);
		}

    // Initialize Action Functions
    for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++) {
      RCstrategyData[0].setActionFunction(robot, stopRobot);
    }
    
    //Initialize the array of SkillSets
    for(robot = ROBOT0; robot < NUM_ROBOTS; robot++) 
    {
      SkillSets[robot] = new SkillSet(SP, vision, this, robot);
    }


		StrategyRunning = false;

    checkTransitions = true;
    checkPositions = true;

}


//deletes all the skill sets, play transition and position swtiching boxes
RoboCupStrategyModule::~RoboCupStrategyModule()
{
    for(int i=0; i<NUM_ROBOTS; i++)
    {
       delete SkillSets[i];      
    }

    delete pt;
    delete weather;

}
RobocupStrategyData* RoboCupStrategyModule::getCurrentRoboCupFrame(int frame)
{ 
  //if frame # unspecified, take current frame
  if(frame==-1)  
    return &RCstrategyData[currentFrame];
  //else, select specified frame
  else{
    if(frame < 0)
      frame = 0;
    if(frame >= MAX_LOG)
      frame = MAX_LOG-1;
  }

  return &RCstrategyData[frame];
}


SkillSet* RoboCupStrategyModule::getSkillSet(RobotIndex id)
{ 
  if(id >= ROBOT0 && id < NUM_ROBOTS)
    return SkillSets[id]; 
  else
    return NULL;
}

RobocupStrategyData* RoboCupStrategyModule::getRelativeRoboCupFrame(int frameOffset)
{
  int frame = currentFrame + frameOffset;
  if(frame < 0)
    frame = MAX_LOG+frame;
  if(frame >= MAX_LOG)
    frame = frame - MAX_LOG;

  return &RCstrategyData[frame];
}

// executes one frame of strategy
void RoboCupStrategyModule::executeStrategy()
{
  //******************************************
  // Copy data from last time.
  //******************************************
  // - if currentFrame == 0, assume the first frame was already initialized
  if(currentFrame > 0)
  {
    RCstrategyData[currentFrame] = RCstrategyData[currentFrame - 1];
  }
  else
  {
    RCstrategyData[0] = RCstrategyData[MAX_LOG - 1];
  }
 
  //**********************************************
  // Update posession status for all robots
  //**********************************************
  updateRobotsPossession(*vision->getRawFrame(), *SP, getCurrentFrame());

  //**********************************************
  // Update visiondata and strategy data pointers in all skills
  //**********************************************
  for(RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++) 
  {
    SkillSets[robot]->updateVisionAndStrategyPointers();
  }

  //******************************************
  // Update play and positions
  //******************************************

  SwitchCommand switchCommand;

  // Did the play change last frame?
  if(RCstrategyData[currentFrame].playChanged())
  {
    RCstrategyData[currentFrame].beginNewPlay(*vision->getCurrentFrame(), *playbook);

    //Since we are in a new play, run position switching code again to make sure right special op present
    if(checkPositions)
      weather->preformSwitching(vision, 
                                (SystemParameters*)&(this->getCurrentRoboCupFrame()->getSystemParams()));
    for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++)
    {
      switchCommand = RCstrategyData[currentFrame].getDestination(robot)->getSwitchCommand();
      if(switchCommand != NO_ACTION)
      {
        handleSwitchCommand(robot, switchCommand);
      }
    }
  }
  // Did the robots choose to switch positions last frame?
  else 
  {
    for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++){
      switchCommand = RCstrategyData[currentFrame].getDestination(robot)->getSwitchCommand();
      if(switchCommand != NO_ACTION){
        handleSwitchCommand(robot, switchCommand);
      }
    }
  }

  // Get pointer to current play
  BasePlay* currentPlay = playbook->getPlay(RCstrategyData[currentFrame].getCurrentPlay());

  for (robot = ROBOT0; robot < NUM_ROBOTS; robot++)
  {
    initializeDestination(robot, *vision->getCurrentFrame(), *(this->SP), 
      RCstrategyData[currentFrame].getDestination(robot));
  } 

  //**********************************************
  // Update offensive and defensive ball locations
  //**********************************************
  RCstrategyData[currentFrame].calculateBallLocations(vision->getCurrentFrame(),
	                                                    *vision->getRawFrame(),
													                            SP);

  //******************************************
  // Execute the current play
  //******************************************
  currentPlay->executePlay(vision->getCurrentFrame(), &RCstrategyData[currentFrame]);

  //******************************************
  // Transition to another play if required
  //******************************************
  if(checkTransitions)
    pt->transition(vision->getCurrentFrame(), 
                   &RCstrategyData[currentFrame], 
                   currentPlay);

  //******************************************
  // Swap positions if necessary
  //******************************************
  if(checkPositions)
    weather->preformSwitching(vision, 
                              (SystemParameters*)&(this->getCurrentRoboCupFrame()->getSystemParams()));
}

// executes one frame of strategy
void RoboCupStrategyModule::executeNoStrategy()
{
  //******************************************
  // Copy data from last time.
  //******************************************

  // - if currentFrame == 0, assume the first frame was already initialized
  if(currentFrame > 0){
    RCstrategyData[currentFrame] = RCstrategyData[currentFrame - 1];
  }
  else
  {
    RCstrategyData[0] = RCstrategyData[MAX_LOG - 1];
  }

  for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++)
  {
    RCstrategyData[currentFrame].setMessage(robot, "AI disabled");
  }

  updateRobotsPossession(*vision->getRawFrame(),*SP, getCurrentFrame());
  
}

// copies current frame to last frame
void RoboCupStrategyModule::copyIntoLastFrame(){
  if (currentFrame > 1) 
    RCstrategyData[MAX_LOG - 1] = RCstrategyData[currentFrame - 1];
  else 
    RCstrategyData[MAX_LOG - 1] = RCstrategyData[currentFrame];

  RCstrategyData[VISION_FIRST_FRAME] = RCstrategyData[MAX_LOG - 1];
}

void RoboCupStrategyModule::Initialize2002Positions()
{
  //initialize all robots to have 2002 positions
  RCstrategyData[currentFrame].Initialize2002Positions( *vision->getCurrentFrame() );

  //get pointer to current play
  BasePlay* currentPlay = playbook->getPlay(RCstrategyData[currentFrame].getCurrentPlay());

  //initialize the play again so that each robot is assigned the correct action function
  currentPlay->initializePlay(*vision->getCurrentFrame(), &RCstrategyData[currentFrame]);
}


void RoboCupStrategyModule::ResetPositions(){
  RCstrategyData[currentFrame].ResetPositions();
}


//Tells the robot to stay put.
void initializeDestination(RobotIndex index, 
						   const VisionData& field, 
						   const SystemParameters& params,
						   Destination* command)
{
  command->initialize();
  command->setPos( field.getRobot(params.general.TEAM, index).getPos() );
  command->setRotation(field.getRobot(params.general.TEAM, index).getRotation() );
}


void RoboCupStrategyModule::handleSwitchCommand(RobotIndex ID, SwitchCommand switchCommand){
    RobocupStrategyData* sd = getCurrentRoboCupFrame();

    // Get pointer to current play
    BasePlay* currentPlay = playbook->getPlay(RCstrategyData[currentFrame].getCurrentPlay());

	  // Set the correct action function
	  switch (switchCommand) {
		  case NO_ACTION:             break;

		  default: cout << "Invalid SwitchCommand passed to handleSwitchCommand(): " << switchCommand << endl; break;
	  }

  }

