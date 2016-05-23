#include "info/robot/robot.h"
#include "math/dist.h"
#include "cones_play.h"
#include "../basic_actions.h"
Play ConesPlay::playNum = -1;

ConesPlay::ConesPlay(VisionModule *vm) : BasePlay(vm)
{
  
}

void ConesPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  separationDistance = 3.0f*rsd->getSystemParams().general.PLAYER_RADIUS +
                       1.0f*rsd->getSystemParams().general.OBSTACLE_CLEARANCE;

  
  for(int i = 0; i<NUM_ROBOTS; i++)
  {
    Offsets[i].setX((i-2)*separationDistance - pushBackDistance);
    Offsets[i].setY(0.0f);
  }
  
  //--
  striveFor[0].setX(-1.5f*separationDistance - pushBackDistance);
  striveFor[0].setY(separationDistance/2);
  //--
  striveFor[1].setX(-1.0f*separationDistance - pushBackDistance);
  striveFor[1].setY(separationDistance/2);
  //--
  striveFor[2].setX(-0.5f*separationDistance - pushBackDistance);
  striveFor[2].setY(separationDistance/2);
  //--
  striveFor[3].setX(-0.5f*separationDistance - pushBackDistance);
  striveFor[3].setY(0.0f);
  //--
  striveFor[4].setX(-0.5f*separationDistance - pushBackDistance);
  striveFor[4].setY(-separationDistance/2);
  //--
  striveFor[5].setX(-pushBackDistance);
  striveFor[5].setY(-separationDistance/2);
  //--
  striveFor[6].setX(0.5f*separationDistance - pushBackDistance);
  striveFor[6].setY(-separationDistance/2);
  //--
  striveFor[7].setX(0.5f*separationDistance - pushBackDistance);
  striveFor[7].setY(0.0f);
  //--
  striveFor[8].setX(0.5f*separationDistance - pushBackDistance);
  striveFor[8].setY(separationDistance/2);
  //--
  striveFor[9].setX(1.0f*separationDistance - pushBackDistance);
  striveFor[9].setY(separationDistance/2);
  //--
  striveFor[10].setX(1.5f*separationDistance - pushBackDistance);
  striveFor[10].setY(separationDistance/2);
  //--
  striveFor[11].setX(1.5f*separationDistance - pushBackDistance);
  striveFor[11].setY(0.0f);
  //--
  striveFor[12].setX(1.5f*separationDistance - pushBackDistance);
  striveFor[12].setY(-separationDistance/2);
  //--
  striveFor[13].setX(2.0f*separationDistance - pushBackDistance);
  striveFor[13].setY(-separationDistance/2);
  //--
  striveFor[14].setX(2.5f*separationDistance - pushBackDistance);
  striveFor[14].setY(-separationDistance/2);
  //--
  striveFor[15].setX(2.5f*separationDistance - pushBackDistance);
  striveFor[15].setY(0.0f);
  //--
  activeRobot = 0;
  
  while(
        !robotFound(
                    (RobotIndex)activeRobot, 
                    vision, 
                    rsd->getSystemParams()
                   )
       )
  {
    activeRobot++;
    if(activeRobot >= NUM_ROBOTS)
    { 
      activeRobot = 0;
      break;
    }
  }
  
  
  goingTo = 0; //going into formation.
}

ActionFunction ConesPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void ConesPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    rsd->setMessage(i,"Showing Off");
    
    int pushUp = 0;
    for(int k=activeRobot; k!=i; k++)
    {
      pushUp++;
      if(k >=NUM_ROBOTS-1)
        k = -1;
      if(pushUp >= NUM_ROBOTS)
        pushUp = 0;
    }
    
    rsd->getDestination(i)->setPos(Offsets[pushUp]);
    rsd->getDestination(i)->setRotation(getRotation(ROBOT0, *vision, rsd->getSystemParams()) + 0.15f);
//    rsd->getDestination(i)->setRotation(0.0f);
    rsd->getDestination(i)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(i)->setSpeed(DEFAULT_SPEED);
    //---------------------
  }
  
  Pair dest;
  if(goingTo == 0)
  {
    dest.set(Offsets[0]);
  }
  else
  { 
    dest.set(striveFor[goingTo-1]);
  }
  
  rsd->getDestination(activeRobot)->setPos(dest);
  
  if(dist(dest, getLocation((RobotIndex)activeRobot, (*vision), rsd->getSystemParams())) < 0.07)
  {
    goingTo++;
  }
  
  if(goingTo > 16)
  {
    goingTo = 0;
    activeRobot++;
    
    int counter = 0;
    while(
           !robotFound((RobotIndex)activeRobot, (*vision), rsd->getSystemParams()) &&
           counter < NUM_ROBOTS
         )
    {
      counter++;
      activeRobot++;
      if(activeRobot >= NUM_ROBOTS) //no robot is found, break out
        activeRobot = 0;
    }  
  }
}