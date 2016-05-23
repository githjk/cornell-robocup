#include "PenaltyShotUsAimAndShootSkill.h"
#include "SimpleKickSkill.h"
#include "info/robot/robot.h"
#include "info/ball/possession.h"
#include "math/dist.h"
#include "math/angle.h"
#include "pair/pair.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"

int PenaltyShotUsAimAndShootSkill::skillNum = -1;



//===============================================================================
///Constructor.  Calls the base class Skill constructor
PenaltyShotUsAimAndShootSkill::PenaltyShotUsAimAndShootSkill(SystemParameters *sp,
                                                             VisionModule *vm,  
                                                             RoboCupStrategyModule *strat,
                                                             RobotIndex id, 
                                                             SkillSet* set) : Skill(sp,vm,strat,id, set)
{
   loadValues();
}
//===============================================================================
/// Check to see if we have the ball or not.
bool PenaltyShotUsAimAndShootSkill::isValid() 
{
  return true;
}
//===============================================================================
///resets timer
void PenaltyShotUsAimAndShootSkill::initialize() 
{
   kicked=false;
   goalieFound=false;
   timer->resetTimer();
   initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void PenaltyShotUsAimAndShootSkill::execute()
{
  //----
  //if already kicking, keep kicking  
  if(kicked)
  {
    SimpleKickSkill* skillHandle = (SimpleKickSkill*)skillSet->getSkill(SimpleKickSkill::skillNum);
    if(!skillHandle->isInitialized())
      skillHandle->initialize(KICK_SHOT_IMMEDIATE);
    skillHandle->run();
  }
  //----
  //else, decide angle at which to kick ball
  else
  {
    //get opponent goalie's current Y location
    opponentGoalie.set(-32000.0f, -32000.0f);
    goalieFound = false;

    for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
    {
      //of found, and farther ahead, update y-coord
      if(
          theirRobotFound(i, *currentVisionData, *sp) &&
          (
            getTheirRobotLocation(i, *currentVisionData, *sp).getX() > opponentGoalie.getX() ||
            !goalieFound
          )
        )
      {
        opponentGoalie.set(getTheirRobotLocation(i, *currentVisionData, *sp));
        goalieFound = true;
      }
    }

    //pick y-coord of bigger opening

    //if taking too long, keep current y-target, do not overwrite
    if(!(timer->currentTime() > TIMEOUT_AND_STOP_CHOOSING_SIDES))
    {
      //if no goalie, pick center
      if(!goalieFound)
      {
        yTarget = sp->field.SPLIT_LINE;
      }
      //else pick center of larger gap
      else
      {
        //find larger gap
        float gapLeftSize = sp->field.LEFT_GOAL_POST - opponentGoalie.getY();
        float gapRightSize = opponentGoalie.getY() - sp->field.RIGHT_GOAL_POST;
        yLeft = sp->field.LEFT_GOAL_POST - POST_OFFSET;
        yRight = sp->field.RIGHT_GOAL_POST + POST_OFFSET;

        if(gapLeftSize + 
          getRotVel(robotID,*currentVisionData,*sp)*
          sp->field.PENALTY_KICK_X_OFFSET/
          sp->general.FRAME_RATE 
                  > gapRightSize)
        {
          yTarget = yLeft;
        }
        else
        {
          yTarget = yRight;
        }
      }
    }
    
    //set robot location to current location
    //set robot rotation to rotate to this new point
    robotLoc.set(getLocation(robotID,*currentVisionData,*sp));
    command->setPos(robotLoc);
    command->setRotation(angleBetween(robotLoc.getX(),
                                      robotLoc.getY(),
                                      sp->field.THEIR_GOAL_LINE,
                                      yTarget));
    //dribble of course
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
  }

  //if current angle within tolerance, set kick boolean
  //if too much time has taken place since give up maneuver, just kick (robot can't turn)
  if(
      !kicked &&
      (
      ABS(
          angleDifference(
                          getRotation(robotID, *currentVisionData, *sp),
                          angleBetween(robotLoc.getX(),
                                       robotLoc.getY(),
                                       sp->field.THEIR_GOAL_LINE,
                                       yTarget) 
                         )
         ) < ANGLE_TOLERANCE ||
         timer->currentTime() > (TIMEOUT_AND_STOP_CHOOSING_SIDES + TIMEOUT_AND_KICK)
      )
    )
  {
    kicked = true;
    timer->resetTimer();
  }

  if(goalieFound)
  {
    GUI_Record.debuggingInfo.setDebugPoint(robotID, 0, opponentGoalie);
    GUI_Record.debuggingInfo.setDebugPoint(robotID, 1, sp->field.THEIR_GOAL_LINE,
                                                       yLeft);
    GUI_Record.debuggingInfo.setDebugPoint(robotID, 2, sp->field.THEIR_GOAL_LINE,
                                                       yRight);
  }

  GUI_Record.debuggingInfo.setDebugPoint(robotID, 3, sp->field.THEIR_GOAL_LINE + 2.0f*sp->general.PLAYER_RADIUS,
                                                     yTarget);

  if(kicked)
  {
    strategy->getCurrentFrame()->setMessage(robotID, "Shooting");
  }
  else
  {
    if(yLeft == yTarget)
      strategy->getCurrentFrame()->setMessage(robotID, "Aiming LEFT");
    else
      strategy->getCurrentFrame()->setMessage(robotID, "Aiming RIGHT");
  }
}
//===============================================================================
bool PenaltyShotUsAimAndShootSkill::evaluate() 
{
  return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void PenaltyShotUsAimAndShootSkill::update() 
{
}
//===============================================================================
///check if we've kicked at least tolerance seconds ago
/// (allows if PenaltyShotUsAimAndShootSkill push through the ball for a certain amount of time.)
bool PenaltyShotUsAimAndShootSkill::isFinished(float tolerance)
{
  return kicked && timer->currentTime()>=KICK_TIME;
}
//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void PenaltyShotUsAimAndShootSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/PenaltyShotUsAimAndShootSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(TIMEOUT_AND_STOP_CHOOSING_SIDES);
  READ_FLOAT(TIMEOUT_AND_KICK);
  READ_FLOAT(POST_OFFSET);
  READ_FLOAT(ANGLE_TOLERANCE);
  READ_FLOAT(KICK_TIME);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//===============================================================================
