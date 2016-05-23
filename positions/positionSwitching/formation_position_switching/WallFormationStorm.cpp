#include "WallFormationStorm.h"

//========================================
WallFormationStorm::WallFormationStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList0[] = {DEFENDER,
                       AGGRESSOR,
                       SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR,
                       CREATOR};
  clouds[0] = new Cloud(pList0, 6);

  Position pList1[] = {BLOCKER};
  clouds[1] = new Cloud(pList1, 1);                                                          
}
//========================================
void WallFormationStorm::preformSwitching(VisionModule* vm,
                                        SystemParameters* params)
{ 
    //-----
    //Get latest strategy data and vision data required for tests
    RobocupStrategyData* sd = strategy->getCurrentRoboCupFrame();
    VisionData* vision = vm->getCurrentFrame();
    //-----
    //order the robots based on their x cooridnates
    //BLOCKER/GOALIE
    //DEFENDER/ACTIVE_DEFENDER
    //AGGRESSOR/MIDFIELDER
    //SPECIAL_OP/MAIN_FORWARD
    //CREATOR/PASS_FORWARD

    //=====
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

    for(int i=NUM_ROBOTS-2; i>0; i--)
    {
      if(stop)
        break;
      for(int j=0; j<i; j++)
      {
        int compareID = j+1;
        while(rID[compareID] == NO_ROBOT &&
              compareID <= i)
        {
          compareID++;
        }

        if(compareID <=i &&
           rID[j] != NO_ROBOT &&
           rID[compareID] != NO_ROBOT &&
           rLocation[j].getX() < rLocation[compareID].getX() - params->general.PLAYER_RADIUS)
        {
          switchPositions(rPosition[j], rPosition[compareID]);
          stop = true;
          break;
        }
      }
    }   
}
//========================================
