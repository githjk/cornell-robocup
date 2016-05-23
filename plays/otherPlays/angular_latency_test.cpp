/*
 *	latency_test.cpp
 */

#include "angular_latency_test.h"
#include "../../main.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"

#include "../noplay.h"

const float START_ANGLE = PI/4;
const float START_ANGLE_THRESHOLD = 0.10f; //radians
const float START_ANGLE_MIN_TIME = 2.0f; //radians
const float TARGET_ANGLE = 0;

//The robot is considered stable if, for the entire time between MIN_TURN_TIME and MAX_TURN_TIME,
// its position is within stabilityThresholdTheta radians of TARGET_ANGLE
const float INTITIAL_TIME = 6.00f; //seconds
const float TEST_LENGTH   = 10.00f; //seconds

const float DEFINTITELY_UNSTABLE = 0.75f*PI; //radians

const float PRECISION = 0.1f;
const float HUGE_NUMBER = 999999.0f;
Play AngularLatencyTest::playNum = -1;

AngularLatencyTest::AngularLatencyTest(VisionModule *vm) : BasePlay(vm)
{

}

void AngularLatencyTest::initializePlay(const VisionData& vision, RobocupStrategyData* rsd) 
{
	 //backup old parameters:
  oldVThetaMax = rsd->getSystemParams().control.Omni2001.DEFAULT.MAX_THETA_VELOCITY;
  oldDefaultAG = rsd->getSystemParams().control.Omni2001.DEFAULT.ANGULAR_GAIN;
  oldLatencyFrames = rsd->getSystemParams().general.LATENCY_FRAMES;

  // Don't put any limit on the rotational velocity:
  rsd->getMutableSystemParams().control.Omni2001.DEFAULT.MAX_THETA_VELOCITY = HUGE_NUMBER;
  // Don't predict:
  rsd->getMutableSystemParams().general.LATENCY_FRAMES = 0;

  state = ALT_MOVING_TO_START;
  time = 0;
  stabilityThresholdTheta = HUGE_NUMBER;

  //move slowly into the initial position:
  rsd->getMutableSystemParams().control.Omni2001.DEFAULT.ANGULAR_GAIN = oldDefaultAG/2;

  //update parameters:
  sm->resetParametersInModules();

  highK = 33.0f;
  lowK = 1.0f;
  currentK = (lowK + highK)/2.0f;

  //find a robot to use:
  //(use the robot with teh smallest index who's on the field)
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    if(vision.isRobotFound(rsd->getSystemParams().general.TEAM, i))
    {
      index = i;
      return;
    }
  }
  cout << "No robot availible for latency test!\n" << endl;
  index = ROBOT0;
}

void AngularLatencyTest::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  stopRobot(index, this, *vision, rsd);
  Destination* command = rsd->getDestination(index);
  command->setControl(OMNI_NO_AVOID);
  command->setSpeed(DEFAULT_SPEED);

  float currentRotation = getRotation(index, *vision, rsd->getSystemParams());
  float currentRotVel = vision->getRobot(rsd->getSystemParams().general.TEAM, index ).getRotVel();

  time += 1/rsd->getSystemParams().general.FRAME_RATE;

  if (state == ALT_MOVING_TO_START)
  {
    command->setRotation(START_ANGLE);
    if(  (ABS(normalizeAngle(currentRotation - START_ANGLE))
          < START_ANGLE_THRESHOLD)
          &&(time > START_ANGLE_MIN_TIME))
    {
      state = ALT_TURNING_QUICKLY;
      rsd->getMutableSystemParams().control.Omni2001.DEFAULT.ANGULAR_GAIN = currentK;
      sm->resetParametersInModules();

      command->setRotation(TARGET_ANGLE);
      time = 0;
    }
  }
  else //ALT_TURNING_QUICKLY
  {
    command->setRotation(TARGET_ANGLE);
    if(time <  INTITIAL_TIME)
    {
      if(ABS(normalizeAngle(currentRotation - TARGET_ANGLE))
         > DEFINTITELY_UNSTABLE)
      {
        //failure! the robot is acting like a hyperactive child.
        updateK(false, rsd); //add Ritalin
      }
      //otherwise, set stabilityThresholdTheta to the largest angle reached during this trial:
      if (ABS(currentRotation) > stabilityThresholdTheta)
        stabilityThresholdTheta = ABS(currentRotation);
    }
    else if (time < TEST_LENGTH)
    {
      if(ABS(normalizeAngle(currentRotation - TARGET_ANGLE))
         > stabilityThresholdTheta)
      {
        //failure! the robot is still acting like a hyperactive child.
        updateK(false, rsd); //add more Ritalin
      }
      //otherwise, give it a minute to make sure it's stable
    }
    else //time >= MAX_TURN_TIME
    {
      //success!  The robot is stable
      updateK(true, rsd);
    }
  }
}

void AngularLatencyTest::endPlay(RobocupStrategyData* rsd)
{
  rsd->getMutableSystemParams().control.Omni2001.DEFAULT.MAX_THETA_VELOCITY = oldVThetaMax;
  rsd->getMutableSystemParams().control.Omni2001.DEFAULT.ANGULAR_GAIN = oldDefaultAG;
  sm->resetParametersInModules();

}


void AngularLatencyTest::updateK(bool stable, RobocupStrategyData* rsd)
{
  if(stable)
  {
    //try a higher gain:
    lowK = currentK;
  }
  else
  {
    //try a lower gain:
    highK = currentK;
  }

  currentK = (lowK + highK)/2.0f;

  if((highK - lowK) < PRECISION)
  {
    rsd->setNewPlay(NoPlay::playNum);
    sprintf(out, "Final: K=%frad/s, latency=%fs",currentK,PI/(2.0f*currentK));
    GUI_Record.debuggingInfo.addDebugMessage(out);
  }
  else
  {
    sprintf(out, "updating... K=%frad/s, latency=%fs",currentK,PI/(2.0f*currentK));
    GUI_Record.debuggingInfo.addDebugMessage(out);
    state = ALT_MOVING_TO_START;
    rsd->getMutableSystemParams().control.Omni2001.DEFAULT.ANGULAR_GAIN = oldDefaultAG/2;
    sm->resetParametersInModules();
    time = 0;
    stabilityThresholdTheta = HUGE_NUMBER;
  }
}
