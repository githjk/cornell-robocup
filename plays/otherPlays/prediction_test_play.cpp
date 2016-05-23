/*
 *	prediction_test_play.cpp
 *  Makes the robots run laps, wait at corners, face different directions
 *	for prediction error data collection.
 */

#include "prediction_test_play.h"
#include "info/robot/robot.h"
#include "math/cmath.h"
Play PredictionTestPlay::playNum = -1;

PredictionTestPlay::PredictionTestPlay(VisionModule *vm) : BasePlay(vm)
{

}

void PredictionTestPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  for (RobotIndex i = ROBOT0; i < NUM_ROBOTS; i++)
  {
    testLapState[i] = runningUpLong;
		testWaitState[i] = running;
		waitCounter = 0;
		rot = 0.0f;
  }
}

const float DIST_FROM_TARGET_WALL = 0.30f;//0.25f;
const float DIST_FROM_OTHER_WALL = 0.30f;
const float THRESH_DIST_FROM_WALL = 0.55f;
const int MAX_WAIT = 100;	//wait for duration of 1000 calls to executePlay.

void PredictionTestPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  for (RobotIndex robot = ROBOT0; robot < NUM_ROBOTS; robot++)
  {
    if (rsd->getPositionOfRobot(robot) != NO_POSITION) {
      Destination* command = rsd->getDestination(robot);
      command->setSpeed(DEFAULT_SPEED);
      command->setControl(OMNI_NORMAL_ENTERBOX);
      const SystemParameters& p = rsd->getSystemParams();

      switch(testLapState[robot])
      {
      case runningUpLong:
				if( testWaitState[robot] == waiting )
				{
					if( waitCounter < MAX_WAIT )
					{
						waitCounter++;
						rsd->setMessage(robot, "Waiting...");
						command->setRotation(rot);
					}
					else
					{
						testLapState[robot] = runningRightCross;
						testWaitState[robot] = running;
						waitCounter = 0;
					}
				}
				else
				{
					rsd->setMessage(robot, "Running Up the field");
					command->setPos(p.field.THEIR_GOAL_LINE - DIST_FROM_TARGET_WALL, p.field.LEFT_SIDE_LINE - DIST_FROM_OTHER_WALL);
					command->setRotation(rot);
					if(getXLocation(robot, *vision, p) > p.field.THEIR_GOAL_LINE - THRESH_DIST_FROM_WALL)
						testWaitState[robot] = waiting;
				}
        break;
      case runningRightCross:
				if( testWaitState[robot] == waiting )
				{
					if( waitCounter < MAX_WAIT )
					{
						waitCounter++;
						rsd->setMessage(robot, "Waiting...");
						command->setRotation(rot);
					}
					else
					{
						testLapState[robot] = runningDownLong;
						testWaitState[robot] = running;
						waitCounter = 0;
					}
				}
				else
				{
					rsd->setMessage(robot, "Running right");
					command->setPos(p.field.THEIR_GOAL_LINE - DIST_FROM_OTHER_WALL, p.field.RIGHT_SIDE_LINE + DIST_FROM_TARGET_WALL);
					command->setRotation(rot);
					if(getYLocation(robot, *vision, p) < p.field.RIGHT_SIDE_LINE + THRESH_DIST_FROM_WALL)
						testWaitState[robot] = waiting;
				}
        break;
      case runningDownLong:
				if( testWaitState[robot] == waiting )
				{
					if( waitCounter < MAX_WAIT )
					{
						waitCounter++;
						rsd->setMessage(robot, "Waiting...");
						command->setRotation(rot);
					}
					else
					{
						testLapState[robot] = runningLeftCross;
						testWaitState[robot] = running;
						waitCounter = 0;
					}
				}
				else
				{
					rsd->setMessage(robot, "Running down the field");
					command->setPos(p.field.OUR_GOAL_LINE + DIST_FROM_TARGET_WALL, p.field.RIGHT_SIDE_LINE + DIST_FROM_OTHER_WALL);
					command->setRotation(rot);
					if(getXLocation(robot, *vision, p) < p.field.OUR_GOAL_LINE + THRESH_DIST_FROM_WALL)
						testWaitState[robot] = waiting;
				}
        break;
      case runningLeftCross:
				if( testWaitState[robot] == waiting )
				{
					if (waitCounter < MAX_WAIT )
					{
						waitCounter++;
						rsd->setMessage(robot, "Waiting...");
						command->setRotation(rot);
					}
					else
					{
						testLapState[robot] = runningUpLong;
						testWaitState[robot] = running;
						waitCounter = 0;
						//finished lap, so turn 45 degrees,and do another
						rot += PI/4;
						normalizeAngle(rot);
					}
				}
				else
				{
					char output[200];
					sprintf(output, "Running left, rot = %.2f", rot);
					rsd->setMessage(robot,output);
					command->setPos(p.field.OUR_GOAL_LINE + DIST_FROM_OTHER_WALL, p.field.LEFT_SIDE_LINE - DIST_FROM_TARGET_WALL);
					command->setRotation(rot);
					if(getYLocation(robot, *vision, p) > p.field.LEFT_SIDE_LINE - THRESH_DIST_FROM_WALL)
						testWaitState[robot] = waiting;
				}
        break;
      default:
        rsd->setMessage(robot, "internal error in prediction_test_play");
        break;
      }
    }
  }
}


