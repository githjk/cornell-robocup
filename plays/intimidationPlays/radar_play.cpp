#include "info/robot/robot.h"
#include "math/dist.h"
#include "radar_play.h"
#include "../basic_actions.h"
Play RadarPlay::playNum = -1;

RadarPlay::RadarPlay(VisionModule *vm) : BasePlay(vm)
{

}

void RadarPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

  angle = 0.0f;

//  offsets[0] = 0.5f;
//  offsets[1] = 0.25f;
//  offsets[2] = 0.0f;
//  offsets[3] = -0.25f;
//  offsets[4] = -0.5f;

  float sep = 0.25f;

  offsets[0] =  2.0f*sep;
  offsets[1] =  1.0f*sep;
  offsets[2] =  0.0f;
  offsets[3] = -1.0f*sep;
  offsets[4] = -2.0f*sep;

  lookAngleOffset1[0] = PI/2;
  lookAngleOffset1[1] = PI/2;
  lookAngleOffset1[2] = 0;
  lookAngleOffset1[3] = -PI/2;
  lookAngleOffset1[4] = -PI/2;

  lookAngleOffset2[0] = PI/2;
  lookAngleOffset2[1] = PI/2;
  lookAngleOffset2[2] = PI/2;
  lookAngleOffset2[3] = PI/2;
  lookAngleOffset2[4] = PI/2;

}

ActionFunction RadarPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void RadarPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  Pair dest[NUM_ROBOTS];
  bool ready[NUM_ROBOTS];
  bool move = true;

  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
      dest[i].set(offsets[i]*COS(angle), offsets[i]*SIN(angle));	  	  
	  rsd->setMessage(i,"Showing Off");
      rsd->getDestination(i)->setPos(dest[i]);
      rsd->getDestination(i)->setRotation(lookAngleOffset1[i] + angle); 		  
      rsd->getDestination(i)->setControl(OMNI_NORMAL_ENTERBOX);
      rsd->getDestination(i)->setSpeed(DEFAULT_SPEED);
	  //---------------------
      ready[i] = false;
	  if(dist(dest[i], getLocation(i, (*vision), rsd->getSystemParams())) < 0.20f ||
         !robotFound(i, (*vision), rsd->getSystemParams()))
	  {
		  ready[i] = true;
	  }

      move = move && ready[i];
  }

  if(move)
  {
	  angle+= (PI/10);
	  if(angle > PI)
	  {
		  angle = -2*PI + angle;
	  }
  }
}