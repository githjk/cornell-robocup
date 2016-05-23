#include "StripBallSkill.h"
#include "CarefulStripBallSkill.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/info/ball/possession.h"
#include "common/util/info/ball/ball.h"
#include "common/util/info/robot/robot.h"
#include "common/util/math/angle.h"
#include "common/util/pair/pair.h"
#include "common/util/intersect/intersect.h"
#include "common/util/math/angle.h"
#include "InterceptBallSkill.h"
#include "common/util/2002Strategy/2002Strategy.h"
#include "plays/2002plays/ThreeManDefense.h"

int StripBallSkill::skillNum = -1;

//===============================================================================
///Constructor.  Calls the base class Skill constructor
StripBallSkill::StripBallSkill(SystemParameters *sp,
                               VisionModule *vm, 
                               RoboCupStrategyModule *strat,
                               RobotIndex id, 
                               SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();
}
//===============================================================================
//Skill is valid if an opponent has possession, and not too many opponent or friendlies on ball
bool StripBallSkill::isValid() 
{ 

  //if in their goalie box don't call me
  if(inTheirGoalieBox(getBallLocation(*currentVisionData), *sp))
    return false;

  //they don't have possesion, don't call me!
  if(!theyHavePossession(*currentVisionData, *sp, &opponentID))
    return false;

  //Don't strip from goalie
  RobotIndex goalie;
  getTheirGoalie(*currentVisionData,*sp,&goalie);
  if(opponentID==goalie) return false;

  //if too many friendlies or too many robots, don't call me!!

  //assume no opponents
  numRobots = 0;
  numFriendly = 0;

  //for each opponent found, increment counter. if prev found avg two points.
  //if more than 3 then break out of loop, since we'll end up bailing anyways
  for(int team=0; team<NUM_TEAMS; team++)
  {
    for(RobotIndex robot=ROBOT0; robot<NUM_ROBOTS; robot++)
    {
      if(robotFound(team, robot, *currentVisionData, *sp))
      {
        tRobotLoc.set(getLocation(team, robot, *currentVisionData));
        if(
          //robot near enough, tol diff for different team
          (
          (team == sp->general.TEAM &&
          ballLoc.distanceTo(tRobotLoc) < 2.0f*sp->general.PLAYER_RADIUS) ||
          (team == sp->general.OTHER_TEAM &&
          ballLoc.distanceTo(tRobotLoc) < 2.0f*sp->general.OPPONENT_RADIUS)
          ) &&
          //not me!
          !(team == sp->general.TEAM && robot == robotID)
          )
        {
          numRobots++;
          //if friendly guy, inc that counter
          if(team == sp->general.TEAM)
            numFriendly++;
          
          //first guy, use his loc
          if(numRobots == 1)
            opponentLoc.set(tRobotLoc);
          //second guy, average two
          else if(numRobots == 2)
          {
            opponentLoc.setX((opponentLoc.getX() + tRobotLoc.getX()) / 2.0f);
            opponentLoc.setY((opponentLoc.getY() + tRobotLoc.getY()) / 2.0f);
          }
          
          //if 3 or more robots, or 2 or more friendlies, we ain't going to strip
          if(numRobots >= 3 || numFriendly >=2)
          {               
            strategy->getCurrentFrame()->setMessage(robotID, "Strip: Too many robots, DON'T CALL ME!");
            return false;
          }
        }
      }
    }
  }
  //huh, not too many robots, must be valid
  return true;
} 
//===============================================================================
void StripBallSkill::initialize() 
{
  //run careful mode if param value is set
  if(sp->strategy2002.ENABLE_CAREFUL_STRIP_BALL)
  {
    skillSet->getSkill(CarefulStripBallSkill::skillNum)->initialize();
  }
  else
  {
    if(isValid())
    { 
      //first step is facing the opponent
      stripping_step = APPROACH;
      initialized = true;
      stripLeft = true;
    }
    else
    {
      initialized = false;
    }
  
    dangerousRobot = false;
    ballStripped = false;
  }
}
//===============================================================================
void StripBallSkill::execute()
{      
  //run careful mode if param value is set
  if(sp->strategy2002.ENABLE_CAREFUL_STRIP_BALL)
  {
    skillSet->getSkill(CarefulStripBallSkill::skillNum)->run();
    return;
  }

  //-------------------------------------------------------------------------
  ///If not initialized bail
  if(!initialized) 
  {
    strategy->getCurrentFrame()->setMessage(robotID, "Strip Not Intialized");
    return;
  }
  //-------------------------------------------------------------------------
  //get robots current location and angle
  robotLoc.set(getLocation(robotID,*currentVisionData,*sp));
  robotAngle = getRotation(robotID, *currentVisionData, *sp);
  //set ball loc
  ballLoc.set(strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation());
  //-------------------------------------------------------------------------
  //determine opponent to strip from, if no opponent has the ball just go straight for it
  if(!theyHavePossession(*currentVisionData, *sp, &opponentID))
  {
    Skill* interceptHandle = skillSet->getSkill(InterceptBallSkill::skillNum);
    interceptHandle->initialize();
    interceptHandle->run();
    return;
  }
  //else set opponent location
  else
  {
    //assume no opponents
    numRobots = 0;
    numFriendly = 0;

    //for each opponent found, increment counter. if prev found avg two points.
    //if more than 3 then break out of loop, since we'll end up bailing anyways
    for(int team=0; team<NUM_TEAMS; team++)
    {
      for(RobotIndex robot=ROBOT0; robot<NUM_ROBOTS; robot++)
      {
        if(robotFound(team, robot, *currentVisionData, *sp))
        {
          tRobotLoc.set(getLocation(team, robot, *currentVisionData));
          if(
              //robot near enough, tol diff for different team
              (
                (team == sp->general.TEAM &&
                 ballLoc.distanceTo(tRobotLoc) < 2.0f*sp->general.PLAYER_RADIUS) ||
                (team == sp->general.OTHER_TEAM &&
                 ballLoc.distanceTo(tRobotLoc) < 2.0f*sp->general.OPPONENT_RADIUS)
              ) &&
              //not me!
              !(team == sp->general.TEAM && robot == robotID)
            )
          {
            numRobots++;
            //if friendly guy, inc that counter
            if(team == sp->general.TEAM)
              numFriendly++;

            //first guy, use his loc
            if(numRobots == 1)
              opponentLoc.set(tRobotLoc);
            //second guy, average two
            else if(numRobots == 2)
            {
              opponentLoc.setX((opponentLoc.getX() + tRobotLoc.getX()) / 2.0f);
              opponentLoc.setY((opponentLoc.getY() + tRobotLoc.getY()) / 2.0f);
            }

            //if 3 or more robots, or 2 or more friendlies, we ain't going to strip
            if(numRobots >= 3 || numFriendly >=2)
            {               
              strategy->getCurrentFrame()->setMessage(robotID, "Strip: Too many robots, DON'T CALL ME!");
              return;
            }
          }
        }
      }
    }
  }
  //-------------------------------------------------------------------------
  //determine:
  //-incoming angle + strip point
  //-outcoming angle + scoop point
  //------------------------------------------------------------------------
  //stripping left or right?
  Pair frontPoint;
  extendPoint(ballLoc, opponentLoc, -STRIPPING_DISTANCE, frontPoint);

  if(stripLeft && 
     angleBetween(frontPoint, opponentLoc) > LEFT_VS_RIGHT_HISTORISIS)
  {
    stripLeft = false;
  }
  else if(!stripLeft &&
          angleBetween(frontPoint, opponentLoc) < -LEFT_VS_RIGHT_HISTORISIS)
  {
    stripLeft = true;
  }
  //------------------------------------------------------------------------
  //now, based on if stripping left or right, push 
  //the stripPoint and the stripAvoidPoint to the side
  //so that robot comes from the side slightly
  //note, if opponent is coming towards our goal, it is dangerious to offset these points
  //because agggressor will get out of the way of robot and he'll have a free kick on our goal.
  //solution is to check to see if opponent has ball 
  //and is aiming within a cone towards our goal. if so
  //don't offset these points, just come in head on.

  Pair centerOfGoal(sp->field.OUR_GOAL_LINE, sp->field.SPLIT_LINE);
  if(ABS(angleDifference(angleBetween(opponentLoc, ballLoc),
                         angleBetween(opponentLoc, centerOfGoal))) < 
                            (HEAD_ON_THRESHOLD - HEAD_ON_THRESHOLD_HISTORISIS) &&
     !dangerousRobot)
  {
    dangerousRobot = true;
  }
  
  if(ABS(angleDifference(angleBetween(opponentLoc, ballLoc),
                         angleBetween(opponentLoc, centerOfGoal))) >
                           (HEAD_ON_THRESHOLD + HEAD_ON_THRESHOLD_HISTORISIS) &&
     dangerousRobot)
  {
    dangerousRobot = false;
  }
    
  if(dangerousRobot)
  {  
    extendPoint(frontPoint, opponentLoc, -(sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS), stripPoint);
    extendPoint(opponentLoc, frontPoint, 2.0f*SIDE_SHIFT, stripAvoidPoint);
  }
  else
  {
    Pair sideStrip;

    if(stripLeft)
    {
      rotateAboutPoint(opponentLoc, frontPoint, PI/2.0f, sideStrip);
    } 
    else
    {
      rotateAboutPoint(opponentLoc, frontPoint, -PI/2.0f, sideStrip);
    } 

    extendPoint(sideStrip, frontPoint, 2.0f*SIDE_SHIFT, stripAvoidPoint);
    extendPoint(sideStrip, frontPoint, SIDE_SHIFT, stripPoint);

    //if stripper will knock ball when going to strip avoid point, push it out away from the opponent
    Pair intersection;
    Line opponentBall(opponentLoc, ballLoc);
    Line robotAvoid(robotLoc, stripAvoidPoint);
    findLineIntersection(opponentBall, robotAvoid, &intersection);
    if(intersection.getX() != NEVER_INTERSECT &&
       robotLoc.distanceTo(intersection) < robotLoc.distanceTo(stripAvoidPoint) &&
       intersection.distanceTo(ballLoc) < 2.5f*sp->general.PLAYER_RADIUS)
    {
      Pair temp;
      if(stripLeft)
      {
        rotateAboutPoint(robotLoc, stripAvoidPoint, PI/2.0f, temp);
      }
      else
      {
        rotateAboutPoint(robotLoc, stripAvoidPoint, PI/2.0f, temp);
      }

      extendPoint(temp, stripAvoidPoint, 2.5f*sp->general.PLAYER_RADIUS, stripAvoidPoint);
    }
  }
  //------------------------------------------------------------------------  
  if(stripLeft)
    incomingAngle = normalizeAngle(angleBetween(robotLoc, ballLoc) + 
                                   INCOMING_ANGLE_OFFSET);
  else
    incomingAngle = normalizeAngle(angleBetween(robotLoc, ballLoc) - 
                                   INCOMING_ANGLE_OFFSET);

  //scoop point and angle
  extendPoint(frontPoint, opponentLoc, 2.0f*STRIPPING_DISTANCE, scoopPoint);

  float angle;
  if(stripLeft)
    angle = PI/2.0f;
  else
    angle = -PI/2.0f;

  angle = angle*0.8f;

  rotateAboutPoint(scoopPoint, frontPoint, angle, scoopPoint);
  outcomingAngle = normalizeAngle(incomingAngle + angle);

  //if still scooping, use last scoop point
  if(stripping_step == TURN_AND_SCOOP)
  {
    scoopPoint.set(lastScoopPoint);
  }
  lastScoopPoint.set(scoopPoint);
  //-------------------------------------------------------------------------
  
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 0, ballLoc);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 1, opponentLoc);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 2, stripPoint);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 3, stripAvoidPoint);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 4, scoopPoint);
  
  //-------------------------------------------------------------------------
  //using historisis, change step if necessary:
  //
  // Steps:
  // 1.) Approach
  // 2.) Attack!
  // 3.) Turn and scoop
  //
  
  //APPROACH -> ATTACK
  if(stripping_step == APPROACH &&
     robotLoc.distanceTo(stripAvoidPoint) < APPROACH_TO_ATTACK_THRESHOLD &&
     ABS(angleDifference(robotAngle, angleBetween(robotLoc, ballLoc))) < ANGLE_THRESHOLD)
  {
     stripping_step = ATTACK;
     timer->resetTimer();
  }
  //ATTACK -> TURN_AND_SCOOP
  else if(
           (
             stripping_step == ATTACK &&
             robotLoc.distanceTo(stripPoint) < START_TURNING_DISTANCE
           ) &&
           (
             timer->currentTime() > ATTACK_TO_SCOOP_DELAY ||
             isThreeManDefenseFormed(*currentVisionData,
                                     *strategy->getRelativeRoboCupFrame(-1),
                                     *sp,
                                     THREE_MAN_TOLERANCE_DISTNACE) ||
             strategy->getCurrentRoboCupFrame()->getCurrentPlay() != ThreeManDefense::playNum
           )
         )
  {
     stripping_step = TURN_AND_SCOOP;
  }

  //TURN_AND_SCOOP -> APPROACH 
 
  //defensive approach
  if(robotLoc.getX() < sp->field.HALF_LINE)
  {
    if(stripping_step == TURN_AND_SCOOP &&
       robotLoc.distanceTo(stripPoint) > STOP_SCOOP_DISTANCE &&
       !friendlyHasPossession(robotID, *sp))
    {
      stripping_step = APPROACH;
      //if our angle isn't horrible, kick
      if(ABS(robotAngle) < WORST_KICK_ANGLE)
      {
        command->setKick(KICK_SHOT);
      }
    }
  }
  //offensive approach
  else
  {
    if(stripping_step == TURN_AND_SCOOP && 
       robotLoc.distanceTo(stripPoint) > STOP_SCOOP_DISTANCE &&
       !friendlyHasPossession(robotID,
                              *sp,
                              *currentVisionData,
                              *strategy->getCurrentRoboCupFrame(),
                              true))
    {
      stripping_step = APPROACH;
    }
  }

  //-------------------------------------------------------------------------
  //Execute Proper Step:

  //APPROACH
  if(stripping_step == APPROACH)
  {
    //attack at incoming angle
    command->setRotation(angleToBall(robotID, *currentVisionData, *sp));	
    command->setControl(OMNI_NORMAL);
    command->setPos(stripAvoidPoint);
    
    //if robot is dangerous (aka heading toward our goal), or if two opponents, attack head on)
    if(dangerousRobot || numRobots == 2)
      strategy->getCurrentFrame()->setMessage(robotID, "Strip: Approach Head On");
    else
      strategy->getCurrentFrame()->setMessage(robotID, "Strip: Approach Offset");
  }
  //ATTACK
  else if(stripping_step == ATTACK)
  {
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
    //attack at incoming angle
    command->setRotation(incomingAngle);
  
    //if within stop avoidance distance, 
    //turn off obstacle avoidance
    if(robotLoc.distanceTo(stripPoint) < STOP_AVOIDANCE_DISTANCE)
    {
      command->setControl(OMNI_NO_AVOID);
      command->setPos(stripPoint);
      if(dangerousRobot)
        strategy->getCurrentFrame()->setMessage(robotID, "Strip: ATTACK Head On");
      else
        strategy->getCurrentFrame()->setMessage(robotID, "Strip: ATTACK Offset");
    }
    else
    {
      command->setControl(OMNI_NORMAL);
      command->setPos(stripPoint);
      if(dangerousRobot)
        strategy->getCurrentFrame()->setMessage(robotID, "Strip: attack Head On");
      else
        strategy->getCurrentFrame()->setMessage(robotID, "Strip: attack Offset");
    }
  }
  //TURN_AND_SCOOP
  else
  {
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
    strategy->getCurrentFrame()->setMessage(robotID, "Strip: Scooping");
    //scoop at outcoming angle
    command->setPos(scoopPoint);
    command->setRotation(angleBetween(robotLoc, scoopPoint));

    //don't avoid opponent
    command->setControl(OMNI_NO_AVOID);
   
    //if we are significantly away from the strip point and the ball is still lost, and
    //we are on the offensive side, pretend the ball is stripped to allow us to go to single 
    //assist offense
    if(opponentLoc.distanceTo(getLocation(robotID, *currentVisionData, *sp)) - 
       sp->general.PLAYER_RADIUS -
       sp->general.OPPONENT_RADIUS > SUCCESS_DISTANCE &&
       friendlyHasPossession(robotID,
                             *sp,
                             *currentVisionData,
                             *strategy->getCurrentRoboCupFrame(),
                             true))   
    {
      ballStripped = true;
    }
  }

  //-------------------------------------------------------------------------
}
//===============================================================================
bool StripBallSkill::evaluate() 
{
  return true;
}
//===============================================================================
void StripBallSkill::update() 
{

}
//===============================================================================
bool StripBallSkill::isFinished(float tolerance)
{
  return !initialized;
}
//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void StripBallSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/StripBallSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################

  READ_FLOAT(LEFT_VS_RIGHT_HISTORISIS);

  READ_FLOAT(INCOMING_ANGLE_OFFSET);

  READ_FLOAT(ANGLE_THRESHOLD);

  READ_FLOAT (APPROACH_TO_ATTACK_THRESHOLD);

  READ_FLOAT(START_TURNING_DISTANCE);

  READ_FLOAT(STOP_AVOIDANCE_DISTANCE);

  READ_FLOAT(STRIPPING_DISTANCE);

  READ_FLOAT(STOP_SCOOP_DISTANCE);

  READ_FLOAT(SUCCESS_DISTANCE);

  READ_FLOAT(SIDE_SHIFT);
  
  READ_FLOAT(HEAD_ON_THRESHOLD);

  READ_FLOAT(HEAD_ON_THRESHOLD_HISTORISIS);

  READ_FLOAT(WORST_KICK_ANGLE);
  WORST_KICK_ANGLE = WORST_KICK_ANGLE*PI;

  READ_FLOAT(THREE_MAN_TOLERANCE_DISTNACE);
  READ_FLOAT(ATTACK_TO_SCOOP_DELAY);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================
bool StripBallSkill::ballIsStripped()
{
  return ballStripped && initialized;
}
//===============================================================================



