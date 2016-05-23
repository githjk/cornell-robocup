/*
 * SnowflakeTester
 */
#include "SnowflakeTester.h"
#include "common/util/math/angle.h"
#include "common/util/pair/pair.h"
#include "common/util/math/dist.h"
#include "GUI/GUI.h"


Play SnowflakeTester::playNum = -1;

SnowflakeTester::SnowflakeTester(VisionModule *vm) : BasePlay(vm)
{
  maxSteps = 12;
  dests = new Pair[maxSteps];
}

SnowflakeTester::~SnowflakeTester()
{
  delete[] dests;
}

void SnowflakeTester::initializePlay(const VisionData& vision, RobocupStrategyData* rsd) 
{
  float offset = 0.8f;

  origin.set(rsd->getSystemParams().field.HALF_LINE,rsd->getSystemParams().field.SPLIT_LINE);
  Pair first(0.8f,rsd->getSystemParams().field.SPLIT_LINE);
  for(int i=0; i<maxSteps; i++)
  {
    float angle;

    angle = 2.0f*((float)i)/((float)maxSteps)*PI;
    //make between -PI and PI, instead of 0 to 2PI
    angle = normalizeAngle(angle);
    rotateAboutPoint(first,origin,angle,dests[i]);

  }

  step = maxSteps-1; //going to first step
  mode = 2; //going out to step
}

void SnowflakeTester::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  RobotIndex robotID=ROBOT0;

  GUI_Record.debuggingInfo.setDebugPoint(0, 0, dests[step]);

  if(mode==1){
    if(dist(robotID,dests[step],*vision,rsd->getSystemParams()) <= .01f){
      //we're there
      mode=2;
    }else{
      rsd->getDestination(robotID)->setPos(dests[step]);
      rsd->setMessage(robotID,"Heading to point");
    }
  }

  if(mode==2){
    if(dist(robotID,origin,*vision,rsd->getSystemParams()) <= .01f){
      //we're there
      mode=1;

      step++;
      if(step == maxSteps) step = 0;

    }else{
      rsd->getDestination(robotID)->setPos(origin);
      rsd->setMessage(robotID,"Returning to origin");
    }
  }

  //always look forward
  rsd->getDestination(robotID)->setRotation(0.0f);

}
