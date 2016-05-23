#include "JukeAroundOpponentSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/intersect/intersect.h"
#include "common/util/pair/pair.h"
#include "GUI/GUI.h"
#include "Skillz/SpinSkill.h"
#include "Skillz/ManeuverSkill.h"

//so we can create random numbers
#include <stdlib.h>
#include <time.h>

int JukeAroundOpponentSkill::skillNum = -1;

//========================================================================= 
///Constructor.  Calls the base class Skill constructor
JukeAroundOpponentSkill::JukeAroundOpponentSkill(SystemParameters *sp,
                                                 VisionModule *vm, 
                        						             RoboCupStrategyModule *strat,
                                                 RobotIndex id, 
                                                 SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  initialized = false;
  frustrated = false;
  
  currentTechnique = NO_TECHNIQUE;
  loadValues();
  //seed random number generator
  srand( (unsigned)time(NULL));
}

//========================================================================= 
///Call this function to check the prerequisites of the skill.  This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool JukeAroundOpponentSkill::isValid() 
{ 
  if(!initialized)
  {
      strategy->getCurrentFrame()->setMessage(robotID, "JukeAroundOpponent not initialized");  
      return false;
  }
  else
  {
     //make opponent is still between us and our goal dest

    //where we are right now
    Pair us(getLocation(sp->general.TEAM, robotID, (*currentVisionData)));

    if(isBetween(us, dest, 
                 sp->general.OTHER_TEAM,
                 opponentID,
                 (*currentVisionData),
                 (*sp),
                 6*sp->general.PLAYER_RADIUS,
                 4*sp->general.PLAYER_RADIUS))
    {
      return true;
    }
    else
    {
      strategy->getCurrentFrame()->setMessage(robotID, "Opponent not in the way");  
      return false;
    }
  }
}
//========================================================================= 
///Perform any initializations for the skill, such as reseting the timer.
void JukeAroundOpponentSkill::initialize(Pair newDest)
{
  //set the ultimate destination
  dest.set(newDest);

  //pick a technique
  pickNewTechnique();

  //determine opponent we are juking around
  //iterate through each opponent, find opponent that
  //a.) is in between us and our destination AND
  //b.) closest too us

  //where we are right now
  Pair us(getLocation(sp->general.TEAM, robotID, (*currentVisionData)));

  float closestDist = -32000.0f;
  opponentID = (RobotIndex)-1;

  for(int i=0; i<NUM_ROBOTS; i++)
  {
    if(isBetween(us, dest, 
                 sp->general.OTHER_TEAM,
                 (RobotIndex)i,
                 (*currentVisionData),
                 (*sp),
                 6*sp->general.PLAYER_RADIUS,
                 4*sp->general.PLAYER_RADIUS))
    {
      //if distance is smaller then write it down =-)
      float newDist = us.distanceTo(getLocation(sp->general.OTHER_TEAM, 
                                                (RobotIndex)i, 
                                                (*currentVisionData)));

      if(closestDist == -32000.0f ||
         newDist < closestDist)
      {
          opponentID = (RobotIndex)i;
          closestDist = newDist;
      }         
    }            
  }
   
  //if we found an opponent init worked
  if(opponentID != -1)
  {
    initialized = true;
  }
  else
  {
    GUI_Record.debuggingInfo.addDebugMessage("Unable to initialize JukeAroundOpponent: unable to find a blocking robot");
  }
}
//========================================================================= 
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void JukeAroundOpponentSkill::execute()
{    
  command->setSpeed(SLOW_ROTATE);
  switch(currentTechnique)
  {
  case PUSH_THROUGH:
    pushThrough(); 
    break;
  case MOVE_AROUND:
    moveAround(); 
    break;
  case FAKE_AROUND:
    fakeAround(); 
    break;
  }    
}
//========================================================================= 
///If the timer goes past 3 secs, reverse rotation
bool JukeAroundOpponentSkill::evaluate() 
{
  //if not intialized just pop out
  if(!initialized)
    return true;

  //if too much time has passed by and we have not finished set
  //frustrated boolean
  if(
       timer->currentTime() > MAX_TIME_PER_TECHNIQUE &&
       !isFinished(-1)
    )
  {
    frustrated = true;
  }

  //what does this return value mean??
  return true;
}
//========================================================================= 
///For tweaking the skill.  You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void JukeAroundOpponentSkill::update() 
{
  //if not intialized then pop out
  if(!initialized)
    return;

  //if skill is frustrated, pick a new technique
  if(frustrated)
    pickNewTechnique(currentTechnique);
}
//========================================================================= 
///Not yet implemented.  never finished
bool JukeAroundOpponentSkill::isFinished(float tolerance)
{

  //if opponent is nolonger between us and our target, then we're done! =-)
  if(finished)
    return true;
  else
  {
    Pair us(getLocation(sp->general.TEAM, robotID, (*currentVisionData)));

    if(isBetween(us, dest, 
                 sp->general.OTHER_TEAM,
                 opponentID,
                 (*currentVisionData),
                 (*sp),
                 6*sp->general.PLAYER_RADIUS,
                 2*sp->general.PLAYER_RADIUS))
    {
      return false;
    }
    else
    {
      finished = true;
      return true;
    }
  }
}
//========================================================================= 
///Reads in the values of the parameters from the skill's parameter text file.
void JukeAroundOpponentSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/JukeAroundOpponentSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //Bias's
  float PUSH_THROUGH_BIAS;
  READ_FLOAT(PUSH_THROUGH_BIAS);    
  Biases[PUSH_THROUGH] = PUSH_THROUGH_BIAS;
  //---  
  float MOVE_AROUND_BIAS;
  READ_FLOAT(MOVE_AROUND_BIAS);    
  Biases[MOVE_AROUND] = MOVE_AROUND_BIAS;
  //---  
  float FAKE_AROUND_BIAS;
  READ_FLOAT(FAKE_AROUND_BIAS);    
  Biases[FAKE_AROUND] = FAKE_AROUND_BIAS;
  //---  
  READ_FLOAT(DELTA_BIAS);
  READ_FLOAT(MAX_TIME_PER_TECHNIQUE);
  //---  
  //Push_Through parameters
  READ_FLOAT(PUSH_THROUGH_AVOID_DISTANCE);
  READ_FLOAT(PUSH_THROUGH_DISTANCE);
  READ_FLOAT(PUSH_THROUGH_ANGLE_THRESHHOLD);
  //---  
  //Move_Around parameters
  READ_FLOAT(MOVE_AROUND_AVOID_DISTANCE);
  READ_FLOAT(MOVE_AROUND_ANGLE_THRESHHOLD);
  READ_FLOAT(MOVE_AROUND_DELTA);
  READ_FLOAT(MOVE_AROUND_ROTATION_RATE);

  //Fake_Around parameters
  READ_FLOAT(FAKE_AROUND_AVOID_DISTANCE);
  READ_FLOAT(FAKE_AROUND_DISTANCE);
  READ_FLOAT(FAKE_AROUND_ANGLE_THRESHHOLD);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//========================================================================= 
void JukeAroundOpponentSkill::pickNewTechnique(int lastTechnique)
{
  //punish last technique since it failed
  if(lastTechnique >= 0 &&
     lastTechnique < NUM_TECHNIQUES)
  {
     Biases[lastTechnique] = Biases[lastTechnique] - DELTA_BIAS;    
  }

  //find which technique has the highest weight and use it 
  int numAtMax = 0;
  int indexOfMax = -1;
  float maxWeight = -32000.0f;
  
  for(int i=0; i<NUM_TECHNIQUES; i++)
  {
    if(Biases[i] > maxWeight ||
       maxWeight == -32000.0f)
    {
      maxWeight = Biases[i];
      indexOfMax = i;
      numAtMax = 1;
    }
    else if(Biases[i] == maxWeight)
      numAtMax++;
  }

  //if there is only one max the just choose it
  if(numAtMax == 1)
  {
    currentTechnique = indexOfMax;
  }
  //else pick one of the tied techniques at random
  else
  {
    int i = (rand()*numAtMax/RAND_MAX) + 1;
    for(int i2=0; i2<NUM_TECHNIQUES; i2++)
    {
      if(Biases[i2] == maxWeight)
      {
        i--;
        if(i == 0)
          currentTechnique = i2;
      }
    }
  }

  //beginning step
  techniqueStep = STEP_0; 

  //reset timer
  timer->resetTimer();

  frustrated = false;
  finished = false;
/*
  char message[100];
  switch(currentTechnique)
  {
    case PUSH_THROUGH:
<<<<<<< JukeAroundOpponentSkill.cpp
      sprintf(message,
              "Robot %d initializing JukeAroundOpponent on opponent %d, technique: pushThrough",
              robotID, 
              opponentID);
      GUI_Record.debuggingInfo.addDebugMessage(message); *
=======
      sprintf(message, "Pushing through opponent %d", opponentID);
      GUI_Record.debuggingInfo.addDebugMessage(message); 
>>>>>>> 1.17
      break;
    case MOVE_AROUND:
      sprintf(message, "Moving around opponent %d", opponentID);
      GUI_Record.debuggingInfo.addDebugMessage(message); 
      break;
    case FAKE_AROUND:
      sprintf(message, "Faking around opponent %d", opponentID);
      GUI_Record.debuggingInfo.addDebugMessage(message); 
      break;
    default:
      sprintf(message,
              "ERROR! Juke initialied but unable to find legal technique");
      GUI_Record.debuggingInfo.addDebugMessage(message); 
  }
*/
  }
//========================================================================= 
void JukeAroundOpponentSkill::pushThrough()
{
  Pair us(getLocation(sp->general.TEAM, robotID, (*currentVisionData)));

  //rotate to
  if(techniqueStep == STEP_0)
  {
	//find point on proper side of opponent we will push through
    Pair opponent(getLocation(sp->general.OTHER_TEAM, opponentID, (*currentVisionData)));
    Pair pushThroughPoint;

    pickSide(us, 
             dest, 
             opponent, 
             PUSH_THROUGH_AVOID_DISTANCE+2*sp->general.PLAYER_RADIUS,
             pushThroughPoint);
    //---
    //extend point past opponent by extension distance
    extendPoint(us,
                pushThroughPoint,
                PUSH_THROUGH_DISTANCE,
                pushToPoint);

    SpinAroundBallSkill* spinSkill = (SpinAroundBallSkill*)(skillSet->getSkill(SpinAroundBallSkill::skillNum));
    spinSkill->initialize(pushToPoint);

    techniqueStep = STEP_1;
  }
  if(techniqueStep == STEP_1)
  {
    //---
    char message[100];
    sprintf(message,"Juke: pushThrough op.%d: rotating",opponentID);
    strategy->getCurrentFrame()->setMessage(robotID, message);
    //---    

    SpinAroundBallSkill* spinSkill = (SpinAroundBallSkill*)(skillSet->getSkill(SpinAroundBallSkill::skillNum));
	spinSkill->run();

    //if our rotate is within threshhold to the destination rotation, move on
    //to step 2: push past
    if(ABS(angleDifference(getRotation(robotID, (*currentVisionData), (*sp)),
                       angleBetween(us, pushToPoint))) < PUSH_THROUGH_ANGLE_THRESHHOLD)
      techniqueStep = STEP_2;
  }
  //push past
  else
  {
    //---
    char message[100];
    sprintf(message,"Juke: pushThrough op.%d: pushing",opponentID);
    strategy->getCurrentFrame()->setMessage(robotID, message);
    //---    
    command->setPos(pushToPoint);
    command->setRotation( angleBetween(us, pushToPoint) );  
    command->setDribble(DRIBBLE_DEFAULT);
    command->setVerticalDribble(V_DRIBBLE_DEFAULT);
  }
}
//========================================================================= 
void JukeAroundOpponentSkill::moveAround()
{
  Pair us(getLocation(sp->general.TEAM, robotID, (*currentVisionData)));
  Pair opponent(getLocation(sp->general.OTHER_TEAM, opponentID, (*currentVisionData)));

  if(techniqueStep == STEP_0)
  {
    //---    
    //find point on proper side of opponent we will push through
    Pair opponent(getLocation(sp->general.OTHER_TEAM, opponentID, (*currentVisionData)));
    pickSide(us, 
             dest, 
             opponent, 
             MOVE_AROUND_AVOID_DISTANCE+2*sp->general.PLAYER_RADIUS,
             moveThroughPoint);   
    //---
    techniqueStep = STEP_1;
  }
  //rotate to
  if(techniqueStep == STEP_1)
  {
    //---
    char message[100];
    sprintf(message,"Juke: moveAround op.%d: rotating",opponentID);
    strategy->getCurrentFrame()->setMessage(robotID, message);
    SpinAroundBallSkill* spinSkill = (SpinAroundBallSkill*)(skillSet->getSkill(SpinAroundBallSkill::skillNum));
    spinSkill->initialize(moveThroughPoint);
  	spinSkill->run();

    //if our rotate is within threshhold to the destination rotation, move on
    //to step 2: move around
    if(ABS(angleDifference(getRotation(robotID, (*currentVisionData), (*sp)),
                           angleBetween(us, moveThroughPoint))) < MOVE_AROUND_ANGLE_THRESHHOLD)
    {

  	  techniqueStep = STEP_2;
      
      //determine if we're going around the left or the right
      if(angleDifference(angleBetween(us, moveThroughPoint),
                         angleBetween(us, opponent)) > 0.0f)
        goAroundLeft = true;
      else
        goAroundLeft = false;

      ManeuverWithBallSkill* maneuverSkill = (ManeuverWithBallSkill*)(skillSet->getSkill(ManeuverWithBallSkill::skillNum));
      
      if(goAroundLeft)
	  {
	      rotateAboutPoint(moveThroughPoint,
		                   opponent,
			               4*PI/6,
				           moveAroundLookToPoint);
	  }
      else
	  { 
	      rotateAboutPoint(moveThroughPoint,
		                   opponent,
			               -4*PI/6,
				           moveAroundLookToPoint);
	  }
	  
	  maneuverSkill->initialize(moveThroughPoint, moveAroundLookToPoint);
	}
  }
  //move around
  else
  {
    //---
    char message[100];
    sprintf(message,"Juke: moveAround op.%d: moving",opponentID);
	strategy->getCurrentFrame()->setMessage(robotID, message);
	//---    
	ManeuverWithBallSkill* maneuverSkill = (ManeuverWithBallSkill*)(skillSet->getSkill(ManeuverWithBallSkill::skillNum));
	maneuverSkill->run();  
  }
  
}
//========================================================================= 
void JukeAroundOpponentSkill::fakeAround()
{
  Pair us(getLocation(sp->general.TEAM, robotID, (*currentVisionData)));

  if(techniqueStep == STEP_0)
  {
    techniqueStep = STEP_1;

    //---    
    //find point on proper side of opponent we will push through
    Pair opponent(getLocation(sp->general.OTHER_TEAM, opponentID, (*currentVisionData)));

    pickSide(us, 
             dest, 
             opponent, 
             FAKE_AROUND_AVOID_DISTANCE+2*sp->general.PLAYER_RADIUS,
             fakeThroughPoint);
    //---
    //calculate point to look towards

    //are we faking around the left or the right?
    if(angleDifference(angleBetween(us, fakeThroughPoint),
                       angleBetween(us, opponent)) > 0.0f)
    {
      rotateAboutPoint(fakeThroughPoint,
                       us,
                       PI/2,
                       fakeToPoint);    
    }
    else
    {
      rotateAboutPoint(fakeThroughPoint,
                       us,
                       -PI/2,
                       fakeToPoint);    
    }
    //---    

  }
  //rotate to
  if(techniqueStep == STEP_1)
  {
    //---
    char message[100];
    sprintf(message,"Juke: fakeAround op.%d: posting",opponentID);
    strategy->getCurrentFrame()->setMessage(robotID, message);
    //---    
    SpinAroundBallSkill* spinSkill = (SpinAroundBallSkill*)(skillSet->getSkill(SpinAroundBallSkill::skillNum));
    spinSkill->initialize(fakeToPoint);
  	spinSkill->run();
  

    //if our rotate is within threshhold to the destination rotation, move on
    //to step 2: move around

	GUI_Record.debuggingInfo.setDebugPoint(robotID, 0, fakeToPoint);
	GUI_Record.debuggingInfo.setDebugPoint(robotID, 1, fakeThroughPoint);
    if(ABS(angleDifference(getRotation(robotID, (*currentVisionData), (*sp)),
                           angleBetween(us, fakeToPoint))) < MOVE_AROUND_ANGLE_THRESHHOLD)
    {
      techniqueStep = STEP_2;

      //initialize maneuverWithBall for the next frame
      Pair fakeThroughExtendedPoint;
      extendPoint(us, fakeThroughPoint, 1.0f, fakeThroughExtendedPoint);
      ManeuverWithBallSkill* maneuverSkill = (ManeuverWithBallSkill*)(skillSet->getSkill(ManeuverWithBallSkill::skillNum));
    	maneuverSkill->initialize(fakeThroughPoint, fakeThroughExtendedPoint);
    }
  }
  //fake past
  else
  {
    //---
    char message[100];
    sprintf(message,"Juke: fakeAround op.%d: fakePAST",opponentID);
    strategy->getCurrentFrame()->setMessage(robotID, message);
    //---    

    ManeuverWithBallSkill* maneuverSkill = (ManeuverWithBallSkill*)(skillSet->getSkill(ManeuverWithBallSkill::skillNum));
  	maneuverSkill->run();
  }
}
//========================================================================= 
void JukeAroundOpponentSkill::pickSide(Pair us, 
                                       Pair goalDest, 
                                       Pair obstacle, 
                                       float avoidDistance, 
                                       Pair& sidePoint)
{
  //STEPS:
  //1.) draw a line parallel to us->goalDest through the obstacle
  //2.) extend through the obstacle to a point by avoidDistance
  //3.) use rotate function to rotate that point pi/2 and -pi2 around the obstacle
  //4.) pick which point is better and use it

  //STEP 1
  Pair parallelPoint;

  //handle trivial cases that might result in a divide by 0
  if(goalDest.getX() == us.getX())
  {
    parallelPoint.setX(obstacle.getX());
    //arbitrary, since main point is that x isn't changing
    if(obstacle.getY() == us.getY())
      parallelPoint.setY(us.getY() - 10); 
    else
      parallelPoint.setY(us.getY());
  }
  else if(goalDest.getY() == us.getY())
  {
    parallelPoint.setY(us.getY());
    //arbitrary since we just want the slope to be 0
    if(us.getX() == obstacle.getX())
      parallelPoint.setX(us.getX() - 10);
    else
      parallelPoint.setX(us.getX());
  }
  //else normal case where no divide by 0's will occur...
  else
  {
    float usGoalSlope = (goalDest.getY() - us.getY())/(goalDest.getX() - us.getX());
    float perpendicularSlope = -1/usGoalSlope;

    //create a point such that the perpendicular goes through the obstacle and this point
    Pair arbitraryPoint;
    arbitraryPoint.setX(obstacle.getX() + 10);
    arbitraryPoint.setY(obstacle.getY() + perpendicularSlope*10);

    //find where perpendicular line intersects usGoalDest line
    Line usGoalDest(us,goalDest);
    Line perpendicular(obstacle,arbitraryPoint);
    Pair intersection = findLineIntersection(usGoalDest,perpendicular);

    //find the x and y displacement from the parallel line to the obstacle
    float xDiff = obstacle.getX() - intersection.getX();
    float yDiff = obstacle.getY() - intersection.getY();

    //add this displacement to "us" to get point on parallel line
    parallelPoint.setX(us.getX() + xDiff);
    parallelPoint.setY(us.getY() + yDiff);
  }

  //STEP 2
  Pair extendedPoint;
  extendPoint(parallelPoint,
              obstacle,
              avoidDistance,
              extendedPoint);

  //STEP 3
  Pair optionA,optionB;
  rotateAboutPoint(extendedPoint,
                   obstacle,
                   PI/2,
                   optionA);
  rotateAboutPoint(extendedPoint,
                   obstacle,
                   -PI/2,
                   optionB);


  //STEP 4
  if(us.distanceTo(optionA) < us.distanceTo(optionB))
    sidePoint.set(optionA);
  else
    sidePoint.set(optionB);
}
//========================================================================= 
