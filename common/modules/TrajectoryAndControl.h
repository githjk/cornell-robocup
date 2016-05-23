/*******************************************************************
 *
 * TrajectoryAndControl.h:  Defines the TrajectoryData
 *  and controlData classes, and the abstract base class for
 *  TrajectoryModule.
 *
 * 04-29-01  Matt Harren
 ******************************************************************/

#include "ai.h"
#include "Path.h"

#ifndef TRAJECTORY_AND_CONTROL_H
#define TRAJECTORY_AND_CONTROL_H

class ControlModule;

/*********************
* Trajectory Data    *
*********************/
class TrajectoryData
{
	
public:

 	// constructor 
	TrajectoryData() {}

  	// writes frame to file
	Path* getPath() {return &path;}
	float getETA() {return ETA;}
	void setETA(float eta) { ETA=eta; }
	
	//get the first step of the path
  //Omni only
	Pair getVelocity(void) const { return path.getStepVel(0); }
	float getVelocityX(void) const { return path.getStepVel(0).getX(); }
	float getVelocityY(void) const { return path.getStepVel(0).getY(); }
	float getVelocityTheta(void) const { return velocityTheta; }

protected:
	Path	path;
	float	ETA;

  //Omni only
	float	velocityTheta;
};

/******************************
* Trajectory Module Interface *
******************************/
class TrajectoryModule
{
  
public:
	//---------------------------------------------------------------------
  //use bang-bang on longer axis
  virtual void findPathZeroFinalVel(const MovingObject& here,
	                                const Destination& there,
							        TrajectoryData* output) const = 0;
  //---------------------------------------------------------------------
  //use bang-bang on both axis
  virtual void findPathAlphaZeroFinalVel(const MovingObject& here,
	                                     const Destination& there,
									     TrajectoryData* output) const = 0;
  //---------------------------------------------------------------------
  //don't care about final velocity
  virtual void findPathAnyFinalVel(const MovingObject& here,
	                               const Destination& there,
								   TrajectoryData* output) const = 0;
  //---------------------------------------------------------------------
  //This is a bad way to get the time, but for 2-wheeled it's the only way
  virtual float findPathTime(const MovingObject& here,
	                         const Destination& there) const
  {
    TrajectoryData td;
    findPathZeroFinalVel(here, there, &td);
    return td.getETA();
  }
  //---------------------------------------------------------------------
  virtual void loadParameters() 
  {
  }
  //---------------------------------------------------------------------
  //for use with MANUAL_TRANSLATION
  virtual float getAngularVelocityOnly(const MovingObject& here,
	                                   const Destination& there) const 
  {
    cout << "Error: setAngularVelocityOnly() only works with omni robots." << endl;
	return 0;
  }
  //---------------------------------------------------------------------
protected:

	SystemParameters*   params;


 //Make the defualt constructor protected so that only
  // subclasses can create a TrajectoryModule
  TrajectoryModule(SystemParameters* p) {
    this->params = p;
  }
};

#endif // TRAJECTORY_AND_CONTROL_H