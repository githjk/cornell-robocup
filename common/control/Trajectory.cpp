//#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Trajectory.h"
#include "TrajectorySupport.h"


//************************************************************************
// zeroFinalVelocityPath solves
// x''(t)+x'(t)=ux(t)
// y''(t)+y'(t)=uy(t)
// theta''(t)+theta'(t)=utheta(t)
// with boundary conditions
// x(0)=x0  x'(0)=vx0 x(tf)=xf  x'(tf)=0
// y(0)=y0  y'(0)=vy0 y(tf)=yf  y'(tf)=0
// theta(0)=theta0  theta'(0)=vtheta0 theta(tf)=thetaf  theta'(tf)=0
// and the constraint ux(t)^2+uy(t)^2=1
// and returns the path
//inputs (dimensional)
//               z0 - starting point
//               zf - destination
//              vz0 - initial velocity (final velocity is zero)
//        frameRate - points in the path are generated with stepsize 1/frameRate
//         maxAccel - maximum translational acceleration
//    maxThetaAccel - maximum angular acceleration
//      maxVelocity - maximum translational velocity
// maxThetaVelocity - maximum angular velocity
//    numberOfSteps - number of points generated along the path (-1 returns the whole path)
//************************************************************************
void zeroFinalVelocityPath( ObjectPosVel start,
                            ObjectPosVel final,
                            float frameRate,
                            float maxAccel,
                            float maxThetaAccel,
                            float maxVelocity,
                            float maxThetaVelocity,
                            ObjectPath& thePath,
                            int numberOfSteps
                          )
{
  int  i,j,numIter,maxStepNumber;
  float du,fmid,xmid,u;
  float t1x,t2x,tfx,ux;
  float t1y,t2y,tfy,uy;
  float t1theta,t2theta,tftheta,utheta;
  float dt,angulardt;
  float x,vx,y,vy,theta,vtheta;
    
  if (maxAccel==0 or maxThetaAccel==0 or maxVelocity==0 or maxThetaVelocity==0)
    cout << "Trajectory code error: invalid scale(s)" << endl;
  
  float timeScale             = maxVelocity/maxAccel;
  float angularTimeScale      = maxThetaVelocity/maxThetaAccel;
  float lengthScale           = maxVelocity*timeScale;
  float velocityScale         = maxVelocity;
  float angularScale          = maxThetaVelocity*angularTimeScale;
  float angularVelocityScale  = maxThetaVelocity;

//  nondimensionalize positions and velocities
  float oneoverlengthscale = 1/lengthScale;
  float x0      = start.xPos*oneoverlengthscale;
  float y0      = start.yPos*oneoverlengthscale;
  float xf      = final.xPos*oneoverlengthscale;
  float yf      = final.yPos*oneoverlengthscale;
  float vx0     = start.xVel/velocityScale;
  float vy0     = start.yVel/velocityScale;
  float vtheta0 = start.rotVel/angularVelocityScale;

  float theta0  = start.rotation;
  float thetaf  = final.rotation;

//finds the shortest "rotational path" between theta0 and thetaf
  float dtheta=thetaf-theta0;
  if (dtheta < -PI)
    dtheta = dtheta + 2*PI;
  if (dtheta > PI)
    dtheta = dtheta - 2*PI;
  thetaf=theta0 + dtheta;

  theta0 = theta0 / angularScale;
  thetaf = thetaf / angularScale;

//finds the zero of tf(x0,xf,vx0,u)-tf(y0,yf,vy0,sqrt(1-sqr(u))) in (0,1) by bisection
  numIter = 10;//number of iterations performed. error in u is 2^(-numIter)
  u   =  0.999f;
  du  = -0.998f;
  
  for (j=0; j<numIter; j++) 
  {
    fmid = dtFinal(x0,xf,vx0,y0,yf,vy0,xmid=u+(du *= 0.5));
    if (fmid <= 0.0) u=xmid;
  }

//now we calculate the path properties
  bangBangPathProperties(x0,xf,vx0,u,t1x,t2x,tfx,ux);
  bangBangPathProperties(y0,yf,vy0,(float)sqrt(1-sqr(u)),t1y,t2y,tfy,uy);
  
  //theta is independent of x and y so we use bang-bang with u=1
  bangBangPathProperties(theta0,thetaf,vtheta0,1,t1theta,t2theta,tftheta,utheta);

// building the path
  thePath.clearPath();

  float nondimFrameRate = frameRate*timeScale;
  float nondimAngularFrameRate = frameRate*angularTimeScale;
  int tfint = (int)ceil(__max(tfx,tfy)*nondimFrameRate);
  int tfthetaint = (int)ceil(tftheta*nondimAngularFrameRate);

  dt = 1/nondimFrameRate;
  angulardt = 1/nondimAngularFrameRate;

  if (numberOfSteps == -1){
    maxStepNumber=__max(tfint,tfthetaint);
  }
  else {
    maxStepNumber=__min(numberOfSteps,__max(tfint,tfthetaint));
  }
  maxStepNumber=__min(maxStepNumber,MAX_TRAJECTORY_STEPS);

  for ( i = 0; i <= maxStepNumber; i++ )
  {
    // Calculate step along x-axis
    step(x0,xf,vx0,ux,t1x,tfx,dt,i, x, vx);
    x  = lengthScale*x;
    vx = velocityScale*vx;
    
    // Calculate step along y-axis
    step(y0,yf,vy0,uy,t1y,tfy,dt,i,y,vy);
    y  = lengthScale*y;
    vy = velocityScale*vy;
    
    // Calculate step along theta-axis
    step(theta0,thetaf,vtheta0,utheta,t1theta,tftheta,angulardt,i,theta,vtheta);
    theta  = angularScale*theta;
    vtheta = angularVelocityScale*vtheta;

    // Add step to the path
    thePath.addStep(x,y,theta,vx,vy,vtheta);
  }
}

//************************************************************************
void zeroFinalVelocityTheta(ObjectPosVel start,
                            ObjectPosVel final,
                            float frameRate,
                            float maxThetaAccel,
                            float maxThetaVelocity,
                            float & nextAngularVelocity
                          )
{
  float t1theta,t2theta,tftheta,utheta;
    
  float angularTimeScale      = maxThetaVelocity/maxThetaAccel;
  float angularScale          = sqr(maxThetaVelocity)/maxThetaAccel;
  float angularVelocityScale  = maxThetaVelocity;

  float theta0  = start.rotation;
  float thetaf  = final.rotation;
  
  float dtheta=thetaf-theta0;

  if (dtheta < -PI)
    dtheta = dtheta + 2*PI;
  if (dtheta > PI)
    dtheta = dtheta - 2*PI;

  thetaf=theta0 + dtheta;

  theta0 = theta0 / angularScale;
  thetaf = thetaf / angularScale;

  
  
  float vtheta0 = start.rotVel/angularVelocityScale;
  
  //use bang-bang with u=1 on the theta-axis
  bangBangPathProperties(theta0,thetaf,vtheta0,1,t1theta,t2theta,tftheta,utheta);

  float nondimAngularFrameRate = frameRate*angularTimeScale;
  float angulardt = 1/nondimAngularFrameRate;
  float temp=(float)exp(-angulardt);
  nextAngularVelocity=angularVelocityScale*(temp*vtheta0+utheta*(1-temp));

}

//************************************************************************
void fastPath( ObjectPosVel start,
               ObjectPosVel final,
               float frameRate,
               float maxAccel,
               float maxVelocity,
               ObjectPath& thePath,
               int numberOfSteps)
{
  int  i,maxStepNumber;
  float x,vx,y,vy;
    
  if (maxAccel==0 or maxVelocity==0)
    cout << "Trajectory code error: invalid scale(s)" << endl;
  
  float timeScale             = maxVelocity/maxAccel;
  float lengthScale           = sqr(maxVelocity)/maxAccel;
  float velocityScale         = maxVelocity;

  //  nondimensionalize positions and velocities
  float x0      = start.xPos/lengthScale;
  float y0      = start.yPos/lengthScale;
  float theta0  = start.rotation;
  float thetaf  = final.rotation;
  float vx0     = start.xVel/velocityScale;
  float vy0     = start.yVel/velocityScale;
  float xf      = final.xPos/lengthScale;
  float yf      = final.yPos/lengthScale;

  float dx=xf-x0;
  float dy=yf-y0;
  float t=0;
  float dt=0.01f;
  float func=0;
  float temp=0;
  while(func>=0)
  {
    t+=dt;
    temp=1-(float)exp(-t);
    func=sqr( dx-(vx0*temp) ) + sqr( dy- (vy0*temp) ) - sqr(temp-t);
  }
  float phi = (float)atan2((dy-vy0*temp) / (t-temp), (dx-vx0*temp) / (t-temp));
  
// building the path
  thePath.clearPath();

  float nondimFrameRate = frameRate*timeScale;
  int tfint = (int)ceil(t*nondimFrameRate);

  dt = 1/nondimFrameRate;

  if (numberOfSteps == -1){
    maxStepNumber=tfint;
  }
  else {
    maxStepNumber=__min(numberOfSteps,tfint);
  }
  maxStepNumber=__min(maxStepNumber,MAX_TRAJECTORY_STEPS);

  for ( i = 0; i <= maxStepNumber; i++ )
  {

    // Calculate step along x-axis
    step(x0,xf,vx0,(float)cos(phi),t,t,dt,i, x, vx);
    x  = lengthScale*x;
    vx = velocityScale*vx;
    
    // Calculate step along y-axis
    step(y0,yf,vy0,(float)sin(phi),t,t,dt,i,y,vy);
    y  = lengthScale*y;
    vy = velocityScale*vy;
    
    // Add step to the path
    thePath.addStep(x,y,theta0,vx,vy,0.0f);
  }
}
//************************************************************************
void expectedPathTime(  ObjectPosVel start,
                        ObjectPosVel final,
                        float frameRate,
                        float maxAccel,
                        float maxVelocity,
                        float& pathTime
                      )
{
  int  j,numIter;
  float du,fmid,xmid,u;
  float t1x,t2x,tfx,ux;
  float t1y,t2y,tfy,uy;
    
  if (maxAccel==0 or maxVelocity==0)
    cout << "Trajectory code error: invalid scale(s)" << endl;
  
  float timeScale             = maxVelocity/maxAccel;
  float lengthScale           = sqr(maxVelocity)/maxAccel;
  float velocityScale         = maxVelocity;

  //  nondimensionalize positions and velocities
  float x0      = start.xPos/lengthScale;
  float y0      = start.yPos/lengthScale;
  float vx0     = start.xVel/velocityScale;
  float vy0     = start.yVel/velocityScale;
  float xf      = final.xPos/lengthScale;
  float yf      = final.yPos/lengthScale;

//finds the zero of tf(x0,xf,vx0,u)-tf(y0,yf,vy0,sqrt(1-sqr(u))) in (0,1) by bisection
  numIter = 10;//number of iterations performed
  u   =  0.999f;
  du  = -0.998f;
  
  for (j=1; j<=numIter; j++) 
  {
    fmid = dtFinal(x0,xf,vx0,y0,yf,vy0,xmid=u+(du *= 0.5));
    if (fmid <= 0.0) u=xmid;
  }

//now we calculate the path properties
  bangBangPathProperties(x0,xf,vx0,u,t1x,t2x,tfx,ux);
//  pathTime=timeScale*tfx;

  bangBangPathProperties(y0,yf,vy0,(float)sqrt(1 - u*u),t1y,t2y,tfy,uy);
  pathTime=timeScale*__max(tfx,tfy);
}


//=================================================
// PROPORTIONAL GAIN FUNCTIONS

void proportionalGainPath(ObjectPosVel start,
                          ObjectPosVel final,
                          float frameRate,
                          float translationalGain,
                          float angularGain,
                          float maxVelocity,
                          float maxThetaVelocity,
                          ObjectPath& thePath
                         )
{
  
  float x0      = start.xPos;
  float y0      = start.yPos;
  float theta0  = start.rotation;
  float xf      = final.xPos;
  float yf      = final.yPos;
  float thetaf  = final.rotation;
  float vx0     = start.xVel;
  float vy0     = start.yVel;
  float vtheta0 = start.rotVel;

  float dtheta=thetaf-theta0;//convert it!

  if (dtheta < -PI)
    dtheta = dtheta + 2*PI;
  if (dtheta > PI)
    dtheta = dtheta - 2*PI;

  thetaf = theta0 + dtheta;

  float dt = 1/frameRate;

  thePath.clearPath();
  thePath.addStep(x0,y0,theta0,vx0,vy0,vtheta0);//add the initial position

  float vx = translationalGain*(xf-x0);
  float vy = translationalGain*(yf-y0);
  float vtheta = angularGain*dtheta;

  float speed = (float)sqrt(vx*vx + vy*vy);

  // Saturate translational velocity
  if (speed > maxVelocity)
  {
    float scale=maxVelocity/speed;
    vx = scale*vx;
    vy = scale*vy;
  }

  // Saturate angular velocity
  if (vtheta > maxThetaVelocity)
  {
    vtheta = maxThetaVelocity;
  }
  else if (vtheta < -maxThetaVelocity)
  {
    vtheta = -maxThetaVelocity;
  }
  thePath.addStep(x0+dt*vx,y0+dt*vy,theta0+dt*vtheta,vx,vy,vtheta);
}
//=================================================
void proportionalGainTheta(ObjectPosVel start,
                           ObjectPosVel final,
                           float frameRate,
                           float angularGain,
                           float maxThetaVelocity,
                           float & nextAngularVelocity
                          )
{

  float theta0  = start.rotation;
  float thetaf  = final.rotation;

  float dtheta=thetaf-theta0;

  if (dtheta < -PI)
    dtheta = dtheta + 2*PI;
  if (dtheta > PI)
    dtheta = dtheta - 2*PI;

  nextAngularVelocity = angularGain*dtheta;

  // Saturate angular velocity
  if (nextAngularVelocity > maxThetaVelocity)
  {
    nextAngularVelocity = maxThetaVelocity;
  }
  else if (nextAngularVelocity < -maxThetaVelocity)
  {
    nextAngularVelocity = -maxThetaVelocity;
  }
  

}
