#include "info/robot/robot.h"
#include "math/dist.h"
#include "timing_test.h"
#include "../basic_actions.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include <iostream.h>
#include "GUI/GUI.h"
#include "main.h"
#include "logger/playback_writer.h"
Play TimingTest::playNum = -1;


TimingTest::TimingTest(VisionModule *vm) : BasePlay(vm)
{

}

void TimingTest::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

  loadValues();
  state=0;
}

///Reads in the values of the parameters from the plays's parameter text file.
void TimingTest::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Play_Params/other/TimingTestParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  
  ///Read in a params
  READ_FLOAT(START_X);
  READ_FLOAT(START_Y);
  READ_FLOAT(MID_X);
  READ_FLOAT(MID_Y);
  READ_FLOAT(END_X);
  READ_FLOAT(END_Y);
  READ_FLOAT(DISTANCE_FRACTION);
  READ_FLOAT(ROTATION_CHANGE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}

ActionFunction TimingTest::getActionFunction(Position pos)
{
  return stopRobot;
}

void TimingTest::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  RobotIndex robot = ROBOT0;

  dest=rsd->getDestination(robot);
  //show all points
  GUI_Record.debuggingInfo.setDebugPoint(robot,0,START_X,START_Y);
  GUI_Record.debuggingInfo.setDebugPoint(robot,1,MID_X,MID_Y);
  GUI_Record.debuggingInfo.setDebugPoint(robot,2,END_X,END_Y);


  //add on the current frame's time
  //actualTime[lastTest]+=(float) (vision->getFinishTime() - vision->getStartTime());

  //default control
  dest->setRotation(rsd->getDestination(robot)->getRotation() + ROTATION_CHANGE);

  /******************Trajectory Type*************************************/
  dest->setControl(OMNI_NO_AVOID);  //just trajectory, no avoidance
  //dest->setControl(OMNI_NORMAL_ENTERBOX_ANY_SPEED);  //trajectory with any final vel
  

  //go where we're supposed to go
  if (state==0){ // going to start
    rsd->setMessage(robot, "Moving to Start");
    dest->setXPos(START_X);
    dest->setYPos(START_Y);
    dest->setRotation(0.0f);
    if (dist(robot,START_X,START_Y,*vision,rsd->getSystemParams()) <.05){
      state++;
      time.markStartTime();
    }
  }
  if (state==1){ //stop for a second
    dest->setXPos(START_X);
    dest->setYPos(START_Y);
    dest->setRotation(0.0f);

    if(time.getElapsedTime()>=1.0){
      state++;
    }
  }
  if (state==2){ //head toward mid point
    rsd->setMessage(robot, "Running test...");
    dest->setXPos(MID_X);
    dest->setYPos(MID_Y);
    if (dist(robot,START_X,START_Y,*vision,rsd->getSystemParams()) >= DISTANCE_FRACTION * dist(START_X,START_Y,MID_X,MID_Y)){
      //start actual test
      state++;
      sm->resetFrame();
      dest->setXPos(END_X);
      dest->setYPos(END_Y);


//      predTime=rsd->getSystemParams().trajectoryModule->findPathTime(vision->getRobot(rsd->getSystemParams().TEAM,robot),*dest);
      predTime = 0.0f;
      GUI_Record.debuggingInfo.addDebugMessage("fix me! line 125, timing_test.cpp");
      time.markStartTime();
    }
  }
  if (state==3){ //head toward end point
    dest->setXPos(END_X);
    dest->setYPos(END_Y);
    if(dist(robot,END_X,END_Y,*vision,rsd->getSystemParams()) <.08){
      //made it to end point, save actual time, output results
      state++;
      actualTime=(float)time.getElapsedTime();
      char output[200];
      sprintf(output, "pred: %.2f, actual: %.2f, error: %.2f%%",
              predTime, actualTime,(predTime - actualTime)*100/actualTime); 
      rsd->setMessage(robot, output);
      GUI_Record.debuggingInfo.addDebugMessage(output);
	  writePlaybackSequence("TimingTest.brml",false);
    }
  }
  if (state==4){ // done.  sit here.
    dest->setXPos(END_X);
    dest->setYPos(END_Y);
    dest->setRotation(0.0f);
  }
}
