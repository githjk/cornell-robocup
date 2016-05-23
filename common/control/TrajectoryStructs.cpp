//====================================================================
//  TrajectoryStructs.cpp
//
//  Classes for Trajectory part
//====================================================================

#include "TrajectoryStructs.h"
#include <stdio.h>

void ObjectPath::clearPath()
{
  count = 0;
}

bool ObjectPath::get(int step, ObjectPosVel & nextStep)
{
  if (step >= count) return false;

  nextStep.xPos     = path[step].xPos;
  nextStep.yPos     = path[step].yPos;
  nextStep.rotation = path[step].rotation;
  nextStep.xVel     = path[step].xVel;
  nextStep.yVel     = path[step].yVel;
  nextStep.rotVel   = path[step].rotVel;

//  memcpy(&nextStep, &path[step], sizeof(ObjectPosVel));
  return true;
}
  
bool ObjectPath::set(int step, const ObjectPosVel & nextStep)
{
  if (step >= count) return false;

  path[step].xPos     = nextStep.xPos;
  path[step].yPos     = nextStep.yPos;
  path[step].rotation = nextStep.rotation;
  path[step].xVel     = nextStep.xVel;
  path[step].yVel     = nextStep.yVel;
  path[step].rotVel   = nextStep.rotVel;

//  memcpy(&path[step], &nextStep, sizeof(ObjectPosVel));
  return true;
}

bool ObjectPath::set(int step, const float xPos, const float yPos, const float rotation,
            const float xVel, const float yVel, const float rotVel)
{
  if (step >= count) return false;

  path[step].xPos     = xPos;
  path[step].yPos     = yPos;
  path[step].rotation = rotation;
  path[step].xVel     = xVel;
  path[step].yVel     = yVel;
  path[step].rotVel   = rotVel;
  return true;
}

bool ObjectPath::addStep(const ObjectPosVel & nextStep)
{
  if (MAX_TRAJECTORY_STEPS == count) return false;

  path[count].xPos     = nextStep.xPos;
  path[count].yPos     = nextStep.yPos;
  path[count].rotation = nextStep.rotation;
  path[count].xVel     = nextStep.xVel;
  path[count].yVel     = nextStep.yVel;
  path[count].rotVel   = nextStep.rotVel;

//  memcpy(&path[count], &nextStep, sizeof(ObjectPosVel));
  count++;
  return true;
}

bool ObjectPath::addStep(const float xPos, const float yPos, const float rotation,
            const float xVel, const float yVel, const float rotVel)
{
  if (MAX_TRAJECTORY_STEPS == count) return false;

  path[count].xPos      = xPos;
  path[count].yPos      = yPos;
  path[count].rotation  = rotation;
  path[count].xVel      = xVel;
  path[count].yVel      = yVel;
  path[count].rotVel    = rotVel;
  count++;
  return true;
}


bool ObjectPath::writeToFile(const char * fname)
{
    FILE * f = fopen(fname, "a");
    ObjectPosVel nextStep;

    if(!f) return false;

    fprintf(f,"%d\n",count);
    for (int i=0; i < count; i++)
    {
      get(i, nextStep);
      fprintf(f,"(%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f)\n",
        nextStep.xPos,
        nextStep.yPos,
        nextStep.rotation,
        nextStep.xVel,
        nextStep.yVel,
        nextStep.rotVel
      );
    }
    fclose(f);
    return true;
}


