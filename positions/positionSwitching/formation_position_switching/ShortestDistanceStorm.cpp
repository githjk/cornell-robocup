#include "ShortestDistanceStorm.h"

//========================================
ShortestDistanceStorm::ShortestDistanceStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList1[] = {DEFENDER,
                       AGGRESSOR,
                       SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR,
                       CREATOR};
  clouds[0] = new Cloud(pList1, 6);

  Position pList2[] = {BLOCKER};
  clouds[1] = new Cloud(pList2, 1);
                                                           
}
//========================================
void ShortestDistanceStorm::preformSwitching(VisionModule* vm,
                                        SystemParameters* params)
{ 
  //-----
  //Get latest strategy data and vision data required for tests
  RobocupStrategyData* sd = strategy->getCurrentRoboCupFrame();
  VisionData* vision = vm->getCurrentFrame();
  //-----
  //Attempt to get robot id's
  rID[0] = sd->getRobotByPosition(CREATOR);
  rPosition[0] = CREATOR;
  //=====
  rID[1] = sd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  rPosition[1] = SPECIAL_OP_DEFENDER;
  if(rID[1] == NO_ROBOT)
  {
    rID[1] = sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
    rPosition[1] = SPECIAL_OP_AGGRESSOR;
  }
  if(rID[1] == NO_ROBOT)
  {
    rID[1] = sd->getRobotByPosition(SPECIAL_OP_CREATOR);
    rPosition[1] = SPECIAL_OP_CREATOR;
  }
  //=====  
  rID[2] = sd->getRobotByPosition(AGGRESSOR);
  rPosition[2] = AGGRESSOR;
  //=====
  rID[3] = sd->getRobotByPosition(DEFENDER);
  rPosition[3] = DEFENDER;
  //=====
  //Get robot positions
  if(rID[0] != NO_ROBOT)
    rLocation[0] = getLocation(rID[0], *vision, *params);  
  if(rID[1] != NO_ROBOT)
    rLocation[1] = getLocation(rID[1], *vision, *params);  
  if(rID[2] != NO_ROBOT)
    rLocation[2] = getLocation(rID[2], *vision, *params);  
  if(rID[3] != NO_ROBOT)
    rLocation[3] = getLocation(rID[3], *vision, *params);  
  //====================================
  //======= Conditional Switches =======
  //====================================    
  bool stop = false;
  
  for(int i=0; i<NUM_ROBOTS-1; i++)
  {
    if(stop)
      break;
    
    if(rID[i] != NO_ROBOT)
    {
      for(int j=i+1; j<NUM_ROBOTS-1; j++)
      {
        if(rID[j] != NO_ROBOT)
        {
          //get both destinations
          rDestI = strategy->getCurrentFrame()->getDestination(rID[i])->getPos();
          rDestJ = strategy->getCurrentFrame()->getDestination(rID[j])->getPos();
          
          //if it's shorter for them to switch
          float dist1 = rLocation[i].squareDistanceTo(rDestI);
          float dist2 = rLocation[j].squareDistanceTo(rDestJ);
          float dist3 = rLocation[i].squareDistanceTo(rDestJ);
          float dist4 = rLocation[j].squareDistanceTo(rDestI);
          
          //if swapping makes it faster, then swap!
          if((dist3 + dist4) < (dist1 + dist2))
          {
            switchPositions(rPosition[i], rPosition[j]);
            stop = true;
            break;
          }
        }
      }
    }
  }
}
//========================================
