#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "math/angle.h"
#include "math/dist.h"
#include "dribbleAngle.h"
#include "hires_timer.h"
#include "../basic_actions.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"

#include "GUI/GUI.h"
Play DribbleAngleTest::playNum = -1;

DribbleAngleTest::DribbleAngleTest(VisionModule *vm) : BasePlay(vm)
{

}


void DribbleAngleTest::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  robot = ROBOT0;
  rsd->setMessage(robot, "using this robot");

  skills = rsd->getStrategyModule().getSkillSet(robot);
  moveSkill= (MoveToBallSkill *)skills->getSkill(MoveToBallSkill::skillNum);
  spinSkill= (SpinAroundBallSkill *)skills->getSkill(SpinAroundBallSkill::skillNum);

  startPos.setX(-.8f);
  startPos.setY(0.0f);
  endPos.setX(.8f);
  endPos.setY(0.0f);
  started = false;
  state = 0;

  highAngle=PI/4.0f;
  lowAngle=0.0f;
  angle=0.0f;
  moveSkill->initialize();
}

ActionFunction DribbleAngleTest::getActionFunction(Position pos)
{
  return stopRobot;
}

void DribbleAngleTest::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  GUI_Record.debuggingInfo.setDebugPoint(robot,0,startPos);
  GUI_Record.debuggingInfo.setDebugPoint(robot,1,endPos);
  
  if(!started){
    //get the ball and go to start position

    switch(state){
    case 0: //get ball
      //go get ball
      rsd->setMessage(robot, "Going to Ball");
      if (moveSkill->isValid()) moveSkill->run();

      if(friendlyHasPossession(robot,rsd->getSystemParams(),*vision, *rsd, false)){
        state++;
        spinSkill->initialize(startPos);
      }
      break;
    case 1: //face start
      rsd->setMessage(robot, "Turning to start point");
      if(spinSkill->isValid()) spinSkill->run();
      if(angleDifference( getRotation(robot,*vision,rsd->getSystemParams()),
                          angleBetween(startPos,getLocation(robot,*vision,rsd->getSystemParams())))<=.001){
        //facing right way
        state++;
      }
      break;
    case 2: //go to start
      rsd->setMessage(robot, "Moving to start point");
      if (dist(robot, startPos,*vision,rsd->getSystemParams())<.01){
        //we've arrived
        state++;
        spinSkill->initialize(endPos);
      }
      rsd->getDestination(robot)->setPos(startPos);
      rsd->getDestination(robot)->setDribble(FAST_DRIBBLE);
      rsd->getDestination(robot)->setVerticalDribble(FAST_V_DRIBBLE);

      break;
    case 3://face upfield
      rsd->setMessage(robot, "Turning to upfield");
      if(spinSkill->isValid()) spinSkill->run();
      
      if(fabs(getRotation(robot,*vision,rsd->getSystemParams())) <= .01f)
      {
        started=true;
        state=0;
      }
      break;
    }
  }else{
    //we're running the test
    switch(state){
    case 0: //position for correct angle
      rsd->setMessage(robot, "Setting Angle");
      rsd->getDestination(robot)->setRotation(getRotation(robot,*vision,rsd->getSystemParams())-.001f);
      rsd->getDestination(robot)->setDribble(FAST_DRIBBLE);
      rsd->getDestination(robot)->setVerticalDribble(FAST_V_DRIBBLE);
      if(getRotation(robot,*vision,rsd->getSystemParams())<=angle){
        state++;
        rsd->getDestination(robot)->setRotation(angle);
        timer.markStartTime();
      }
      break;
    case 1: //run downfield
      rsd->setMessage(robot, "Running Downfield");
      rsd->getDestination(robot)->setRotation(angle);
      rsd->getDestination(robot)->setDribble(FAST_DRIBBLE);
      rsd->getDestination(robot)->setVerticalDribble(FAST_V_DRIBBLE);
      if(timer.getElapsedTime()>1.0f)rsd->getDestination(robot)->setPos(endPos);
      if(!friendlyHasPossession(robot,rsd->getSystemParams(),*vision, *rsd, false)){
        //we've lost the ball
        state++;
        highAngle=angle;
      }else if(dist(robot, endPos, *vision,rsd->getSystemParams())<.05){
        //made it to the end with the ball
        state++;
        lowAngle=angle;
      }
      break;
    case 2: //done, set up for next run
      rsd->setMessage(robot, "Finished Run");

      started=false;
      state=0;
      angle=(lowAngle+highAngle)/2;
      moveSkill->initialize();
      sprintf(outMessage,"Adjusting to try angle: %f",angle);
      GUI_Record.debuggingInfo.addDebugMessage(outMessage);
      break;
    }
  }
}
