//#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "TrajectorySupport.h"

//************************************************************************
float distance(float x1, float y1, float x2, float y2)
{
  return (float)sqrt ( sqr(x2-x1) + sqr(y2-y1) );
}

//************************************************************************
void bangBangPathProperties(float z0,
                            float zf,
                            float vz0,
                            float ubar,
                            float &t1,
                            float &t2,
                            float &tf,
                            float &u)
{
    float c = z0-zf+vz0;

    if (ubar<0.01) 
      u=0;
    else 
      u = ubar*sgn(vz0/ubar-sgn(c)*((float)exp(fabs(c/ubar))-1));

    if (u==0)
      t1=t2=tf=u=0;
    else
    {
      t2 = (float)log(1+sqrt(1+exp(c/u)*(vz0/u-1)));
      t1 = t2-c/u;
      tf = t1+t2;
    }
}
//************************************************************************
float tFinal(float z0, float zf, float vz0, float ubar)
{
  float t1,t2,tf,u;
  bangBangPathProperties(z0,zf,vz0,ubar,t1,t2,tf,u);
  return tf;
}
//************************************************************************
float dtFinal(float x0, float xf, float vx0, float y0, float yf, float vy0,float u)
{
    return tFinal(x0,xf,vx0,u)-tFinal(y0,yf,vy0,(float)sqrt(1-sqr(u)));
}
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
          )
{
    float t=(float)timestep*dt;
    float expmt=(float)exp(-t);
    
    if (t<=t1z)
    {
        z=uz*(t-1)+expmt*(uz-vz0)+z0+vz0;
        vz=uz+expmt*(vz0-uz);
    }
    else if (t<=tfz)
    {
        float exptfz=(float)exp(tfz);
        z=uz*(1-t+tfz-expmt*exptfz)+zf;
        vz=uz*(expmt*exptfz-1);
    }
    else
    {
        z=zf;
        vz=0;
    }
}