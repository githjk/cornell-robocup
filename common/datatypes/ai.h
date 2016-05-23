/*
 * General AI-required classes and datatypes
 */

#ifndef DATATYPES_AI_TYPES_H
#define DATATYPES_AI_TYPES_H

// should do something like
//#ifdef _WIN
//#include <windows.h>
//#endif
// instead of deleting it, to make it *nix/windows portable

#include "FieldObjects.h"
#include "common/datatypes/SystemParameters.h"
#include "base_types.h"
#include "ActionCodes.h"
#include "ControlData.h"

#define MAX_LOG 8000
///////////////////////////////////////////////////////////////////
//  AI STRATEGY DATA TYPES
///////////////////////////////////////////////////////////////////
  
typedef enum
{
  DEFAULT_SPEED,
  GOALIE_SPEED,
  SPIN_TO_KICK_SPEED,
  CAREFUL_SPEED,
  BALL_POSSESSION_SPEED,
  BALL_APPROACH_SPEED,
  FORMATION_SPEED,
  SLOW_ROTATE,
  OPPONENT_SPEED,
} SpeedControl;

typedef enum
{
	NO_METHOD = -1,
  //--------------------------------------------------
	OMNI_NO_AVOID,			            // no avoidance
  //--------------------------------------------------
  OMNI_NORMAL,				            // avoids all robots, zero final veloctiy (by definition slower)
	OMNI_NORMAL_ENTERBOX,		        // like OMNI_NORMAL, but allows entry to the opponent goalie box.
  //--------------------------------------------------
  OMNI_FAST,                       // avoids all robots, any final velocity (faster)
	OMNI_FAST_ENTERBOX,              // avoids all robots, any final velocity (faster), enters opponent goalie box
  //--------------------------------------------------
  OMNI_GOALIE,				            // avoids nothing, zero final velocity (slower)
  OMNI_GOALIE_FAST,				        // avoids nothing, any final velocity (faster)
  //--------------------------------------------------
  OMNI_STRIVE,			              // avoids robots and ball
	MANUAL_CONTROL,				          // Bypass TrajGen and ObstacleAvoidance;  just use the final velocity numbers
	MANUAL_TRANSLATION,			        // Like above, but uses the normal proportional gain routine to calculate a theta velocity
} ControlMethod;





typedef enum
{
  BARSTATUS_DOWN,
  BARSTATUS_UP,
  BARSTATUS_ENABLED
} BarStatus;

const BarStatus BARSTATUS_DEFAULT = BARSTATUS_DOWN;

//*****************************************************************

/*****************
 * Keeping Score *
 *****************/
class Score
{
 public:
  Score() { us = them = 0; }

  int getUs (void) { return us; }
  void incUs (void) { ++us; }
  int getThem (void) { return them; }
  void incThem (void) { ++them; }
  
 private:
  int us, them;
};


/**************
 * VisionData *
 **************/

class MovingObject
{
 public:

  MovingObject (const Pair &p, const Pair &v, float r, float rv) :
    pos(p), vel(v), rotation(r), rotVel(rv)
  { }

  MovingObject (const MovingObject& old) :
    pos(old.pos), vel(old.vel), rotation(old.rotation), rotVel(old.rotVel)
  { }

  MovingObject () :
    pos(), vel(), rotation(0), rotVel(0)
  { }
  
  Pair getPos (void) const { return pos; }
  void setPos (const Pair &p) { pos.set(p); }
  void setPos (float x, float y) { pos.setX(x); pos.setY(y); }
  void setXPos (float x) { pos.setX(x); }
  void setYPos (float y) { pos.setY(y); }

  Pair getVel (void) const { return vel; }
  void setVel (const Pair &v) { vel = v; }
  void setVel (float x, float y) { vel.setX(x); vel.setY(y); }
  void setXVel (float x) { vel.setX(x); }
  void setYVel (float y) { vel.setY(y); }
  
  float getRotation (void) const { return rotation; }
  void setRotation (float r) { rotation = r; }

  float getRotVel (void) const { return rotVel; }
  void setRotVel (float v) { rotVel = v; }

  float getXPos (void) const { return pos.getX(); }
  float getYPos (void) const { return pos.getY(); }
  float getXVel (void) const { return vel.getX(); }
  float getYVel (void) const { return vel.getY(); }

  //returns the vector in the direction the bot is facing:
  Line getFacingVector(void) const {
    return Line(pos, Pair(pos.getX() + COS(rotation), pos.getY() + SIN(rotation))); }

  //returns the vector in the direction the bot is moving:
  Line getMovementVector(void) const {
    return Line(pos, Pair(pos.getX() + vel.getX(), pos.getY() + vel.getY())); }

 
 protected:
  Pair pos;   // the position, in meters (x, y)
  Pair vel;   // the velocity, in m/s (vx, vy)
  
  float rotation; // Rotation of object, in radians
  float rotVel;   // Rotational velocity, in radians
};


/**************************
 * Destination:           *
 * Allows easy setting of *
 * final state of robot   *
 **************************/

class Destination : public MovingObject
{
  public:
   /* constructor */
   // by default, initialize to (0, 0), 
   //(0, 0) and all parameter bits to false
   Destination()
	   : MovingObject()
   {
     initialize();
   }

   //Initializes all desination data
   void initialize (void)
   {    
     //Default destinatinon position
     pos.setX(0.0f);
     pos.setY(0.0f);

     //Default destination velocity
     vel.setX(0.0f);
     vel.setY(0.0f);

     //Default destination rotation
     rotation = 0.0f;
     rotVel = 0.0f;

     //Don't do anything else
     clearCommandParameter();
	   speed = DEFAULT_SPEED;
	   control = OMNI_NORMAL;
   }

    void clearCommandParameter (void)
    {
      dribble = NO_DRIBBLE;
      verticalDribble = NO_V_DRIBBLE;
      kick = NO_KICK;
      beep = NO_BEEP;
      explode = false;
      barStatus = BARSTATUS_DEFAULT;
      switchCommand = NO_ACTION;
    }

    //------------------
    // get/set functions
    //------------------

    //------------------
    //Getters
    //------------------
    //Current status of dribble/kick/explode
    DribbleSpeed getDribble (void) const { return dribble; }
    VerticalDribbleSpeed getVerticalDribble(void) const {return verticalDribble;}
    KickSpeed    getKick (void) const    { return kick; }
    BeepCommand  getBeep (void) const    { return beep; }
    bool         getExplode (void) const { return explode; }
    BarStatus    getBarStatus(void) const{ return barStatus; }
    SpeedControl getSpeed(void) const    { return speed; }
    SwitchCommand getSwitchCommand (void) const { return switchCommand; }
    ControlMethod getControl(void) const  { return control; }

    //------------------
    //Setters
    //------------------
    //Set status of dribble/kick/explode
    void setDribble  (DribbleSpeed sDribble) 
    { 
      dribble = sDribble; 
    }
    void setVerticalDribble(VerticalDribbleSpeed vDribble) 
    {
      verticalDribble = vDribble;
    }
    void setKick     (KickSpeed sKick)   { kick = sKick; }
    void setBeep     (BeepCommand b)     { beep = b; }
    void setExplode  (bool sExplode = true)  { explode = sExplode; }
    void setBarStatus(BarStatus sBarStatus) { barStatus = sBarStatus; }
    void setSpeed    (SpeedControl s) { speed = s; }
    void setSwitchCommand (SwitchCommand sw) { switchCommand = sw; }
    void setControl  (ControlMethod c) { control = c; }

  private:
    DribbleSpeed dribble;
    VerticalDribbleSpeed verticalDribble;
    KickSpeed kick;
    BeepCommand beep;
    bool explode; //NEVER DELETE THIS VARIABLE! CAUSES SYSTEM TO CRASH!
    BarStatus barStatus;
    SpeedControl speed;
    SwitchCommand switchCommand;
  	ControlMethod control;
};


#endif // DATATYPES_AI_TYPES_H







