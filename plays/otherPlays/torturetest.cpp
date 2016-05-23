/*
 *	torturetest.h
 *  Makes the robots run laps while dribbling and kicking
 */

#include "torturetest.h"
#include "info/robot/robot.h"
Play TortureTest::playNum = -1;

TortureTest::TortureTest(VisionModule *vm) : BasePlay(vm)
{

}

void TortureTest::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  for (RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++)
  {
    testState[i] = testUp;
  }
}

const float DIST_FROM_TARGET_WALL = 0.30f;//0.25f;
const float DIST_FROM_OTHER_WALL = 0.30f;
const float THRESH_DIST_FROM_WALL = 0.55f;

void TortureTest::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++)
  {
    if (rsd->getPositionOfRobot(robot) != NO_POSITION) {
      Destination* command = rsd->getDestination(robot);
      command->setSpeed(DEFAULT_SPEED);
      command->setControl(OMNI_NORMAL_ENTERBOX);
      const SystemParameters& p = rsd->getSystemParams();

      switch(testState[robot])
      {
      case testUp:
        rsd->setMessage(robot, "Running Up the field, dribble");
        command->setPos(p.field.THEIR_GOAL_LINE - DIST_FROM_TARGET_WALL, p.field.LEFT_SIDE_LINE - DIST_FROM_OTHER_WALL);
        command->setRotation(0);
				command->setKick(NO_KICK);
				command->setDribble(FAST_DRIBBLE);
				command->setVerticalDribble(FAST_V_DRIBBLE);
        if(getXLocation(robot, *vision, p) > p.field.THEIR_GOAL_LINE - THRESH_DIST_FROM_WALL)
        {
          testState[robot] = testRight;
        }
        break;
      case testRight:
        rsd->setMessage(robot, "Running right,kicking,dribbling");
        command->setPos(p.field.THEIR_GOAL_LINE - DIST_FROM_OTHER_WALL, p.field.RIGHT_SIDE_LINE + DIST_FROM_TARGET_WALL);
        command->setRotation(-PI/2);
				command->setKick(KICK_SHOT);
				command->setDribble(FAST_DRIBBLE);
				command->setVerticalDribble(FAST_V_DRIBBLE);
        if(getYLocation(robot, *vision, p) < p.field.RIGHT_SIDE_LINE + THRESH_DIST_FROM_WALL)
        {
          testState[robot] = testDown;
        }
        break;
      case testDown:
        rsd->setMessage(robot, "Running down the field,kicking");
        command->setPos(p.field.OUR_GOAL_LINE + DIST_FROM_TARGET_WALL, p.field.RIGHT_SIDE_LINE + DIST_FROM_OTHER_WALL);
        command->setRotation(PI);
				command->setKick(KICK_SHOT);
				command->setDribble(NO_DRIBBLE);
				command->setVerticalDribble(NO_V_DRIBBLE);				
        if(getXLocation(robot, *vision, p) < p.field.OUR_GOAL_LINE + THRESH_DIST_FROM_WALL)
        {
          testState[robot] = testLeft;
        }
        break;
      case testLeft:
        rsd->setMessage(robot, "Running left");
        command->setPos(p.field.OUR_GOAL_LINE + DIST_FROM_OTHER_WALL, p.field.LEFT_SIDE_LINE - DIST_FROM_TARGET_WALL);
        command->setRotation(PI/2);
				command->setKick(NO_KICK);
				command->setDribble(NO_DRIBBLE);
				command->setVerticalDribble(NO_V_DRIBBLE);	
        if(getYLocation(robot, *vision, p) > p.field.LEFT_SIDE_LINE - THRESH_DIST_FROM_WALL)
        {
          testState[robot] = testUp;
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


