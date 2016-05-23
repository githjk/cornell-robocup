//====================================================================
//  TrajectoryStructs.h
//
//  Classes for trajectory part
//====================================================================

#ifndef TRAJECTORYSTRUCTS_H
#define TRAJECTORYSTRUCTS_H

#define MAX_TRAJECTORY_STEPS 60

struct ObjectPosVel
{
	float xPos, yPos, rotation;
  float xVel, yVel, rotVel;
};

class ObjectPath
{

private:
  ObjectPosVel path[MAX_TRAJECTORY_STEPS];
  int count;

public:

  ObjectPath() { count = 0; };
  void clearPath();

  int getNumSteps() const { return count; }

  bool get(int step, ObjectPosVel & nextStep);
  
  bool set(int step, const ObjectPosVel & nextStep);
  bool set(int step, const float xPos, const float yPos, const float rotation,
              const float xVel, const float yVel, const float rotVel);

  bool addStep(const ObjectPosVel & nextStep);
  bool addStep(const float xPos, const float yPos, const float rotation,
              const float xVel, const float yVel, const float rotVel);
  
  bool writeToFile(const char * fname);
};

#endif //TRAJECTORYSTRUCTS_H
