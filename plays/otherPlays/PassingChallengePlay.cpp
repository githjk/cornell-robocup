#include "PassingChallengePlay.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "Skillz/TestSkill.h"
#include <iostream.h>
#include "Skillz/TestSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"

#include "GUI/GUI.h"
Play PassingChallengePlay::playNum = -1;

void PassingChallengePlay::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  side0=1;
  side1=1;
  /*
   
   robot1ID = rsd->getRobotByPosition(AGGRESSOR);
   robot2ID = rsd->getRobotByPosition(CREATOR);
   skills1 = rsd->getStrategyModule().getSkillSet(robot1ID);
   acquireSkill1 = (AcquirePossessionSkill*)skills1->getSkill(AcquirePossessionSkill::skillNum);
   kickSkill1 = (SimpleKickSkill*)skills1->getSkill(SimpleKickSkill::skillNum);
   receiveSkill1 = (ReceivePassSkill*)skills1->getSkill(ReceivePassSkill::skillNum);
   rotateSkill1 = (RotateAroundBallSkill*)skills1->getSkill(RotateAroundBallSkill::skillNum);

   skills2 = rsd->getStrategyModule().getSkillSet(robot2ID);
   acquireSkill2 = (AcquirePossessionSkill*)skills2->getSkill(AcquirePossessionSkill::skillNum);
   kickSkill2 = (SimpleKickSkill*)skills2->getSkill(SimpleKickSkill::skillNum);
   receiveSkill2 = (ReceivePassSkill*)skills2->getSkill(ReceivePassSkill::skillNum);
   rotateSkill2 = (RotateAroundBallSkill*)skills2->getSkill(RotateAroundBallSkill::skillNum);

   state = MOVE;
   turn = LEFT;

   dest.set(POINT_X,POINT_Y);
   dest1.set(-POINT_X,POINT_Y);

  */
}


ActionFunction PassingChallengePlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void PassingChallengePlay::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  RobotIndex r0=ROBOT0;
  RobotIndex r1=ROBOT1;
  
  Pair ballLoc=getBallLocation(*vision);
  ///----
  for(RobotIndex robotID = r0; robotID < ROBOT2; robotID++){
    Pair robotLoc=getLocation(robotID,*vision,rsd->getSystemParams());
    Pair otherPos;
    int side;
    if(robotID == r0){
      otherPos=getLocation(r1,*vision,rsd->getSystemParams());
      side=side0;
    }else{
      otherPos=getLocation(r0,*vision,rsd->getSystemParams());
      side=side1;
    }

    if((ballLoc.getX() > rsd->getSystemParams().field.HALF_LINE &&
        robotID==r0) ||
       (ballLoc.getX() < rsd->getSystemParams().field.HALF_LINE &&
        robotID==r1))
    {
      //ball on our side
      if(friendlyHasPossession(robotID,rsd->getSystemParams())){
        //go kick
        if(-robotLoc.getY()*side > (rsd->getSystemParams().field.SPLIT_LINE + .5f)){
          //we're there, kick
          rsd->getDestination(robotID)->setKick(KICK_PASS);

        }else{
          //not there yet.  
          rsd->getDestination(robotID)->setDribble(DRIBBLE_DEFAULT);
          rsd->getDestination(robotID)->setVerticalDribble(V_DRIBBLE_DEFAULT);
          //head across
          float backup=.3f;
          if(robotID == r1){
            rsd->getDestination(robotID)->setPos(rsd->getSystemParams().field.HALF_LINE - backup,-side*1.0f);
            rsd->getDestination(robotID)->setRotation(0.0f);
          }else{
            rsd->getDestination(robotID)->setPos(rsd->getSystemParams().field.HALF_LINE + backup,-side*1.0f);
            rsd->getDestination(robotID)->setRotation(PI);
          }
        }
      
      }else{
        //go grab ball
        if(!rsd->getStrategyModule().getSkillSet(robotID)->getSkill(AcquirePossessionSkill::skillNum)->isInitialized()){
          rsd->getStrategyModule().getSkillSet(robotID)->getSkill(AcquirePossessionSkill::skillNum)->initialize();
        }
        rsd->getStrategyModule().getSkillSet(robotID)->getSkill(AcquirePossessionSkill::skillNum)->run();
        rsd->getDestination(robotID)->setDribble(DRIBBLE_DEFAULT);
        rsd->getDestination(robotID)->setVerticalDribble(V_DRIBBLE_DEFAULT);
        rsd->getDestination(robotID)->setKick(NO_KICK);
        rsd->getDestination(robotID)->setSpeed(GOALIE_SPEED);

      }


    }else{
        int side;
        if(ballLoc.getY() > rsd->getSystemParams().field.SPLIT_LINE){
          side=1;
        }else{
          side=-1;
        }
        if(robotID == r0){
          side0=side;
        }else{
          side1=side;
        }

        float backup=.5f;
        if(robotID == r1){
          rsd->getDestination(robotID)->setPos(rsd->getSystemParams().field.HALF_LINE - backup,otherPos.getY());
          rsd->getDestination(robotID)->setRotation(0.0f);
        }else{
          rsd->getDestination(robotID)->setPos(rsd->getSystemParams().field.HALF_LINE + backup,otherPos.getY());
          rsd->getDestination(robotID)->setRotation(PI);
        }
    }
  }

/*
  Pair aggressorPos(getLocation(robot1ID,*vision,rsd->getSystemParams()));
  Pair creatorPos(getLocation(robot2ID,*vision,rsd->getSystemParams()));
  Pair ballLoc(getBallLocation(*vision));

  if(ballLoc.getX() > rsd->getSystemParams().field.HALF_LINE)
  { 
     turn = LEFT;
  }
  else
  {
     turn = RIGHT;
  }
  
  
  if(state == KICK_RECEIVE && readTimer() > MAX_ELAPSED_TIME)
  {
    state = MOVE;
    uninitializeSkills();
  }
  if(state == MOVE && dist(getLocation(robot1ID,*vision,rsd->getSystemParams()),dest) < DISTANCE_THRESHOLD &&
     dist(getLocation(robot2ID,*vision,rsd->getSystemParams()),dest1) < DISTANCE_THRESHOLD && 
     (friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR),rsd->getSystemParams(),*vision, *rsd,true) ||
      friendlyHasPossession(rsd->getRobotByPosition(CREATOR),rsd->getSystemParams(),*vision, *rsd,true)))


  {
    state = ROTATE;
    uninitializeSkills();
  }
  if(state == ROTATE && ABS(getRotation(robot1ID,*vision,rsd->getSystemParams()) - PI  < ROTATION_THRESHOLD &&
     ABS(getRotation(robot2ID,*vision,rsd->getSystemParams()) - 0)  < ROTATION_THRESHOLD) &&
     (friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR),rsd->getSystemParams(),*vision, *rsd,true) ||
     friendlyHasPossession(rsd->getRobotByPosition(CREATOR),rsd->getSystemParams(),*vision, *rsd,true)))

  {
    state = KICK_RECEIVE;
    uninitializeSkills();
  }


  if(turn == LEFT)
  {
    if(state == KICK_RECEIVE)
    {
      kickSkill1->initialize(KICK_PASS);
      receiveSkill2->initialize(0.5f,dest1,false,true);
      kickSkill1->run();
      receiveSkill2->run();
      turn = RIGHT;
      dest.set(POINT_X,-POINT_Y);
      dest1.set(-POINT_X,-POINT_Y);
      startTimer();
      rsd->setMessage(robot1ID,"Kicking");
      rsd->setMessage(robot2ID,"Receiving");
    }
    if(state == MOVE)
    {
      
      rsd->getDestination(robot1ID)->setDribble(DRIBBLE_DEFAULT);
      rsd->getDestination(robot1ID)->setVerticalDribble(V_DRIBBLE_DEFAULT);
      if(getBallLocation(*vision).getX() < rsd->getSystemParams().field.HALF_LINE)
      {
        rsd->getDestination(robot2ID)->setPos(dest1);
        rsd->setMessage(robot2ID,"Moving to Receive");
      }  
      rsd->setMessage(robot1ID,"Moving with Ball to pass.");
      if(!friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR),rsd->getSystemParams(),*vision, *rsd,true))
      {
          acquireSkill1->initialize();
          acquireSkill1->run();
      }
      else
      {
          rsd->getDestination(robot1ID)->setPos(dest);
      }
    }

    if(state == ROTATE)
    {
      rsd->getDestination(robot2ID)->setRotation(angleBetween(creatorPos,aggressorPos));
      rsd->setMessage(robot2ID,"Rotating to receive");
      rsd->setMessage(robot1ID,"Rotating to kick");
      if(!friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR),rsd->getSystemParams(),*vision, *rsd,true))
      {
          acquireSkill1->initialize();
          acquireSkill1->run();
          rsd->getDestination(robot2ID)->setDribble(DRIBBLE_DEFAULT);
          rsd->getDestination(robot2ID)->setVerticalDribble(V_DRIBBLE_DEFAULT);
          rsd->getDestination(robot1ID)->setKick(NO_KICK);
      }
      else
      {
          rotateSkill1->initialize(dest1);
          rotateSkill1->run();
      }
    }
  }
  else
  {

    if(state == KICK_RECEIVE)
    {
      kickSkill2->initialize(KICK_PASS);
      receiveSkill1->initialize(0.5f,dest,false,true);
      kickSkill2->run();
      receiveSkill1->run();
      turn = LEFT;
      dest.set(POINT_X,POINT_Y);
      dest1.set(-POINT_X,POINT_Y);
      startTimer();
      rsd->setMessage(robot2ID,"Kicking");
      rsd->setMessage(robot1ID,"Receiving");
    }
    if(state == MOVE)
    {
      if(getBallLocation(*vision).getX() > rsd->getSystemParams().field.HALF_LINE)
      {
        rsd->getDestination(robot1ID)->setDribble(DRIBBLE_DEFAULT);
        rsd->getDestination(robot1ID)->setVerticalDribble(V_DRIBBLE_DEFAULT);
        rsd->getDestination(robot1ID)->setPos(dest);
        rsd->setMessage(robot1ID,"Moving to Receive");
      }  
      rsd->setMessage(robot2ID,"Moving with Ball to pass.");  
      if(!friendlyHasPossession(rsd->getRobotByPosition(CREATOR),rsd->getSystemParams(),*vision, *rsd,true))
      {
          acquireSkill2->initialize();
          acquireSkill2->run();
          rsd->getDestination(robot2ID)->setDribble(DRIBBLE_DEFAULT);
          rsd->getDestination(robot2ID)->setVerticalDribble(V_DRIBBLE_DEFAULT);
          rsd->getDestination(robot2ID)->setKick(NO_KICK);
      }
      else
      {
          rsd->getDestination(robot2ID)->setPos(dest1);
      }
    }

    if(state == ROTATE)
    {
      rsd->getDestination(robot1ID)->setRotation(angleBetween(creatorPos,aggressorPos));
      rsd->setMessage(robot1ID,"Rotating to receive");
      rsd->setMessage(robot2ID,"Rotating to kick");
      if(!friendlyHasPossession(rsd->getRobotByPosition(CREATOR),rsd->getSystemParams(),*vision, *rsd,true))
      {
          acquireSkill2->initialize();
          acquireSkill2->run();
      }
      else
      {
          rotateSkill2->initialize(dest1);
          rotateSkill2->run();
      }
    }


  }



    
*/
  

}

//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void PassingChallengePlay::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/other/PassingChallengePlayParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  ///Read the parameters
  READ_FLOAT(POINT_X);
  READ_FLOAT(POINT_Y);
  READ_FLOAT(ROTATION_THRESHOLD);
  READ_FLOAT(DISTANCE_THRESHOLD);
  READ_FLOAT(MAX_ELAPSED_TIME);
}

//======================================
///This function is used to uninitialize the skills
void PassingChallengePlay::uninitializeSkills()
{
  if(acquireSkill1->isInitialized())
    acquireSkill1->unInitialize();

  if(kickSkill1->isInitialized())
    kickSkill1->unInitialize();
  
  if(!receiveSkill1->isInitialized())
    receiveSkill1->unInitialize();

  if(acquireSkill2->isInitialized())
    acquireSkill2->unInitialize();

  if(kickSkill2->isInitialized())
    kickSkill2->unInitialize();
  
  if(!receiveSkill2->isInitialized())
    receiveSkill2->unInitialize();


}
