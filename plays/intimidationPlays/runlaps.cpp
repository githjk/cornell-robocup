/*
 *	runlaps.cpp
 *  Makes the robots run laps
 */

#include "runlaps.h"
#include "info/robot/robot.h"
Play RunLaps::playNum = -1;

RunLaps::RunLaps(VisionModule *vm) : BasePlay(vm)
{

}


void RunLaps::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  for (RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++)
  {
    lapState[i] = runningUp;
  }
}

const float DIST_FROM_TARGET_WALL = 0.30f;//0.25f;
const float DIST_FROM_OTHER_WALL = 0.30f;
const float THRESH_DIST_FROM_WALL = 0.55f;

void RunLaps::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++)
  {
    if (rsd->getPositionOfRobot(robot) != NO_POSITION) {
      Destination* command = rsd->getDestination(robot);
      command->setSpeed(DEFAULT_SPEED);
      command->setControl(OMNI_NORMAL_ENTERBOX);
      const SystemParameters& p = rsd->getSystemParams();

      switch(lapState[robot])
      {
      case runningUp:
        rsd->setMessage(robot, "Running Up the field");
        command->setPos(p.field.THEIR_GOAL_LINE - DIST_FROM_TARGET_WALL, p.field.LEFT_SIDE_LINE - DIST_FROM_OTHER_WALL);
        command->setRotation(0);
        if(getXLocation(robot, *vision, p) > p.field.THEIR_GOAL_LINE - THRESH_DIST_FROM_WALL)
        {
          lapState[robot] = runningRight;
        }
        break;
      case runningRight:
        rsd->setMessage(robot, "Running right");
        command->setPos(p.field.THEIR_GOAL_LINE - DIST_FROM_OTHER_WALL, p.field.RIGHT_SIDE_LINE + DIST_FROM_TARGET_WALL);
        command->setRotation(-PI/2);
        if(getYLocation(robot, *vision, p) < p.field.RIGHT_SIDE_LINE + THRESH_DIST_FROM_WALL)
        {
          lapState[robot] = runningDown;
        }
        break;
      case runningDown:
        rsd->setMessage(robot, "Running down the field");
        command->setPos(p.field.OUR_GOAL_LINE + DIST_FROM_TARGET_WALL, p.field.RIGHT_SIDE_LINE + DIST_FROM_OTHER_WALL);
        command->setRotation(PI);
        if(getXLocation(robot, *vision, p) < p.field.OUR_GOAL_LINE + THRESH_DIST_FROM_WALL)
        {
          lapState[robot] = runningLeft;
        }
        break;
      case runningLeft:
        rsd->setMessage(robot, "Running left");
        command->setPos(p.field.OUR_GOAL_LINE + DIST_FROM_OTHER_WALL, p.field.LEFT_SIDE_LINE - DIST_FROM_TARGET_WALL);
        command->setRotation(PI/2);
        if(getYLocation(robot, *vision, p) > p.field.LEFT_SIDE_LINE - THRESH_DIST_FROM_WALL)
        {
          lapState[robot] = runningUp;
        }
        break;
      default:
        rsd->setMessage(robot, "internal error in runLaps");
        break;
      }

      command->setRotation(0);
    }
  }
}


