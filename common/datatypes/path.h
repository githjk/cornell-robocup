/*
 * Path classes
 */
 
#ifndef DATATYPES_PATH_H 
#define DATATYPES_PATH_H

#include "base_types.h"
#include "assert.h"

class Path;

const int MAX_STEPS = 150;//300;

// Two-wheel robot constants
enum PathOptions {DONE=0, NORMAL_PATH=1, START_TURN=2, CONTINUE_TURN=3};
const float ACCEL_COEFF = 0.9f; // limits acceleration to a percentage of maximum accel
//const float PI = 3.1416f;
const float HALF_WHEEL_SEPARATION = 0.09f; // half the distance between robot wheels
const float FRAME_RATE = 30.0f;
const float STIFFNESS_CONSTANT= 6.0f;

// parameters for getting wheel velocities
const float MAX_ACCEL = 2.0f; // maximum achievable acceleration in m/s^2
const float MAX_VEL = 2.0f; // maximum achievable wheel velocity
const float MAX_TURN_VEL = 0.4f; //maximum wheel velocity for robots turning in place
const float DECELERATION_ANGLE = MAX_TURN_VEL*MAX_TURN_VEL/(MAX_ACCEL*HALF_WHEEL_SEPARATION); //ony decelerate at half max acceleration, start decelerating at this # of radians from end orientation
const float POSITIONAL_ACCURACY = .03f; //distance from desired position a robot can be and still be considered "there"
const float EPSILON = .01f; // small number close to zero;
const float TIME_EPSILON = .01f; // small number close to zero
const float DELTA_X = .01f; // used for rectangle approxation of integral for the length of path
const int N = 10; // number of frames to backtrack when robot is going too fast
const int MAX_FRAMES_PER_PATH = 300; //max number of frames ahead to be calculated
const int MAX_TIME_TO_DESTINATION = 2222; // time in seconds

/*****************************************
 * One-dimenstional Path (for BangBang)  *
 *****************************************/
class OneDimPath
{
  friend class Path;
  public:

    OneDimPath() { numSteps = 0; }

    inline void addStep(float z, float zVel){
      if (isFull()){
        cout << "To many steps in Path!  ignoring call to addStep()." << endl;
        return;
      }
      loc[numSteps] = z;
      vel[numSteps] = zVel;
      numSteps++;
    }

	inline bool isFull(void) { return numSteps >= MAX_STEPS; }
/*
  void createFromPoly(float a0, float a1, float a2, float a3, float a4 = 0){
  
  }
*/
  private:
    int numSteps;

    float loc[MAX_STEPS]; // z positions for the first MAX_STEPS
    float vel[MAX_STEPS]; // vz velocities for the first MAX_STEPS
  
};


/*****************************************
 * Two-dimenstional Path                 *
 *****************************************/
class Path
{
 public:

   Path() : xPath(), yPath() { }

   inline void clear(void) {
     xPath.numSteps = 0;
     yPath.numSteps = 0;
   }

   inline OneDimPath* getXPath(void) {
     return &xPath;
   }

   inline OneDimPath* getYPath(void) {
     return &yPath;
   }

   //returns location only
   inline Pair getStepLoc(int step) const {
     ASSERT((step < xPath.numSteps) & (step < yPath.numSteps),
       "Path index out of bounds.");
     Pair p(xPath.loc[step], yPath.loc[step]);
     return p;
   }

   //returns velocity only
   inline Pair getStepVel(int step) const {
     char message[60];
     sprintf(message,"num X steps=%d, num Y steps=%d,index=%d\n",xPath.numSteps,yPath.numSteps,step);
     ASSERT((step < xPath.numSteps) & (step < yPath.numSteps),
//       "Path index out of bounds.");
        message);
     if(step >= xPath.numSteps || step >= yPath.numSteps) {
       printf("Path step error\n");
     }
     Pair p(xPath.vel[step], yPath.vel[step]);
     return p;
   }

   //returns location and velocity
   inline void getStepAt(int i, Pair* location, Pair* velocity) const {
     ASSERT((i < xPath.numSteps) & (i < yPath.numSteps), "Path index out of bounds.");
     location->setX(xPath.loc[i]);
     location->setY(yPath.loc[i]);
     velocity->setX(xPath.vel[i]);
     velocity->setY(yPath.vel[i]);
   }

   inline void scale(float lengthScale, float velocityScale) {
  //   ASSERT(xPath.numSteps == yPath.numSteps, "Unmatched path lengths.");
     for (int step = 0; step < getNumSteps(); step++) {
       xPath.loc[step] *= lengthScale;
       yPath.loc[step] *= lengthScale;
       xPath.vel[step] *= velocityScale;
       yPath.vel[step] *= velocityScale;
     }
   }

   inline void scale(float lengthScaleX, float lengthScaleY,
                     float velocityScaleX, float velocityScaleY) {
  //   ASSERT(xPath.numSteps == yPath.numSteps, "Unmatched path lengths.");
     for (int step = 0; step < getNumSteps(); step++) {
       xPath.loc[step] *= lengthScaleX;
       yPath.loc[step] *= lengthScaleY;
       xPath.vel[step] *= velocityScaleX;
       yPath.vel[step] *= velocityScaleY;
     }
   }

   //NB: the steps are indexed from 0 to getNumSteps()-1
   inline int getNumSteps(void) const {
 //    ASSERT(xPath.numSteps == yPath.numSteps, "Unmatched path lengths.");
     return MIN(xPath.numSteps, yPath.numSteps);
   }

 private:
   OneDimPath xPath, yPath;
  
};


#endif // DATATYPES_PATH_H











