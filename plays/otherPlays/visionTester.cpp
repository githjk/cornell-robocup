#include "info/robot/robot.h"
#include "math/dist.h"
#include "visionTester.h"
#include "../basic_actions.h"
#include "math/cmath.h"
Play VisionTesterPlay::playNum = -1;

VisionTesterPlay::VisionTesterPlay(VisionModule *vm) : BasePlay(vm)
{

}

void VisionTesterPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

  outOfFormation = false;

  for(int i=0; i<NUM_ROBOTS; i++)
  {
    steps[i].boxOffsetY = 0;
    steps[i].index = i;
  
    steps[i].disabled = false;
    steps[i].lostFrames = 0;
  }
}

ActionFunction VisionTesterPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void VisionTesterPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  bool readyToMoveOn = true;

  for(int i=0; i<NUM_ROBOTS; i++)
  {
    switch(i)
    {
       case 0:
         rsd->setMessage((RobotIndex)i,"Crusin =)");
         break;
       case 1:
         rsd->setMessage((RobotIndex)i,"Speeding");
         break;
       case 2:
         rsd->setMessage((RobotIndex)i,"Rolling");
         break;
       case 3:
         rsd->setMessage((RobotIndex)i,"Moving");
         break;
       case 4:
         rsd->setMessage((RobotIndex)i,"Coasting");
         break;
       default:
         rsd->setMessage((RobotIndex)i,"ERROR");
         break;
    }

    if(!outOfFormation)
    {
      rsd->getDestination((RobotIndex)i)->setControl(OMNI_NORMAL_ENTERBOX);
      rsd->getDestination((RobotIndex)i)->setSpeed(CAREFUL_SPEED);
    }
    else
    {
      rsd->getDestination((RobotIndex)i)->setControl(OMNI_NO_AVOID);
      rsd->getDestination((RobotIndex)i)->setSpeed(DEFAULT_SPEED);
    }

    //calculate destination
    Destination tempDest;
    getDest(steps[i].index, steps[i].boxOffsetY, rsd, tempDest);

    rsd->getDestination((RobotIndex)i)->setPos(tempDest.getPos());
		rsd->getDestination((RobotIndex)i)->setRotation(tempDest.getRotation());

    //if this robot is NOT close enough to his destination, we can't move on yet!
    if(dist(tempDest.getPos(), 
		        getLocation(
			                  (RobotIndex)i, 
			                  (*vision), 
                        rsd->getSystemParams()
 				               )
           ) > 0.05f &&
           robotFound((RobotIndex)i, (*vision), rsd->getSystemParams()) &&
           !steps[i].disabled
	  )
    {
      readyToMoveOn = false;
    }
  }

  //if all the robots are ready to move on...
  if(readyToMoveOn)
  {
    //if we are going into formation, get out of that :)
    if(!outOfFormation)
      outOfFormation = true;

    //increment each robots index and yStep counters...
    for(int i=0; i<NUM_ROBOTS; i++)
    {
      bool incIndex = false;
      //if not on the last rectangle, blindly increment the index counter
      if(steps[i].boxOffsetY < (ySteps-1))
        incIndex = true;
      //else, only increment index if robot is allowed to step further...
      else
      {
        if(steps[i].index < (xRes*2 - (NUM_ROBOTS-i)))
          incIndex = true;
      }

      //ok for robot to increment index counter
      if(incIndex)
      {
        steps[i].index++;

        //wrap around if necessary
        if(steps[i].index >= (xRes*2))
        {
           steps[i].index = 0;
           steps[i].boxOffsetY++;
        }
      }
    }
  }

  //if any robot is lost, record it, if lost too long, stop it!
  for(i=0; i<NUM_ROBOTS; i++)
  {
    if(!robotFound((RobotIndex)i, (*vision), rsd->getSystemParams()))
    {
      steps[i].lostFrames++;
      if(steps[i].lostFrames > maxLostFrames)
        steps[i].disabled = true;
    }
    else
    {
      steps[i].lostFrames = 0;
    }

    //if a robot has ever been disabled, always make him stop
    if(steps[i].disabled)
    {
      stopRobot((RobotIndex)i, this, (*vision), rsd);
      rsd->setMessage((RobotIndex)i,"ERROR! WAS LOST TOO LONG!");
    }
  }
}
void VisionTesterPlay::getDest(int index,
                               int yStep,
                               RobocupStrategyData* rsd,
                               Destination& result)
{
  if(index >= 0 &&
     index < xRes)
  {
     result.setXPos(rsd->getSystemParams().field.OUR_GOAL_LINE + (
                                                       index * 
                                                       (rsd->getSystemParams().field.THEIR_GOAL_LINE - 
                                                        rsd->getSystemParams().field.OUR_GOAL_LINE - 
                                                        5*rsd->getSystemParams().general.PLAYER_RADIUS) /
                                                       (xRes-1)
                                                     ) + 3.5f*rsd->getSystemParams().general.PLAYER_RADIUS);
     result.setYPos(0.0f);
  }
  else
  {
     result.setXPos(rsd->getSystemParams().field.THEIR_GOAL_LINE - (
                                                       (index-xRes) * 
                                                       (rsd->getSystemParams().field.THEIR_GOAL_LINE - 
                                                        rsd->getSystemParams().field.OUR_GOAL_LINE - 
                                                        5*rsd->getSystemParams().general.PLAYER_RADIUS) /
                                                       (xRes-1)
                                                     ) - 1.5f*rsd->getSystemParams().general.PLAYER_RADIUS);
     result.setYPos(rsd->getSystemParams().field.LEFT_SIDE_LINE - 1.5f*rsd->getSystemParams().general.PLAYER_RADIUS);
  }
  
  //now shift based on yStep
  result.setYPos(result.getYPos() + yStep * 
                                    (rsd->getSystemParams().field.RIGHT_SIDE_LINE +
                                     1.5f*rsd->getSystemParams().general.PLAYER_RADIUS
                                    ) / 
                                    (ySteps-1));

  result.setRotation(normalizeAngle(2*PI*index/(xRes*yRes)));
}
