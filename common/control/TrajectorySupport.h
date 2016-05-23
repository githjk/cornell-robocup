#ifndef TRAJECTORY_SUPPORT_H
#define TRAJECTORY_SUPPORT_H

#include "TrajectoryStructs.h"

inline float sqr(float x) { return x*x; }
inline int sgn(float a) { return (a>0)?1:((a<0)?-1:0); }
inline bool isZero(float f) { return (fabs(f)<0.00001)?true:false; }
inline bool nonZero(float f) { return !isZero(f); }
inline float sgnf(float a) { return (a>0)?1.0f:((a<0)?-1.0f:0.0f); }

#define or || 

#ifndef PI
  #define PI	3.1415926f
#endif

//************************************************************************
float distance(float x1, float y1, float x2, float y2);

//************************************************************************
void bangBangPathProperties(float z0,
                            float zf,
                            float vz0,
                            float ubar,
                            float &t1,
                            float &t2,
                            float &tf,
                            float &u);

//************************************************************************
float tFinal(float z0, float zf, float vz0, float ubar);


//************************************************************************
float dtFinal(float x0, float xf, float vx0, float y0, float yf, float vy0,float u);


//************************************************************************
// step returns position (z) and velocity (vz) at t=timestep*dt
// from the solution of
// z''(t)+z(t)'=uz 0<=t<t1z
// z''(t)+z(t)'=-uz t1z<=t<tfz
// z(0)=z0  z'(0)=vz0 z(tfz)=zf  z'(tfz)=0
// also z(t)=zf and z'(t)=0 tfz<t
//inputs (nondimensional)
//          z0 - starting point
//          zf - destination
//         vz0 - initial velocity (final velocity is zero)
//          uz - control effort
//         t1z - length of the first interval
//         tfz - total time
//          dt - length of timestep
//    timestep - number of timestep
//************************************************************************
void step(float z0, 
          float zf, 
          float vz0,
          float uz,
          float t1z,
          float tfz,
          float dt,
          int timestep,
          float& z,     // Return variable
          float& vz     // Return variable
          );

#endif //TRAJECTORY_SUPPORT_H
