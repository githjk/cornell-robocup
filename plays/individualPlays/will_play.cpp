#include "info/robot/robot.h"
#include "math/dist.h"
#include "will_play.h"
#include "../basic_actions.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/math/perpendicular.h"
#include "Skillz/StripBallSkill.h"
#include "Skillz/PullBallOffWallSkill.h"
#include "Skillz/PenaltyShotUsAimAndShootSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <string>

Play WillPlay::playNum = -1;
//====================================
WillPlay::WillPlay(VisionModule *vm) : BasePlay(vm)
{
  loadValues();
  log = NULL;
  this->vm = vm;
}
//====================================
void WillPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  t = -1.0f;
  Vx = 0.0f;

  accel = true;
  decel = false;
  stop = false;
  counter = 0;
  index = -1;

  if(log != NULL)
    delete[] log;

  log = new vision_and_commands[MAX_KEPT];

}
//====================================
ActionFunction WillPlay::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void WillPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  RobotIndex activeID = rsd->getRobotByPosition(BLOCKER);
  RobotIndex passiveID = rsd->getRobotByPosition(DEFENDER);

  if(activeID != NO_ROBOT)
  {
    Destination* command1 = rsd->getDestination(activeID);
    command1->setPos(0.0f, 0.0f);
    command1->setRotation(0.0f);
  }

  if(passiveID != NO_ROBOT)
  {
    Destination* command2 = rsd->getDestination(passiveID);
    command2->setPos(0.0f, 0.0f);
    command2->setRotation(0.0f);
  }
  //==============================================
  //==============================================
  //==============================================
  /*
  t+=1.0f;
  Vx = Vmax * ((float)sin(Amax/Vmax*t/60.0f));
  float theoreticalDisplacement = 2.0f*Vmax*Vmax/Amax;

  rsd->getDestination(0)->setVel(Vx, 0.0f);
  rsd->getDestination(0)->setRotVel(-10.0f * getRotation(ROBOT0, *vision, rsd->getParams()));

  rsd->getDestination(0)->setControl(MANUAL_CONTROL);

  char message[100];
  sprintf(message, "Theoretical: %f", theoreticalDisplacement);
  rsd->setMessage(ROBOT0, message);
  */
  //==============================================
  //==============================================
  //==============================================
  /*counter++;

  //if robot behind slowdown line
  if(getLocation(ROBOT0, *vision, rsd->getSystemParams()).getX() > 0.5f && !stop)
  {
    Vx = Vx - Amax/60.0f;
    Vx = 0.0f;
    if(Vx < 0.0f)
    {
      decel = false;
      stop = true;
      //======================================================
      //log all data
      FILE *file;
      file = fopen("raffLog.txt", "w");
      if(file != NULL)
      {
        fprintf(file, "################################\n");
        fprintf(file, "#Format: \n");
        fprintf(file, "#frame_number    rawPositionX    rawPositionY    rawRotation    predictedPositionX    predictedPositionY   predictedRotation    lastPositionCommandX    lastPositionCommandY    lastRotationCommand\n");
        fprintf(file, "################################\n");
        for(int i=0; i<=index; i++)
        {
          fprintf(file, 
                  "%d    %2.5f    %2.5f    %2.5f    %2.5f    %2.5f    %2.5f    %2.5f    %2.5f    %2.5f\n",
                  i,
                  log[i].rawPosition.getX(),
                  log[i].rawPosition.getY(),
                  log[i].rawRotation,
                  log[i].predictedPosition.getX(),
                  log[i].predictedPosition.getY(),
                  log[i].predictedRotation,
                  log[i].lastPositionCommand.getX(),
                  log[i].lastPositionCommand.getY(),
                  log[i].lastRotationCommand);
        }
        fclose(file);
      }
      //======================================================
    }
    if(!decel)
    {
      decel = true;
      accel = false;
      frame2 = counter;
      x2 = getLocation(ROBOT0, *vision, rsd->getSystemParams()).getX();
    }
  }
  else if(accel)
  {    
    Vx = Vx + Amax/60.0f;
    if(Vx > Vmax && accel)
    {
      Vx = Vmax;
      accel = false;
      frame1 = counter;
      x1 = getLocation(ROBOT0, *vision, rsd->getSystemParams()).getX();
    }
  }
  if(decel)
  {
     char message[100];
     float time = (float)(frame2 - frame1 + 1);
     float realSustainedVelocity = (x2-x1) * 60.0f / time;
     sprintf(message, "CVel: %f", realSustainedVelocity);
     rsd->setMessage(ROBOT0, message);
  }
  else
  {
     rsd->setMessage(ROBOT0, "calculating");
  }

  if(stop)
  {
    Vx = 0.0f; 
  }

  rsd->getDestination(0)->setVel(Vx, 0.0f);
  rsd->getDestination(0)->setRotVel(0.0f);
  rsd->getDestination(0)->setControl(MANUAL_CONTROL);

  //==============================================
  //==============================================
  //==============================================
  */
/*

  RobotIndex i=rsd->getRobotByPosition(AGGRESSOR);
  if(i==NO_ROBOT)i=ROBOT0;
  Skill * skill=rsd->getStrategyModule().getSkillSet(i)->getSkill(PullBallOffWallSkill::skillNum);
//  Skill * skill=rsd->getStrategyModule().getSkillSet(i)->getSkill(StripBallSkill::skillNum);
  if(!skill->isInitialized()){
    skill->initialize();
  }
  skill->run();
*/
/*
if(friendlyHasPossession(ROBOT0,
                         rsd->getSystemParams(),
                         *vision,
                         *rsd,
                         true))
     rsd->setMessage(ROBOT0, "happy");
else
     rsd->setMessage(ROBOT0, "pissed");
*/

  
/*
rsd->getDestination(1)->setPos(0.0f, 0.0f);
rsd->getDestination(1)->setRotation(0.0f);
rsd->getDestination(3)->setPos(2.0f*rsd->getSystemParams().general.PLAYER_RADIUS + rsd->getSystemParams().general.OBSTACLE_CLEARANCE, 0.0f);
rsd->getDestination(3)->setRotation(0.0f);
*/
  /*
rsd->getDestination(0)->setPos(rsd->getSystemParams().field.OUR_GOALIE_BOX - 
                               2.0f*rsd->getSystemParams().general.PLAYER_RADIUS, 
                               rsd->getSystemParams().field.SPLIT_LINE);
rsd->getDestination(0)->setRotation(0.0f);
*/

/*
rsd->getDestination(3)->setPos(0.5f, rsd->getSystemParams().field.RIGHT_SIDE_LINE + 0.8f*rsd->getSystemParams().general.PLAYER_RADIUS);
*/
/*
  GUI_Record.debuggingInfo.setDebugPoint(0, 0, -1.0f, (rsd->getMutableSystemParams()).field.getLeftSideLine(-1.0f));
  GUI_Record.debuggingInfo.setDebugPoint(0, 1, -0.5f, rsd->getMutableSystemParams().field.getLeftSideLine(-0.5f));
  GUI_Record.debuggingInfo.setDebugPoint(0, 2, 0.0f, rsd->getMutableSystemParams().field.getLeftSideLine(0.0f));
  GUI_Record.debuggingInfo.setDebugPoint(0, 3, 0.5f, rsd->getMutableSystemParams().field.getLeftSideLine(0.5f));
  GUI_Record.debuggingInfo.setDebugPoint(0, 4, 1.0f, rsd->getMutableSystemParams().field.getLeftSideLine(1.0f));

  GUI_Record.debuggingInfo.setDebugPoint(1, 0, -1.0f, rsd->getMutableSystemParams().field.getRightSideLine(-1.0f));
  GUI_Record.debuggingInfo.setDebugPoint(1, 1, -0.5f, rsd->getMutableSystemParams().field.getRightSideLine(-0.5f));
  GUI_Record.debuggingInfo.setDebugPoint(1, 2, 0.0f, rsd->getMutableSystemParams().field.getRightSideLine(0.0f));
  GUI_Record.debuggingInfo.setDebugPoint(1, 3, 0.5f, rsd->getMutableSystemParams().field.getRightSideLine(0.5f));
  GUI_Record.debuggingInfo.setDebugPoint(1, 4, 1.0f, rsd->getMutableSystemParams().field.getRightSideLine(1.0f));

  GUI_Record.debuggingInfo.setDebugPoint(2, 0, rsd->getMutableSystemParams().field.getOurGoalLine(-1.0f), -1.0f);
  GUI_Record.debuggingInfo.setDebugPoint(2, 1, rsd->getMutableSystemParams().field.getOurGoalLine(-0.5f), -0.5f);
  GUI_Record.debuggingInfo.setDebugPoint(2, 2, rsd->getMutableSystemParams().field.getOurGoalLine(0.0f), 0.0f);
  GUI_Record.debuggingInfo.setDebugPoint(2, 3, rsd->getMutableSystemParams().field.getOurGoalLine(0.5f), 0.5f);
  GUI_Record.debuggingInfo.setDebugPoint(2, 4, rsd->getMutableSystemParams().field.getOurGoalLine(1.0f), 1.0f);

  GUI_Record.debuggingInfo.setDebugPoint(3, 0, rsd->getMutableSystemParams().field.getTheirGoalLine(-1.0f), -1.0f);
  GUI_Record.debuggingInfo.setDebugPoint(3, 1, rsd->getMutableSystemParams().field.getTheirGoalLine(-0.5f), -0.5f);
  GUI_Record.debuggingInfo.setDebugPoint(3, 2, rsd->getMutableSystemParams().field.getTheirGoalLine(0.0f), 0.0f);
  GUI_Record.debuggingInfo.setDebugPoint(3, 3, rsd->getMutableSystemParams().field.getTheirGoalLine(0.5f), 0.5f);
  GUI_Record.debuggingInfo.setDebugPoint(3, 4, rsd->getMutableSystemParams().field.getTheirGoalLine(1.0f), 1.0f);

  GUI_Record.debuggingInfo.setDebugPoint(4, 0, rsd->getMutableSystemParams().field.CENTER_X,
                                               rsd->getMutableSystemParams().field.CENTER_Y);
											   */
/*
  char message[100];
  sprintf(message, "robot %d", rsd->getOffensiveBallRobot());
  GUI_Record.debuggingInfo.addDebugMessage(message);
  */
/*
//-------------------
  Pair A(getLocation(rsd->getSystemParams().general.OTHER_TEAM,
                     ROBOT4,
                     *vision));
  Pair B(getBallLocation(*vision));
  Pair C(getLocation(ROBOT0, *vision, rsd->getSystemParams()));
  float result = pOffSet(A,B,C);
  char message[100];
  sprintf(message, "val: %f", result);
  rsd->setMessage(ROBOT0, message);
//-------------------
*/
/*
  float maxTime = 0.5f;
  float forwardVelocity = 2.0f;
  float sideVelocity = 1.0f;
  float rotVelocity = 1.0f;

  command->setControl(MANUAL_CONTROL);
  command->setVel(forwardVelocity, sideVelocity);
  command->setRotVel(rotVelocity);
  command->setKick(KICK_SHOT_IMMEDIATE);
  */

}
//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void WillPlay::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/individualPlays/WillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################

  READ_FLOAT(Vmax);
  READ_FLOAT(Amax);
  
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//====================================
