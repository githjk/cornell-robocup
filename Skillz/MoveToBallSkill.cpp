#include "MoveToBallSkill.h"
#include "SimpleKickSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"
#include "common/util/defense/defending.h"
#include "common/util/shoot/shoot.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/datatypes/robocup_strategy_data.h"
#include "common/util/intersect/intersect.h"
#include "GUI/GUI.h"

#include "common/util/pair/pair.h"

int MoveToBallSkill::skillNum = -1;

//===============================================================================
//===============================================================================
///Constructor.  Calls the base class Skill constructor
MoveToBallSkill::MoveToBallSkill(SystemParameters *sp,
                                 VisionModule *vm, 
                                 RoboCupStrategyModule *strat,
                                 RobotIndex id, 
                                 SkillSet* set) : Skill(sp,vm,strat,id,set) 
{
  ///Create a new SkillTimer
  possessionTimer = new VisionTimer(visionModule);
}
//===============================================================================
//===============================================================================
///The prereqs of this skill are that no one has possession
bool MoveToBallSkill::isValid() 
{
  ///See if we have the ball
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) 
  {
    if( strategy->getCurrentFrame()->getFriendlyPossession(i))
    {
      strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall !init: we have ball");
      return false;
    }
  }
  
  ///See if an opponent has the ball
  for(i=ROBOT0; i<NUM_ROBOTS; i++) 
  {
    if( strategy->getCurrentFrame()->getOpponentPossession(i))
    {
      strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall !init: they have ball");
      return false;
    }
  }
  
  //nobody has the ball, skill valid
  return true;
}
//===============================================================================
//===============================================================================
//Default init method. Does not rush the ball. 
void MoveToBallSkill::initialize() 
{
  initialize(false);
}
//===============================================================================
//===============================================================================
///Perform any initializations for the skill. Also takes in a bool to see if 
///we want to rush the ball (e.g. if the ball is between us and the goal).
void MoveToBallSkill::initialize(bool rushTheBall) 
{
  ///Indicates if we are rushing the ball
  rushBall = rushTheBall;
  
  ///Timer to determine if we have possession.
  possessionTimer->resetTimer();
  initialized = true;
}
//===============================================================================
//===============================================================================
///Execute the skill.  Go get the ball!
void MoveToBallSkill::execute()
{    
  ///If not initialized, dont do anything!
  if(!initialized) 
  {
    GUI_Record.debuggingInfo.addDebugMessage("MoveToBallSkill not initialized!");    
    return;  
  }
  
  
  //very useful information =-)
  Pair ballLoc(getBallLocation(*currentVisionData));
  Pair robotLoc(getLocation(robotID, *currentVisionData, *sp));
  float robotAngle(getRotation(robotID, *currentVisionData, *sp));
  float ballAngle = angleToBall(robotID, *currentVisionData, *sp);
  

  //Dribble once close
  if(robotLoc.distanceTo(ballLoc) <= 1.7f*sp->general.PLAYER_RADIUS)
  {
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
  }
  
  
  //the ball is between us and the their goal, rush it!
  if(ballLoc.getX() > robotLoc.getX())
  {
    rushBall=true;
  }
  
  //If we have possession, don't move to the ball any more
  if(strategy->getCurrentFrame()->getFriendlyPossession(robotID))
  {
    strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: have possession.. stopped");
    command->setPos(robotLoc);
    command->setRotation(robotAngle);
    
    //run simple kick skill if we have a shot or it's in their box right now
    if((goodCurrentShot(robotLoc,robotAngle,*currentVisionData,*sp,sp->strategy2002.SHOOT_LANE_THRESH) &&
      robotLoc.getX() > sp->field.HALF_LINE) ||
      (inTheirGoalieBox(ballLoc,*sp)))
    {
      SimpleKickSkill * kickSkill=(SimpleKickSkill *) skillSet->getSkill(SimpleKickSkill::skillNum);
      if(!kickSkill->isInitialized())
      {
        kickSkill->initialize(KICK_SHOT);
      }
      kickSkill->run();
    }
  }
  //Else, we don't have the ball. Steps to take: 
  // 1) Turn to ball
  // 2) Go to ball
  // 3) If within APPROACH_DIST and not rushing:
  //     - slow down
  //     - if not within APPROACH_ANGLE:
  //         - dont move
  //         - keep turning
  //         - if ball is REALLY close (almost touching robot), back up from it
  else
  {
    strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: going to ball");
    command->setControl(OMNI_FAST);
    
    //Enter The goalie box if their goalie is far enough away.
    RobotIndex goalie;
    if(robotLoc.getX() > sp->field.HALF_LINE){
      if(getTheirGoalie(*currentVisionData,*sp,&goalie))
      {
        if(getLocation(sp->general.OTHER_TEAM,goalie,*currentVisionData).distanceTo(robotLoc) >= 
          ENTERBOX_CAUTION_DIST+sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS)
        {
          command->setControl(OMNI_FAST_ENTERBOX);
        }
      }
      else
      {
        //if no goalie, enter the box
        command->setControl(OMNI_FAST_ENTERBOX);
      }
    }
    
    ///Turn to ball
    //if ball inside robot, keep current rotation
    if(robotLoc.distanceTo(ballLoc) < sp->general.PLAYER_RADIUS)
    {
      command->setRotation(getRotation(robotID, *currentVisionData, *sp));
    }
    else
    {
      command->setRotation(angleToBall(robotID, *currentVisionData, *sp));
    }
    
    ///Go to ball
    Pair nextToBall,goalPoint;
    if(rushBall)
    {
      //rush right at the ball
      command->setPos(ballLoc);
    }
    else 
    {
      extendPoint(robotLoc, ballLoc, -(sp->general.DIST_TO_DRIBBLER + sp->general.BALL_RADIUS), nextToBall);
      //set if we can safely come around beside it
      extrapolateForY(robotLoc,ballLoc,sp->field.OUR_GOAL_LINE,goalPoint);
      if(
          goalPoint.getY() > sp->field.LEFT_GOAL_POST + GOAL_BUFFER  ||
          goalPoint.getY() < sp->field.RIGHT_GOAL_POST - GOAL_BUFFER
        )
      {
        //Swoop off to the side of it while picking it up.
        float adjustAngle=-ADJUST_ANGLE;
        if(goalPoint.getY() < sp->field.SPLIT_LINE)  adjustAngle= -adjustAngle;
        
        rotateAboutPoint(nextToBall,ballLoc,adjustAngle,nextToBall);
      }
      //go to the calculated point
      command->setPos(nextToBall);
    }
    
    //If we are not within the approach angle, and we are
    //close we should stop moving so we can turn enough 
    //to face the ball before hitting it.
    if(
        ABS(angleDifference(ballAngle, robotAngle)) > APPROACH_ANGLE &&
        robotLoc.distanceTo(ballLoc) < FORCE_ROTATE  &&
        !inTheirGoalieBox(ballLoc,*sp)
      )
    {      
      strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: rotating");
      command->setPos(robotLoc);
      
      //If the ball is VERY close, move away from it first before rotating.
      if(robotLoc.distanceTo(ballLoc) < sp->general.PLAYER_RADIUS + sp->general.BALL_RADIUS)
      {
        command->setRotation(getRotation(robotID, *currentVisionData, *sp));
        
        extendPoint(robotLoc, ballLoc, -(sp->general.PLAYER_RADIUS + 2*sp->general.BALL_RADIUS), nextToBall);
        command->setPos(nextToBall);          
      }
    }
    
    //If we are within the approach distance, make sure the angle is good, and
    //if not rushing slow down.
    if(robotLoc.distanceTo(ballLoc) < APPROACH_DIST)
    {
      if(rushBall)
      {
   	    strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: rushing with care");
      }
      else
      {
        strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: approaching with care");
        command->setSpeed(BALL_APPROACH_SPEED);
      }
      
    }
    
    if(
        ballLoc.getX() < sp->field.DEATH_ZONE_LINE  && 
        robotLoc.getY() < sp->field.LEFT_GOALIE_BOX && 
        robotLoc.getY() > sp->field.RIGHT_GOALIE_BOX &&
        strategy->getCurrentRoboCupFrame()->getRobotByPosition(DEFENDER) != NO_ROBOT
      )
    {
      //be careful, we don't want to hit the defender.

      command->setXPos(sp->field.DEATH_ZONE_LINE + sp->general.PLAYER_RADIUS);
      if(ballLoc.getY() > sp->field.HALF_LINE){
        command->setYPos(sp->field.LEFT_SWEET_SECTOR);
      }else{
        command->setYPos(sp->field.RIGHT_SWEET_SECTOR);
      }
      strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: avoiding defender");
    }
        //aim towards the center of the goal if it's in their goalie box
    if(inTheirGoalieBox(ballLoc,*sp))
    {           
      //determine if we are charging the ball or going around first. if charging, just set rotation and go for it.
      //if not, set a new destination, face the ball at all times, and set
      //robot ot not enter goalbox.
      Line theirGoalLine(sp->field.THEIR_GOAL_LINE, sp->field.LEFT_SIDE_LINE,
                         sp->field.THEIR_GOAL_LINE, sp->field.RIGHT_SIDE_LINE);
      Line robotBallVector(robotLoc, ballLoc);
      Pair intersection;
      findLineIntersection(theirGoalLine, robotBallVector, &intersection);
      
      //good angle, attack :)
      if(
          (
            intersection.getY() < (sp->field.LEFT_GOAL_POST) &&
            intersection.getY() > (sp->field.RIGHT_GOAL_POST)
          ) ||
          (
            (
              robotLoc.getY() > ballLoc.getY() + sp->general.PLAYER_RADIUS &&
              ballLoc.getY() > sp->field.LEFT_GOAL_POST
            ) ||
            (
              robotLoc.getY() < ballLoc.getY() - sp->general.PLAYER_RADIUS &&
              ballLoc.getY() < sp->field.RIGHT_GOAL_POST
            ) 
          )
        )
      {
        command->setRotation(
                              (
                                angleBetween(
                                             robotLoc,
                                             Pair(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE)
                                            ) +
                                angleBetween(robotLoc,ballLoc)
                               ) / 2.0f
                             ); 

        //if we're in the middle and the goalie's in front us, aim to the side of the goalie.
        if(ABS(robotLoc.getX() - sp->field.SPLIT_LINE) <= MIDDLE_DIST)
        {
          Pair target;
          if(calcShot(robotLoc,
                      sp->field.THEIR_RIGHT_GOAL_WALL,
                      sp->strategy2002.SHOOT_LANE_THRESH,
                      sp->field.RIGHT_GOAL_POST,
                      sp->field.LEFT_GOAL_POST,
                      NO_ROBOT,
                      *currentVisionData,
                      *sp,
                      &target))
          {
            command->setRotation(angleBetween(robotLoc,Pair(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE)));
          }
        }     
        strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: attacking!");

        //actually, if their goalie is near the ball, then back off and hover near by
        RobotIndex goalie;
        bool goalieFound = getTheirGoalie(*currentVisionData, *sp, &goalie);
        if(
            goalieFound &&
            (
              ballLoc.distanceTo(getLocation(sp->general.OTHER_TEAM, goalie, *currentVisionData)) <
              (sp->general.OPPONENT_RADIUS + 
               MIN_GOALIE_DISTANCE)
            )
          )
        {
          Line centerToBall(sp->field.THEIR_GOAL_LINE,
                            sp->field.SPLIT_LINE,
                            ballLoc.getX(),
                            ballLoc.getY());

          Line leftGoalieBox(
                              sp->field.THEIR_GOALIE_BOX,
                              (
                                sp->field.LEFT_GOALIE_BOX + 
                                sp->general.PLAYER_RADIUS +
                                sp->general.BALL_RADIUS
                              ),
                              sp->field.THEIR_GOAL_LINE,
                              (
                                sp->field.LEFT_GOALIE_BOX + 
                                sp->general.PLAYER_RADIUS +
                                sp->general.BALL_RADIUS
                              )
                            );
          Line rightGoalieBox(
                              sp->field.THEIR_GOALIE_BOX,
                              (
                                sp->field.RIGHT_GOALIE_BOX - 
                                sp->general.PLAYER_RADIUS -
                                sp->general.BALL_RADIUS
                              ),
                              sp->field.THEIR_GOAL_LINE,
                              (
                                sp->field.RIGHT_GOALIE_BOX -
                                sp->general.PLAYER_RADIUS -
                                sp->general.BALL_RADIUS
                              )
                            );

          Pair dest1, dest2, dest3;
          dest1.set(
                     (
                       sp->field.THEIR_GOALIE_BOX - 
                       sp->general.PLAYER_RADIUS -
                       sp->general.BALL_RADIUS
                     ),
                     ballLoc.getY()
                   );
          findLineIntersection(centerToBall, leftGoalieBox, &dest2);
          findLineIntersection(centerToBall, rightGoalieBox, &dest3);

          //pick closest point
          command->setPos(dest1);
          if(ballLoc.distanceTo(dest2) < ballLoc.distanceTo(command->getPos()))
          {
            command->setPos(dest2);
          }
          if(ballLoc.distanceTo(dest3) < ballLoc.distanceTo(command->getPos()))
          {
            command->setPos(dest3);
          }

          strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: goalie in way");
        }

      }
      //bad angle, turn on goalbox avoidance, set new rotation and destination
      else
      {
        command->setRotation(angleBetween(robotLoc, ballLoc));
        Pair intersection;
 
        Line postBallLine;
        postBallLine.setAposX(sp->field.THEIR_GOAL_LINE);
        if(ballLoc.getY() > sp->field.SPLIT_LINE)
          postBallLine.setAposY(sp->field.LEFT_GOAL_POST - sp->general.PLAYER_RADIUS);
        else
          postBallLine.setAposY(sp->field.RIGHT_GOAL_POST + sp->general.PLAYER_RADIUS);
        postBallLine.setB(ballLoc);

        Line attackLine(sp->field.THEIR_GOALIE_BOX - sp->general.PLAYER_RADIUS,
                        sp->field.LEFT_SIDE_LINE,
                        sp->field.THEIR_GOALIE_BOX - sp->general.PLAYER_RADIUS,
                        sp->field.RIGHT_SIDE_LINE);
        findLineIntersection(postBallLine, attackLine, &intersection);
        command->setPos(intersection);
        command->setControl(OMNI_NORMAL);
        strategy->getCurrentFrame()->setMessage(robotID, "MoveToBall: going around to attack");
      }  
    }
    //arm kicker if we have a shot, or it's in their box
    if(
        (
          goodCurrentShot(robotLoc,
                          robotAngle,
                          *currentVisionData,
                          *sp,
                          sp->strategy2002.SHOOT_LANE_THRESH) &&
          robotLoc.getX() > sp->field.HALF_LINE
        ) ||
        (
          inTheirGoalieBox(ballLoc,*sp) && 
          towardGoal(robotLoc,
                     robotAngle,
                     sp->field.THEIR_GOAL_LINE,
                     sp->field.LEFT_GOAL_POST,
                     sp->field.RIGHT_GOAL_POST) 
        )
      )
    {
      command->setKick(KICK_SHOT);
      command->setDribble(NO_DRIBBLE);
      command->setVerticalDribble(NO_V_DRIBBLE);
    }
  }
  
  //keep in field, and our of our goalie box!!!
  Pair dLoc(command->getPos());
  constrainToField(*sp, &dLoc);
  
  if(strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER) != NO_ROBOT)
  {
    constrainOutOfOurBox(*sp, &dLoc);
  }
  
  //if ball past their goal line, keep out of their box
  if(ballLoc.getX() > (sp->field.THEIR_GOAL_LINE + 6.0f*sp->general.BALL_RADIUS))
  {
    constrainOutOfTheirBox(*sp, &dLoc);
  }
  
  command->setPos(dLoc);
}
//===============================================================================
//===============================================================================
bool MoveToBallSkill::evaluate() 
{
  return true;
}
//===============================================================================
//===============================================================================
void MoveToBallSkill::update() 
{
}
//===============================================================================
//===============================================================================
///Not yet implemented.  never finished
bool MoveToBallSkill::isFinished(float tolerance)
{
  return (possessionTimer->currentTime() > SUCCESS_TIME);
}
//===============================================================================
//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void MoveToBallSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/MoveToBallSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################ 
  //Threshold angle for approaching ball.  If angle to ball is larger than this, slow down.
  READ_FLOAT(APPROACH_ANGLE);    
  ///Threshold on how long we should have possession of the ball to count as a success
  READ_FLOAT(SUCCESS_TIME);
  ///The distance we need to be from ball to start slowing down
  READ_FLOAT(APPROACH_DIST);  
  ///Allows us to change the dist bn picker and ball
  READ_FLOAT(RADIUS_OFFSET);  
  
  READ_FLOAT(ENTERBOX_CAUTION_DIST);
  
  READ_FLOAT(FORCE_ROTATE);
  READ_FLOAT(GOAL_BUFFER);
  READ_FLOAT(ADJUST_ANGLE);
  READ_FLOAT(PASS_ANGLE);
  READ_FLOAT(MIDDLE_DIST);
  READ_FLOAT(MIN_GOALIE_DISTANCE);

  
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
}
//===============================================================================
//===============================================================================
