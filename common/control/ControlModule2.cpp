#include "ControlModule.h"
#include "ObstacleAvoidance.h"
#include "WallAvoidance.h"

#include "info/robot/robot.h"
#include "math/dist.h"

#include "float.h"

static int skipCounter;


void validateValue(float& vel) {
  if ( _isnan(vel) || !_finite(vel) ) {
    vel = 0.0f;
  }
}

//==============================================================================
ControlModule::ControlModule (SystemParameters* sp, 
                              VisionModule* vision, 
                              StrategyModule* strategy)
{
  skipCounter = 0;
  this->SP = sp;
  this->vision = vision;
  this->strategy = strategy;

  currentFrame = VISION_FIRST_FRAME;

  for(int i=0; i<NUM_ROBOTS; i++)
  {
    kickTimer[i] = 0;
    delayKicker[i] = 0;
  }
  stopRobots();
  
  wireless = new WirelessSender(SP->general.COM_PORT);
  wireless->setBoard(sp->general.WIRLESS_BOARD);

  WirelessRunning = false;
}
//==============================================================================
//Converts from AI speeds to wireless command speeds
void ControlModule::parseKickSpeed(int inputSpeed, int robotID)
{
  if(robotID < ROBOT0 || robotID > ROBOT4)
    return;
  switch(inputSpeed)
  {
    case NO_KICK:
      kickSpeed[robotID] = 0; break;
    case KICK_SLOW:
      kickSpeed[robotID] = 1; break;
    case KICK_PASS:
      kickSpeed[robotID] = 2; break;
    case KICK_PASS_IMMEDIATE:
      kickSpeed[robotID] = -2; break;
    case KICK_WEAK_1:
      kickSpeed[robotID] = 3; break;
    case KICK_WEAK_2:
      kickSpeed[robotID] = 4; break;
    case KICK_WEAK_3:
      kickSpeed[robotID] = 5; break;
    case KICK_WEAK_4:
      kickSpeed[robotID] = 6; break;   
    case KICK_SHOT:  
      kickSpeed[robotID] = 7; break;   
    case KICK_SHOT_IMMEDIATE:  
      kickSpeed[robotID] = -7; break;   
    default:
       kickSpeed[robotID] = 7; break;
  }
}
//==============================================================================
//Converts from AI speeds to wireless command speeds
void ControlModule::parseDribbleSpeed(int inputSpeed, int robotID)
{
  if(robotID < ROBOT0 || robotID > ROBOT4)
    return;
  switch(inputSpeed)
  {
    case NO_DRIBBLE:
      dribbleSpeed[robotID] = 0; break;
    case SLOW_DRIBBLE:
      dribbleSpeed[robotID] = 1; break;
    case MID_DRIBBLE:
      dribbleSpeed[robotID] = 2; break;
    case FAST_DRIBBLE:
    default:
      dribbleSpeed[robotID] = 3; break;
  }
}
//==============================================================================
//Converts from AI speeds to wireless command speeds
void ControlModule::parseVerticalDribbleSpeed(int inputSpeed, int robotID)
{
  if(robotID < ROBOT0 || robotID > ROBOT4)
    return;
  switch(inputSpeed)
  {
    case NO_V_DRIBBLE:
      verticalDribbleSpeed[robotID] = 0; break;
    case SLOW_V_DRIBBLE:
      verticalDribbleSpeed[robotID] = 1; break;
    case MID_V_DRIBBLE:
      verticalDribbleSpeed[robotID] = 2; break;
    case FAST_V_DRIBBLE:
    default:
      verticalDribbleSpeed[robotID] = 3; break;
  }
}
//==============================================================================
// Uses the current Parameter file to update the obstacle avoidance system
void ControlModule::updateParameters()
{
  updateObstacleAvoidance
  (
    SP->general.PLAYER_RADIUS,
    SP->general.OPPONENT_RADIUS,
    SP->general.BALL_RADIUS,
    SP->field.OUR_GOAL_LINE,
    SP->field.OUR_GOALIE_BOX,
    SP->field.THEIR_GOAL_LINE,
    SP->field.THEIR_GOALIE_BOX,
    SP->field.RIGHT_SIDE_LINE,
    SP->field.RIGHT_GOALIE_BOX,
    SP->field.LEFT_SIDE_LINE,
    SP->field.LEFT_GOALIE_BOX,
    SP->general.SLOW_CLEARANCE,
    SP->general.FAST_CLEARANCE,
    SP->general.FAST_CLEARANCE_VELOCITY,
    SP->general.LATENCY_FRAMES
  );
 
  updateWallAvoidance
  (
    SP->field.OUR_GOAL_LINE,
    SP->field.THEIR_GOAL_LINE,
    SP->field.RIGHT_SIDE_LINE,
    SP->field.RIGHT_GOALIE_BOX,
    SP->field.RIGHT_GOAL_POST,
    SP->field.LEFT_SIDE_LINE,
    SP->field.LEFT_GOALIE_BOX,
    SP->field.LEFT_GOAL_POST,
    SP->field.OUR_GOALIE_BOX,
    SP->field.THEIR_GOALIE_BOX,
    SP->general.PLAYER_RADIUS,
    SP->general.MAX_WALL_OFFSET,
    SP->general.MAX_WALL_OFFSET_DISTANCE
  );

  wireless->setBoard(SP->general.WIRLESS_BOARD);

}


//==============================================================================
//Returns wireless status (on/off)
bool ControlModule::wirelessStatus()  
{ 
  return WirelessRunning; 
}

//Toggles wireless status (on/off)
bool ControlModule::toggleWirelessStatus()
{
  WirelessRunning = !WirelessRunning;
  return WirelessRunning;
}

//Sets wireless status (on/off)
bool ControlModule::setWirelessStatus(bool newWirelessStatus) 
{ 
  WirelessRunning = newWirelessStatus; 
  return WirelessRunning;
}

//==============================================================================

// retrieves current frame
ControlData* ControlModule::getCurrentFrame(int frame) 
{ 
  //if frame # unspecified, take current frame
  if(frame==-1)  
    return &controlData[currentFrame];
  //else, select specified frame
  else
    return &controlData[frame];
}

//==============================================================================
// sets currentFrame (assume it is a valid frame)
void ControlModule::setFrame(int newFrame) 
{
  currentFrame = newFrame;
}

// sets currentFrame (assume it is a valid frame)
ControlData* ControlModule::getControlArray(void) 
{ 
  return controlData; 
}


//==============================================================================
// sends commands to Robots

//timers used to spin down dribble before kick, then kick for a set number of frames
#define KICK_FRAMES 5
#define SPIN_DOWN_DRIBBLER_FRAMES 5

void ControlModule::sendRPCCommands(Client* client)
{
  skipCounter++;
  if(skipCounter == 2)
    skipCounter = 0;
  if(true)
  //else
  {


  if (WirelessRunning)
  {
    for(int i=0; i<NUM_ROBOTS; i++)
    {
      //==========================================================================================
      //set destination position
      wireless->setRobotVelocity( i, 
                                  controlData[currentFrame].getXVel(i),
                                  controlData[currentFrame].getYVel(i),
                                  controlData[currentFrame].getRotVel(i));

      wireless->setRobotRotation(i, vision->getOurRobotRotation(i,currentFrame));

      //==========================================================================================
      //set destination actions
      bool kick = false;
      bool dribble = false;
      bool vdribble = false;

      //if wait timer has run out, and kick timer 
      //is not zero, kick & ignore current dribble commands
      if(delayKicker[i] == 0 && kickTimer[i] != 0)
      {
        kick = true;
        dribble = false;
        vdribble = false;
        //if ai trying to kick, keep kick counter at KICK_FRAMES
        if(controlData[currentFrame].getKick((RobotIndex)i) != NO_KICK)
        {
            kickTimer[i] = KICK_FRAMES;
            parseKickSpeed(controlData[currentFrame].getKick((RobotIndex)i), i);
        }
      }
      else
      {
        //if ai trying to kick
        if(controlData[currentFrame].getKick((RobotIndex)i) != NO_KICK)
        {
          //if wait timer run out, set kickTimer and kick
          if(delayKicker[i] == 0)
          {
            kick = true;
            dribble = false;
            vdribble = false;
            kickTimer[i] = KICK_FRAMES;
            parseKickSpeed(controlData[currentFrame].getKick((RobotIndex)i), i);
          }
          //else wait for dribbler to spin down, 
          //don't kick or dribble
          else
          {
            kickTimer[i] = KICK_FRAMES;
            parseKickSpeed(controlData[currentFrame].getKick((RobotIndex)i), i);
            kick = false;
            dribble = false;
            vdribble = false;
          }
        }
        //ai not trying to kick
        else
        {
          //if ai trying to dribble
          if(
              controlData[currentFrame].getDribble((RobotIndex)i) != NO_DRIBBLE ||
              controlData[currentFrame].getVerticalDribble((RobotIndex)i) != NO_V_DRIBBLE
            )
          {
            //set wait timer to SPIN_DOWN_DRIBBLER_FRAMES
            delayKicker[i] = SPIN_DOWN_DRIBBLER_FRAMES;

            kick = false;
            if(controlData[currentFrame].getDribble((RobotIndex)i) != NO_DRIBBLE)
            {
              dribble = true;
              parseDribbleSpeed(controlData[currentFrame].getDribble((RobotIndex)i), i);

            }
            if(controlData[currentFrame].getVerticalDribble((RobotIndex)i) != NO_V_DRIBBLE)
            {
              vdribble = true;
              parseVerticalDribbleSpeed(controlData[currentFrame].getVerticalDribble((RobotIndex)i), i);
            }
          }
          //else don't dribble or kick
          else
          {
             kick = false;
             dribble = false;
             vdribble = false;
          }
        }
      }

      //tick down timers

      //dribbler not slowing down, and kicking
      if(kickTimer[i] > 0 && delayKicker[i] == 0)
        kickTimer[i]--;

      //dribbler slowing down
      if(delayKicker[i] > 0)
        delayKicker[i]--;

     
      //if kick, dribble, or vertical dribble is set to false, set speed to 0
      int kSpeed, dSpeed, vdSpeed;
      
      if(!kick)
        kSpeed = 0;
      else
        kSpeed = kickSpeed[i];

      //negative kick speeds indicate kick immediatley at this speed
      if(controlData[currentFrame].getKick((RobotIndex)i) < 0)
      {
        kSpeed = -controlData[currentFrame].getKick((RobotIndex)i);
        kickTimer[i] = KICK_FRAMES;
      }

      if(!dribble)
        dSpeed = 0;
      else
        dSpeed = dribbleSpeed[i];

      if(!vdribble)
        vdSpeed = 0;
      else
        vdSpeed = verticalDribbleSpeed[i];

      wireless->setRobotActions(i,
                                kSpeed,
                                dSpeed,
                                vdSpeed);
      //==========================================================================================
    }

    wireless->buildPacket();
    wireless->sendCommands();
  }

  }
}

static AIData instructions;
const unsigned char SIM_DRIBBLE = (0x06); // 00000110
const unsigned char SIM_KICK    = (0x01); // 00000001

//==============================================================================
// sends commands to Simulator
void ControlModule::sendSimulatorCommands(Client* client)
{
  // Set the data for all robots
  int multiplier = 1;
  if (vision->areSidesFlipped())
    multiplier = -1;

  float xVel, yVel, rotVel;

  for (RobotIndex i= ROBOT0; i < NUM_ROBOTS; i++) 
  {
    xVel = multiplier * controlData[currentFrame].getXVel(i);
    yVel = multiplier * controlData[currentFrame].getYVel(i);
    rotVel = controlData[currentFrame].getRotVel(i);

    validateValue(xVel);
    validateValue(yVel);
    validateValue(rotVel);

    instructions.robot_instructs[i].xVel = xVel;
    instructions.robot_instructs[i].yVel = yVel;
    instructions.robot_instructs[i].thetaVel = rotVel;

    instructions.robot_instructs[i].parameter = 0x00;
    if (controlData[currentFrame].getKick(i) != NO_KICK) 
      instructions.robot_instructs[i].parameter |= SIM_KICK;
    if (controlData[currentFrame].getDribble(i) != NO_DRIBBLE) 
      instructions.robot_instructs[i].parameter |= SIM_DRIBBLE;
  }

  client->send(&instructions, sizeof(instructions));
}

//==============================================================================
void ControlModule::stopRobots() 
{
  for (RobotIndex i= ROBOT0; i < NUM_ROBOTS; i++)
  {
    controlData[currentFrame].stopRobot(i);
  }
}

void validateVelocity(float& vel) {
  if ( _isnan(vel) || !_finite(vel) ) {
    vel = 0.0f;
  }
}


//==============================================================================
void ControlModule::generatePaths() 
{

  // Initialize Obstacle Avoidance with current frame of vision
  VisionData* field = vision->getCurrentFrame();
  setupObstacleAvoidance(field, SP->general.FRAME_RATE);

  // Get current destination data for each robot
  StrategyData* strategyData = strategy->getCurrentFrame();
  
  Destination* dest;
  ControlMethod controlMethod;

  ObjectPosVel start, final, nextStep;
  float distance;

  // Generate a path for each robot
  for (RobotIndex index=ROBOT0; index<NUM_ROBOTS; index++) 
  {

    //-------------------------------------------------------------------------
    // If the robot is lost, make it spin (hopefully bringing it back into view)
    if(!field->isRobotFound(SP->general.TEAM, index))
    {
      controlData[currentFrame].driftRobot(index,SP->general.FRAME_RATE);
      continue;
    }

    //-------------------------------------------------------------------------
    dest = strategyData->getDestination(index);
    controlMethod = dest->getControl();

    // Initialize control data
    controlData[currentFrame].setRobotVelocity(
      index,
      0.0f,
      0.0f,
      0.0f
    );

    // Set robot commands
    controlData[currentFrame].setRobotCommands(
      index,
      dest->getKick(),
      dest->getDribble(),
      dest->getVerticalDribble(),
      dest->getBeep()
    );

    // Get robot and trajectory parameters for the robot
    RobotTypeParams*  typeParams;
    TrajectoryParams* trajParams;

    switch  (wireless->getRobotType(index) ) 
    {
    case ROBOT_2002:
      typeParams = &SP->control.Omni2002;
      break;
    case ROBOT_2001:
    default:
      typeParams = &SP->control.Omni2001;
      break;
    }

    switch ( dest->getSpeed() )
    {
    case GOALIE_SPEED:
      trajParams = &typeParams->GOALIE;
      break;
    case SPIN_TO_KICK_SPEED:
      trajParams = &typeParams->SPIN_TO_KICK;
      break;
    case CAREFUL_SPEED:
      trajParams = &typeParams->CAREFUL_SPEED;
      break;
    case BALL_POSSESSION_SPEED:
      trajParams = &typeParams->BALL_POSSESSION_SPEED;
      break;
    case BALL_APPROACH_SPEED:
      trajParams = &typeParams->BALL_APPROACH_SPEED;
      break;
    case FORMATION_SPEED:
      trajParams = &typeParams->FORMATION_SPEED;
      break;
    case SLOW_ROTATE:
      trajParams = &typeParams->SLOW_ROTATE;
      break;
    case DEFAULT_SPEED:
    default:
      trajParams = &typeParams->DEFAULT;
      break;
    }

    //-------------------------------------------------------------------------
    // Manual Control Methods
    switch (controlMethod) 
    {

    case MANUAL_TRANSLATION:

      if ( ABS(start.rotation - final.rotation) < 0.05f )
      {
        nextStep.rotVel = 0.0f;
      }
      else if ( ABS(start.rotation - final.rotation) < 0.10f )
      {
        // Calculate rotational velocity using proportional gain
        proportionalGainTheta( start,
                                final,
                                SP->general.FRAME_RATE,
                                trajParams->ANGULAR_GAIN,
                                trajParams->MAX_THETA_VELOCITY,
                                nextStep.rotVel
                              );
      }
      else {
        // Calculate rotational velocity using bang-bang
        zeroFinalVelocityTheta( start,
                                final,
                                SP->general.FRAME_RATE,
                                trajParams->MAX_THETA_ACCELERATION,
                                trajParams->MAX_THETA_VELOCITY,
                                nextStep.rotVel
                              );
      }

      dest->setRotVel(nextStep.rotVel);

      // Don't break, fall through to MANUAL_CONTROL
    case MANUAL_CONTROL:
      controlData[currentFrame].setRobotVelocity(
        index,
        dest->getXVel(),
        dest->getYVel(),
        dest->getRotVel()
      );
      continue;
    }

    //-------------------------------------------------------------------------
    // The rest of the ControlMethods are path generation methods.
    //    Therefore, check for default conditions first

    start.xPos      = getXLocation(index, *field, *SP);
    start.xVel      = getXVel(index, *field, *SP);
    start.yPos      = getYLocation(index, *field, *SP);
    start.yVel      = getYVel(index, *field, *SP);
    start.rotation  = getRotation(index, *field, *SP);
    start.rotVel    = getRotVel(index, *field, *SP);

    final.xPos      = dest->getXPos();
    final.xVel      = dest->getXVel();
    final.yPos      = dest->getYPos();
    final.yVel      = dest->getYVel();
    final.rotation  = dest->getRotation();
    final.rotVel    = dest->getRotVel();

    //if final dest is inside a robot, push out to just next to robot



    distance = dist( start.xPos, start.yPos, final.xPos, final.yPos);

    // Are we close enough to stop?
    if ( distance < typeParams->STOP_DISTANCE )
    {
      robotPath[index].clearPath();

      robotPath[index].addStep(start);

      nextStep.xPos     = start.xPos;
      nextStep.yPos     = start.yPos;
      nextStep.rotation = start.rotation;

      nextStep.xVel     = 0.0f;
      nextStep.yVel     = 0.0f;
      nextStep.rotVel   = 0.0f;

      robotPath[index].addStep(nextStep);
    }

    // Are we close enough to use proportional gain?
    else if ( distance < typeParams->PROP_GAIN_DISTANCE )
    {
      proportionalGainPath(  
        start,
        final,
        SP->general.FRAME_RATE,
        trajParams->LATERAL_GAIN, 
        trajParams->ANGULAR_GAIN, 
        trajParams->MAX_VELOCITY, 
        trajParams->MAX_THETA_VELOCITY,
        robotPath[index]
      );
    }
    else {
      //-------------------------------------------------------------------------
      // Choose proper path generation function
      switch (controlMethod) 
      {
      // Normal Trajectory Generation
      case OMNI_GOALIE:
//        avoidGoalCorners(start, final);
      case OMNI_NO_AVOID:
        zeroFinalVelocityPath(
          start,
          final,
          SP->general.FRAME_RATE,
          trajParams->MAX_ACCELERATION,
          trajParams->MAX_THETA_ACCELERATION, 
          trajParams->MAX_VELOCITY, 
          trajParams->MAX_THETA_VELOCITY,
          robotPath[index],
          -1
        );
        break;

      // Fast Trajectory Generation
      case OMNI_GOALIE_FAST:
//        avoidGoalCorners(start, final);
        fastPath(
          start,
          final,
          SP->general.FRAME_RATE,
          trajParams->MAX_ACCELERATION,
          trajParams->MAX_VELOCITY, 
          robotPath[index],
          -1
        );
        break;

      // Normal Obstacle Avoidance
      case OMNI_NORMAL_ENTERBOX:
      case OMNI_NORMAL:
      case OMNI_STRIVE:
      //temp in here
      case OMNI_FAST:
      case OMNI_FAST_ENTERBOX:

#if 1
        findOpenPath(
          start,
          final,
          SP->general.TEAM,
          index,
          SP->general.FRAME_RATE,
          trajParams->MAX_ACCELERATION,
          trajParams->MAX_THETA_ACCELERATION, 
          trajParams->MAX_VELOCITY, 
          trajParams->MAX_THETA_VELOCITY,
          robotPath[index]
        );
#else
        findTestOpenPath(
          start,
          final,
          SP->general.TEAM,
          index,
          SP->general.FRAME_RATE,
          trajParams->MAX_ACCELERATION,
          trajParams->MAX_THETA_ACCELERATION, 
          trajParams->MAX_VELOCITY, 
          trajParams->MAX_THETA_VELOCITY,
          robotPath[index]
        );
#endif
        
        break;

      // Fast Obstacle Avoidance
//      case OMNI_FAST:
//      case OMNI_FAST_ENTERBOX:
       findFastOpenPath(
          start,
          final,
          SP->general.TEAM,
          index,
          SP->general.FRAME_RATE,
          trajParams->MAX_ACCELERATION,
          trajParams->MAX_THETA_ACCELERATION, 
          trajParams->MAX_VELOCITY, 
          trajParams->MAX_THETA_VELOCITY,
          robotPath[index],
          -1
        );
        break;
    
      } // End switch(controlMethod)
    }

    // If there is not at least one step in the path,
    //    then stop the robot
    if ( !robotPath[index].get(1, nextStep) )
    {
      robotPath[index].clearPath();

      robotPath[index].addStep(start);

      nextStep.xPos     = start.xPos;
      nextStep.yPos     = start.yPos;
      nextStep.rotation = start.rotation;

      nextStep.xVel     = 0.0f;
      nextStep.yVel     = 0.0f;
      nextStep.rotVel   = 0.0f;

      robotPath[index].addStep(nextStep);
    }

    //-------------------------------------------------------------------------
    // Do we need to avoid the goalie box?
    switch (controlMethod) 
    {
    case OMNI_NORMAL:
    case OMNI_STRIVE:
    case OMNI_FAST:
      avoidGoalieBox( 
        final,
        nextStep,
        SP->general.FRAME_RATE 
      );
      break;
    } // End switch(controlMethod)


    //-------------------------------------------------------------------------
    // Do we need to avoid the walls?
    switch (controlMethod) 
    {
    case OMNI_GOALIE:
    case OMNI_GOALIE_FAST:
      // Allow goalie to move behind the goal 
      avoidGoalieWalls ( SP->general.FRAME_RATE, final, nextStep );
      break;
    case OMNI_NO_AVOID:
      break;
    default:
      // Avoid the walls
      avoidWalls ( SP->general.FRAME_RATE, final, nextStep );
      break;
    }


    //-------------------------------------------------------------------------
    // Calculate theta velocity

    if ( ABS(start.rotation - final.rotation) < typeParams->STOP_THETA_DISTANCE )
    {
      nextStep.rotVel = 0.0f;
    }
    else if ( ABS(start.rotation - final.rotation) < typeParams->PROP_GAIN_THETA_DISTANCE )
    {
      // Calculate rotational velocity using proportional gain
      proportionalGainTheta(  start,
                              final,
                              SP->general.FRAME_RATE,
                              trajParams->ANGULAR_GAIN,
                              trajParams->MAX_THETA_VELOCITY,
                              nextStep.rotVel
                            );
    }
    else {
      // Calculate rotational velocity using bang-bang
      zeroFinalVelocityTheta( start,
                              final,
                              SP->general.FRAME_RATE,
                              trajParams->MAX_THETA_ACCELERATION,
                              trajParams->MAX_THETA_VELOCITY,
                              nextStep.rotVel
                            );
    }

    validateVelocity(nextStep.xVel);
    validateVelocity(nextStep.yVel);
    validateVelocity(nextStep.rotVel);

    // Set the robot's velocity
    controlData[currentFrame].setRobotVelocity(
      index,
      nextStep.xVel,
      nextStep.yVel,
      nextStep.rotVel
    );

  } // End for index = ROBOT0 to NUM_ROBOTS

} // End generatePaths()

//=================================================================================================
