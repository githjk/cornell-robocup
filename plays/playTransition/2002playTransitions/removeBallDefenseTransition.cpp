#include "removeBallDefenseTransition.h"
#include "common/util/math/dist.h"
#include "info/ball/ball.h"

#include "plays/2002plays/looseDefense.h"
#include "plays/2002plays/CageDefense.h"
#include "plays/2002plays/TightDefense.h"

#include "Skillz/RemoveBallSkill.h"


void RemoveBallDefenseTransition::executePlayTransition(VisionData& field, RobocupStrategyData* rsd, 
			  									        BasePlay* currentPlay)
{
	float tolerance = 1;
  RobotIndex BlockerID=rsd->getRobotByPosition(BLOCKER);
  

  if(BlockerID != NO_ROBOT)
  {
      Pair ballPos = getBallLocation(field);
      Pair blockerLoc = getLocation(BlockerID,field, rsd->getSystemParams());

	    //If threshold reached since chip kick was triggered (if skill is finished) or if ball past a certain distance from Blocker
	    //then transition to looseDefense
      if(!friendlyHasPossession(BlockerID,rsd->getSystemParams()) ||
          (dist(ballPos, blockerLoc) > MAX_DISTANCE_BETWEEN_BLOCKER_AND_BALL + 
                                       rsd->getSystemParams().general.PLAYER_RADIUS +
                                       2.0f*rsd->getSystemParams().general.BALL_RADIUS))

	    {
		    setPlay(CageDefense::playNum, rsd);
	    }
  }
  else
  {

      setPlay(LooseDefense::playNum, rsd);
  }

}



//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void RemoveBallDefenseTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/RemoveBallDefenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(MAX_ELAPSED_TIME_SINCE_CHIP_KICK);
  READ_FLOAT(MAX_DISTANCE_BETWEEN_BLOCKER_AND_BALL);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

