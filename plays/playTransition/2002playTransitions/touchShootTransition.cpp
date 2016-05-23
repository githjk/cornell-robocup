#include "TouchShootTransition.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"
#include "common/util/shoot/shoot.h"
#include "info/ball/ball.h"

#include "plays/2002plays/upfieldDefense.h"
#include "plays/2002plays/SingleAssistOffense.h"
#include "plays/baseplay.h"
#include "GUI/GUI.h"
void TouchShootTransition::executePlayTransition(VisionData& field, RobocupStrategyData* rsd, 
			  									        BasePlay* currentPlay)
{
  PlayState state = rsd->getStrategyModule().playbook->getPlay(rsd->getCurrentPlay())->getPlayState();
  RobotIndex receiverID;
  if(state == PASS_TO_SPECIAL_OP){
    receiverID = rsd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
    GUI_Record.debuggingInfo.addDebugMessage("Passing to Special Op");
  }else{
    receiverID = rsd->getRobotByPosition(CREATOR);
  }
  //-----------------------------------------------
  //if no creator, in big trouble, go to upfield defense
  if(receiverID == NO_ROBOT)
	{
  		setPlay(UpfieldDefense::playNum, rsd);
      return;
	}
  //-----------------------------------------------
  //determine if the ball is moving away from the creator
  Pair ballPos = getBallLocation(field);
  
  Pair ballNextPos(ballPos.getX() + ballXVel(field),
                   ballPos.getY() + ballYVel(field));

  //If we've received the ball, go into single assist.
  if(friendlyHasPossession(receiverID,rsd->getSystemParams(),field,*rsd,false)){
    setPlay(SingleAssistOffense::playNum, rsd);
  }

  //see if the ball is moving away from the creator.
  bool ballMovingAway;
  if(getBallSpeed(field) < 0.001f)
    ballMovingAway = false;
  else
  {
    float ballVelAngle = angleBetween(ballPos,
                                      ballNextPos);

    float ballRobotAngle = angleBetween(ballPos,
                                        getLocation(receiverID,
                                                    field,
                                                    rsd->getSystemParams()));
    ballMovingAway = (
                      (ABS(angleDifference(ballVelAngle, ballRobotAngle)) > PI/2.0f) &&
                       getBallSpeed(field) >= MIN_BALL_SPEED
                     );
  }

  //-----------------------------------------------
  //If the time threshold is past OR 
  //the ball is moving away from the creator OR
  //the ball ismoving very slowly and aggressor and creator don't have possession
  if(
        (currentPlay->readTimer() >= MAX_ELAPSED_TIME) || 
        ballMovingAway
    )
  {
    setPlay(UpfieldDefense::playNum, rsd);
  }
  //-----------------------------------------------
}



//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void TouchShootTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/TouchShootTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(MAX_ELAPSED_TIME);
  READ_FLOAT(MIN_BALL_SPEED);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

