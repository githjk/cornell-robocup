#include "info/robot/robot.h"
#include "math/dist.h"
#include "figure_eight.h"
#include "../basic_actions.h"
#include "math/cmath.h"
Play FigureEightPlay::playNum = -1;

FigureEightPlay::FigureEightPlay(VisionModule *vm) : BasePlay(vm)
{

}

void FigureEightPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{
  
  MAJOR = 1.0f;
  MINOR = 0.6f;
    
  //if the robots are close enought to their destinations and we havn't
  //already gotten out of formation, set this bool to true and
  //reset the system timer
  outOfFormation = false;
  
  for(int k=0; k<NUM_ROBOTS; k++)
  {
    Translations[k].Radius = 0.8f;
    Translations[k].Frequency = FREQUENCY;
    Translations[k].Phase = 0.55f*k*2.0f*PI/5.0f/FREQUENCY;
    
    Rotations[k].Frequency = 0.0f;
    Rotations[k].Phase = 0.0f;
  }
}

ActionFunction FigureEightPlay::getActionFunction(Position pos)
{
  return stopRobot;
}

void FigureEightPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  float Gain = 10.0f;
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
        rsd->getSystemParams()
					   )
             ) > 0.03f &&
             
        robotFound((RobotIndex)k, (*vision), rsd->getSystemParams())


             )
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
        rsd->getSystemParams()
        );
      float actualRotation = getRotation((RobotIndex)k,
        (*vision),
        rsd->getSystemParams()
        );
      
      command->setControl(MANUAL_CONTROL);
      command->setXVel(DesiredVel.getXPos() + 
        Gain*(DesiredPos.getXPos() - 
        actualLocation.getX()
        )
        );
      
      command->setYVel(DesiredVel.getYPos() + 
        Gain*(DesiredPos.getYPos() - 
        actualLocation.getY()
        )
        );
      
      command->setRotVel(DesiredVel.getRotation() + 
        Gain*normalizeAngle(DesiredPos.getRotation() - 
        actualRotation
        )
        );

      //dribble 50% of the time, that is, when you are on one half of the field
      if(actualLocation.getX() > rsd->getSystemParams().field.HALF_LINE)
      {
        command->setDribble(FAST_DRIBBLE);
        command->setVerticalDribble(FAST_V_DRIBBLE);
      }

      //if any of the robots gets way too far out of wack, then reset the play to go back into
        //formation...
        if(dist(actualLocation, DesiredPos.getPos()) > 0.10f &&
           robotFound((RobotIndex)k, (*vision), rsd->getSystemParams())
          )
          rsd->setNewPlay(FigureEightPlay::playNum);
    }
  }
}


void FigureEightPlay::PosCalculate(translation Translate,
                                   rotation Rotate,
                                   float Time,
                                   Destination* desiredPosition)
{
  desiredPosition->setXPos(MAJOR * 
    Translate.Radius * 
    COS(Translate.Frequency*(Time+Translate.Phase)));
  
  desiredPosition->setYPos(MINOR * 
    Translate.Radius * 
    SIN(2.0f*Translate.Frequency*(Time+Translate.Phase)));
  
  desiredPosition->setRotation(normalizeAngle(Rotate.Frequency * 
    (Time+Rotate.Phase)
    )
    );
}

void FigureEightPlay::VelCalculate(translation Translate,
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
  
  desiredPosition->setYPos(2.0f*VelFactor *
    MINOR * 
    Translate.Radius * 
    Translate.Frequency *
    COS(2.0f*Translate.Frequency*(Time+Translate.Phase)));
  
  desiredPosition->setRotation(Rotate.Frequency);
}

//*********************************************************
//read params
void FigureEightPlay::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/intimidationPlays/FigureEightParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(FREQUENCY);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

