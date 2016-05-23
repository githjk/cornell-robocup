#include "SimpleScreenSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "GUI/GUI.h"


int SimpleScreenSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
SimpleScreenSkill::SimpleScreenSkill(SystemParameters *sp,
                                     VisionModule *vm, 
                                     RoboCupStrategyModule *strat,
                                     RobotIndex id, 
									 SkillSet* set) : Skill(sp,vm,strat,id,set) 
{
  initialized = false;
  loadValues();
}
//=====================================================
///Call this function to check the prerequisites of the skill.  This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool SimpleScreenSkill::isValid() 
{ 
  return true;
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------

  //if points A and B are not far enough apart, then skill is invalid since it 
  //either get in the way, or collide with either robot
  //skill must be initialized to do this check!
  if(!initialized)
  {
    return false;
  }
  else
  {
    grabPair(pointA_Type, pointA_ID, &pointA);
    grabPair(pointB_Type, pointB_ID, &pointB);

    if( pointA.distanceTo(pointB) < (4*sp->general.PLAYER_RADIUS) )
      return false;
    else
      return true;
  }

}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void SimpleScreenSkill::initialize(int NpointA_Type,
                                   int NpointA_ID, 
                                   int NpointB_Type,
                                   int NpointB_ID,
                                   int NpointC_Type,
                                   int NpointC_ID)
{
  initialize(NpointA_Type,
	         NpointA_ID, 
			 NpointB_Type,
			 NpointB_ID,
			 NpointC_Type,
			 NpointC_ID,
             sp->general.OBSTACLE_CLEARANCE);
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void SimpleScreenSkill::initialize(int NpointA_Type,
                                   int NpointA_ID, 
                                   int NpointB_Type,
                                   int NpointB_ID,
                                   int NpointC_Type,
                                   int NpointC_ID,
                                   float NgraceDistance)
{
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------
  //reset timer
  timer->resetTimer();

  //set all points and grace distance needed to 
  //calculate destination for every frame
  pointA_Type = NpointA_Type;
  pointA_ID = NpointA_ID;
  pointB_Type = NpointB_Type;
  pointB_ID = NpointB_ID;
  pointC_Type = NpointC_Type;
  pointC_ID = NpointC_ID;

  //if valid grace distance specifid
  graceDistance = sp->general.PLAYER_RADIUS + 
                  NgraceDistance +                   
                  sp->general.OPPONENT_RADIUS;
  initialized = true;
}
//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void SimpleScreenSkill::execute()
{      
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------

  ///If not active, dont do anything!
  if(!initialized)
  {
    return;  
  }
  else
  {
    
    //grab the 3 points we are looking at for this frame
    grabPair(pointA_Type, pointA_ID, &pointA);
    grabPair(pointB_Type, pointB_ID, &pointB);
    grabPair(pointC_Type, pointC_ID, &pointC);
  

    int dI = 0;
    GUI_Record.debuggingInfo.setDebugPoint(robotID,
                                           dI++,
                                           pointA.getX(),
                                           pointA.getY());
    GUI_Record.debuggingInfo.setDebugPoint(robotID,
                                           dI++,
                                           pointB.getX(),
                                           pointB.getY());
    GUI_Record.debuggingInfo.setDebugPoint(robotID,
                                           dI++,
                                           pointC.getX(),
                                           pointC.getY());


    strategy->getCurrentFrame()->setMessage(robotID, "Screening...");  

    //draw a vector between points A and B, and find the destination
    //distance graceDistance away from A
    
	xDiff = pointB.getX() - pointA.getX();
    yDiff = pointB.getY() - pointA.getY();
    dist = SQRT(xDiff*xDiff + yDiff*yDiff);
    percentage = graceDistance / dist;

    Pair dest;
    dest.setX(pointA.getX() + percentage*xDiff);
    dest.setY(pointA.getY() + percentage*yDiff);

    //keep screening robot from coming back too far
    //keep destination out of our goalie box
    if(dest.getX() < sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS + sp->general.BALL_RADIUS)
    {
      dest.setX(sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS + sp->general.BALL_RADIUS);
    }

    command->setPos(dest);

    //set robot to look at point C
    command->setRotation( angleBetween(dest, pointC) );  
  }
}
//=====================================================
///If the timer goes past 3 secs, reverse rotation
bool SimpleScreenSkill::evaluate() 
{
  return true;
}
//=====================================================
///For tweaking the skill.  You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void SimpleScreenSkill::update() 
{
  ///at this time simple screening does not "learn"
}
//=====================================================
///Given inputs determines which object on the field 
///we are looking for and sets it's most recent coordinates.

void SimpleScreenSkill::grabPair(int pairType, 
                                 int pairID, 
                                 Pair* result)
{
  //friendly robot
  if(pairType == SCREEN_FRIENDLY_ROBOT)
  {
    (*result) = getLocation((RobotIndex)pairID, (*currentVisionData), (*sp));
  }
  //opponent robot
  else if(pairType == SCREEN_OPPONENT_ROBOT)
  {
    (*result) = getTheirRobotLocation((RobotIndex)pairID, (*currentVisionData), (*sp));
  }
  //the ball
  else
  {
    (*result) = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();     
  }
}
//=====================================================

///Screen is never finished, it can always continue
bool SimpleScreenSkill::isFinished(float tolerance){
  return false;
}




///Reads in the values of the parameters from the skill's parameter text file.
void SimpleScreenSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/SimpleScreenSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}