#include "Storm.h"
#include "iostream.h"
#include "common/datatypes/ai.h"
#include "GUI/GUI.h"
#include "main.h" //so we have access to sm

//========================================
Storm::Storm(RoboCupStrategyModule* strategy)

{
  this->strategy = strategy;

  clouds = new Cloud*[NUM_ROBOTS+1];
  numClouds = NUM_ROBOTS;
  for(int i=0; i<NUM_ROBOTS; i++)
  {
    clouds[i] = NULL;
  }
}
//========================================
Storm::~Storm()
{
  for(int i=0; i<numClouds; i++)
  {
    delete clouds[i];
  }
  delete[] clouds;
  clouds = NULL;
}
//========================================
void Storm::switchPositions(Position pos1, Position pos2)
{
  //both pos's are same, no switch necessary
  if (pos1 == pos2)
	  return;

  //check to see if both robots 
  //are in the same cloud
  //if not reject switch
  
  //find cloud pos1 is in
  int cloud1 = -1;
  for(int i=0; i<numClouds; i++)
  {
    if(clouds[i] != NULL &&
       clouds[i]->positionInCloud(pos1))
    {
        cloud1 = i;
        break;
    }
  }

  //if pos1 not in any cloud, reject switch and complain
  if(cloud1 == -1)
  {
    char error[100];
    sprintf(error, "Error! Attempt to swap position %s without first inserting it into a cloud!", 
            sm->getPositionText(pos1));

    GUI_Record.debuggingInfo.addDebugMessage(error);
    return;
  }

  char* p1 = sm->getPositionText(pos1);
  char* p2 = sm->getPositionText(pos2);

  //if pos2 not in this cloud, reject switch and complain
  if(!clouds[cloud1]->positionInCloud(pos2))
  {
    char error[100];
    sprintf(error, 
            "Error! Attempt to swap positions %s and %s that are not in the same cloud.", 
            sm->getPositionText(pos1), 
            sm->getPositionText(pos2));

    GUI_Record.debuggingInfo.addDebugMessage(error);
    return;
  }

  //else swap ok, go ahead    
  RobocupStrategyData* sd = strategy->getCurrentRoboCupFrame();
  sd->swapPositions(pos1, pos2);
  
  //unititalize all skills for these robots
  RobotIndex ID1 = sd->getRobotByPosition(pos1);
  RobotIndex ID2 = sd->getRobotByPosition(pos2);
  if(ID1 != NO_ROBOT)
    sd->getStrategyModule().getSkillSet(ID1)->unitializeSkillz();
  if(ID2 != NO_ROBOT)
    sd->getStrategyModule().getSkillSet(ID2)->unitializeSkillz();
}
//========================================
void Storm::preformSwitching(VisionModule* vm,
                             SystemParameters* params)
{
}
//========================================
