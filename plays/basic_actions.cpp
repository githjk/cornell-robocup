/*
*   basic_actions.cpp
*/
#include "basic_actions.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"

// Stops the robot's motion
void stopRobot(RobotIndex ID, BasePlay* play, 
                const VisionData& field, RobocupStrategyData* sd)
{
  initializeDestination(ID, field, sd->getSystemParams(), sd->getDestination(ID));

  sd->getDestination(ID)->setControl(MANUAL_CONTROL);

  sd->setMessage(ID, "Stopping the robot" );

}