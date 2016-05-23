#include "baseplay.h"
#include "info/robot/robot.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include <float.h>

void BasePlay::standardExecution(const VisionData& vision, RobocupStrategyData* rsd)
{
	//Initialize robot positions
  for (RobotIndex index = ROBOT0; index < NUM_ROBOTS; index++)
  {
    //initialize all destinations to current position with 0 velocity
		initializeDestination(index, vision, rsd->getSystemParams(), rsd->getDestination(index));
  }
	
	//Call each robot's action function
	for (index = ROBOT0; index < NUM_ROBOTS; index++)
  {	  
 		if(robotFound(index, vision, rsd->getSystemParams())) 
    {
			ActionFunction action = rsd->getActionFunction(index);

			action(index, this, vision, rsd);

			//Double-check the results:
			if(_isnan(rsd->getDestination(index)->getXPos()) || _isnan(rsd->getDestination(index)->getYPos()) ) 
      {
				printf("Error! The destination for robot %d was set to (%f,%f).  Don't do that.\n",
					index, 
					rsd->getDestination(index)->getXPos(),
					rsd->getDestination(index)->getYPos());
				initializeDestination(index, vision, rsd->getSystemParams(), rsd->getDestination(index));
			}
		}
		else 
    {
		//nothing to do in the else case -- the destinations have been initialied.
			rsd->setMessage(index, "Lost robot");
		}
	}
}

