#include "info/robot/robot.h"
#include "math/dist.h"
#include "ellipses.h"
#include "../basic_actions.h"
#include "math/cmath.h"
Play EllipsesPlay::playNum = -1;

EllipsesPlay::EllipsesPlay(VisionModule *vm) : BasePlay(vm)
{

}

//========================================================================= 
void EllipsesPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

  MAJOR = 1.0f;
  MINOR = 0.6f;

  /*
  //======
  //RADAR:
  float Freq = 1.0f;
  float Spacing = 0.40f;
  float Offset = 0.80f;

  //if the robots are close enought to their destinations and we havn't
  //already gotten out of formation, set this bool to true and
  //reset the system timer
  outOfFormation = false;

  for(int k=0; k<NUM_ROBOTS; k++)
  {
    Translations[k].Radius = k*Spacing - Offset;
    Translations[k].Frequency = Freq;
    Translations[k].Phase = 0.0f;

    Rotations[k].Frequency = Freq;
	  Rotations[k].Phase = 0.0f;
  }
  //END RADAR
  //=========
  */

  //======
  //CIRCUS:
  //if the robots are close enought to their destinations and we havn't
  //already gotten out of formation, set this bool to true and
  //reset the system timer
  outOfFormation = false;

  //--
  Translations[0].Radius = 0.1f/MINOR;
  Translations[0].Frequency = .8f*2.0f;
  Translations[0].Phase = 0.0f;
  Rotations[0].Frequency = .8f*1.0f;
  Rotations[0].Phase = 0.0f;
  //--
  Translations[1].Radius = 0.35f/MINOR;
  Translations[1].Frequency = .8f*-1.0f;
  Translations[1].Phase = PI/2.0f;
  Rotations[1].Frequency = .8f*-1.0f;
  Rotations[1].Phase = PI;
  //--
  Translations[2].Radius = 0.35f/MINOR;
  Translations[2].Frequency = .8f*-1.0f;
  Translations[2].Phase = -PI/2.0f;
  Rotations[2].Frequency = .8f*-1.0f;
  Rotations[2].Phase = PI;
  //--
  Translations[3].Radius = 0.60f/MINOR;
  Translations[3].Frequency = .8f*1.0f;
  Translations[3].Phase = -PI/2.0f;
  Rotations[3].Frequency = .8f*1.0f;
  Rotations[3].Phase = 0.0f;
  //--
  Translations[4].Radius = 0.60f/MINOR;
  Translations[4].Frequency = .8f*1.0f;
  Translations[4].Phase = PI/2.0f;
  Rotations[4].Frequency = .8f*1.0f;
  Rotations[4].Phase = 0.0f;
  //--
  //END CIRCUS
  //=========
}
//========================================================================= 
ActionFunction EllipsesPlay::getActionFunction(Position pos)
{
  return stopRobot;
}
//========================================================================= 
void EllipsesPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  float Gain = 5.0f;
  float TimeRamp = 3.0f;
  Destination InitializePos[NUM_ROBOTS];

  for(int k=0; k<NUM_ROBOTS; k++)
  {
     PosCalculate(Translations[k],
                  Rotations[k],
                  0.0f,
	  			        &(InitializePos[k]));
  }
	
  bool breakOutOfFormation = true;

  for(k = 0; k < NUM_ROBOTS; k++)
  {
    if(dist(InitializePos[k].getPos(), 
	     	    getLocation(
			      (RobotIndex)k, 
			      (*vision), 
			  	  rsd->getSystemParams())) > 0.03f &&
       robotFound((RobotIndex)k, (*vision), rsd->getSystemParams()))
      breakOutOfFormation = false;
  }

  //if we havn't already broken out of formation and we can now, reset
  //the system timer
  if(breakOutOfFormation && !outOfFormation)
  { 
	  outOfFormation = true;
    startTimer();
  }
  
  //if we are still going into formation, use normal trajectory
  if(!outOfFormation)
  {
	  for(int i=0; i<NUM_ROBOTS; i++)
	  {
	    rsd->setMessage((RobotIndex)i,"Getting Ready!");
  		rsd->getDestination((RobotIndex)i)->setPos(InitializePos[i].getPos());
	  	rsd->getDestination((RobotIndex)i)->setRotation(InitializePos[i].getRotation()); 		  
		  rsd->getDestination((RobotIndex)i)->setControl(OMNI_NORMAL_ENTERBOX);
		  rsd->getDestination((RobotIndex)i)->setSpeed(DEFAULT_SPEED);
    }
  }
  //else, set exact robot velocities
  else
  {
	  float T = (float)readTimer(); 
    float t;
	  float VelFactor;

	  if(T < TimeRamp)
    {
      t = (T*T)/(2*TimeRamp);
	    VelFactor = T/TimeRamp;
    }
	  else
    {
	    t = T - TimeRamp/2;
      VelFactor = 1.0f;
    }
  
	  for(k=0; k<NUM_ROBOTS; k++)
    {
      rsd->setMessage((RobotIndex)k,"Going Around!");

      Destination DesiredPos;
      Destination DesiredVel;
     
      PosCalculate(Translations[k],
                   Rotations[k],
                   t,
                   &DesiredPos);
      VelCalculate(Translations[k],
                   Rotations[k],
                   t,
                   VelFactor,
                   &DesiredVel);
     
      Destination* command = rsd->getDestination((RobotIndex)k);
     
      Pair actualLocation = getLocation((RobotIndex)k,
                                        (*vision),
                                        rsd->getSystemParams());
      float actualRotation = getRotation((RobotIndex)k,
                                         (*vision),
                                         rsd->getSystemParams());
     
      command->setControl(MANUAL_CONTROL);
      command->setXVel(DesiredVel.getXPos() + 
                       Gain*(DesiredPos.getXPos() - 
                             actualLocation.getX()));
     
      command->setYVel(DesiredVel.getYPos() + 
                       Gain*(DesiredPos.getYPos() - 
                             actualLocation.getY()));
     
      command->setRotVel(DesiredVel.getRotation() + 
                         Gain*normalizeAngle(DesiredPos.getRotation() - 
                         actualRotation));
     
      //if any of the robots gets way too far out of wack, then reset the play to go back into
      //formation...
      if(dist(actualLocation, DesiredPos.getPos()) > 0.10f &&
         robotFound((RobotIndex)k, (*vision), rsd->getSystemParams()))
         rsd->setNewPlay(EllipsesPlay::playNum);
     
    }
  }
}
//========================================================================= 
void EllipsesPlay::PosCalculate(translation Translate,
   	                            rotation Rotate,
                                float Time,
                                Destination* desiredPosition)
{
  desiredPosition->setXPos(MAJOR * 
    Translate.Radius * 
    COS(Translate.Frequency*(Time+Translate.Phase)));
  
  desiredPosition->setYPos(MINOR * 
    Translate.Radius * 
    SIN(Translate.Frequency*(Time+Translate.Phase)));
  
  desiredPosition->setRotation(normalizeAngle(Rotate.Frequency * 
    (Time+Rotate.Phase)));
}
//========================================================================= 
void EllipsesPlay::VelCalculate(translation Translate,
                                rotation Rotate,
                                float Time,
                                float VelFactor,
                                Destination* desiredPosition)
{
  desiredPosition->setXPos(VelFactor * 
    (-MAJOR) * 
    Translate.Radius * 
    Translate.Frequency *
    SIN(Translate.Frequency*(Time+Translate.Phase)));
  
  desiredPosition->setYPos(VelFactor *
    MINOR * 
    Translate.Radius * 
    Translate.Frequency *
    COS(Translate.Frequency*(Time+Translate.Phase)));
  
  desiredPosition->setRotation(Rotate.Frequency);
}
//========================================================================= 
