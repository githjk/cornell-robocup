#include "formation_tester.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/info/robot/robot.h"
#include "common/util/math/angle.h"


Play FormationTester::playNum = -1;

//==============================================
void FormationTester::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    if(robotFound(i, vision, rsd->getSystemParams()))
    {
      found[i] = true;
      location[i] = getLocation(i, vision, rsd->getSystemParams());
      rotation[i] = getRotation(i, vision, rsd->getSystemParams());
    }
    else
    {
      found[i] = false;
    }
  }

  moveOn = true;
  robotID = -1;
}
//==============================================
void FormationTester::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  if(moveOn)
  {
    robotID++;
    mode = 0;
    moveOn = false;
  }

  //for not active robots, set their dest location and rotation
  for(int i=0; i<NUM_ROBOTS; i++)
  {
    rsd->getDestination(i)->setControl(OMNI_NORMAL_ENTERBOX);
    if(i != robotID)
    {   
      rsd->getDestination(i)->setPos(location[i]);
      rsd->getDestination(i)->setRotation(rotation[i]);
      rsd->setMessage((RobotIndex)i, "Camping Out");
    }
  }

  //if robot doesn't exist, move
  while(
         robotID <= NUM_ROBOTS &&
         !found[robotID]       
       )
  {
    robotID++;
  }

  if(robotID >=0 && robotID < NUM_ROBOTS)
  {
    //rotate right
    if(mode == 0)
    {
      rsd->getDestination(robotID)->setPos(location[robotID]);

      float destAngle = normalizeAngle(rotation[robotID] - 5.0f*PI/12.0f);
      rsd->getDestination(robotID)->setRotation(destAngle);
      rsd->setMessage((RobotIndex)robotID, "Rotating Right");

      if(
          ABS(angleDifference(
                               getRotation((RobotIndex)robotID, *vision, rsd->getSystemParams()),
                               destAngle
                             )) < PI/16.0f
        )
      {
        mode++;
      }
    }
    //rotate left
    else if(mode == 1)
    {
      rsd->getDestination(robotID)->setPos(location[robotID]);

      float destAngle = normalizeAngle(rotation[robotID] + 5.0f*PI/12.0f);
      rsd->getDestination(robotID)->setRotation(destAngle);
      rsd->setMessage((RobotIndex)robotID, "Rotating Left");

      if(
          ABS(angleDifference(
                               getRotation((RobotIndex)robotID, *vision, rsd->getSystemParams()),
                               destAngle
                             )) < PI/16.0f
        )
      {
        mode++;
      }
    }
    //go back to normal
    else
    {
      rsd->getDestination(robotID)->setPos(location[robotID]);

      float destAngle = rotation[robotID];     
      rsd->getDestination(robotID)->setRotation(destAngle);
      rsd->setMessage((RobotIndex)robotID, "Rotating Back");
      if(
          ABS(angleDifference(
                               getRotation((RobotIndex)robotID, *vision, rsd->getSystemParams()),
                               destAngle
                             )) < PI/16.0f
        )
      {
        moveOn = true;
      }
    }
  }
}
//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void FormationTester::loadValues()
{
}
//====================================
