#include "PullBallOffWallSkill.h"
#include "AggressivePullBallOffWallSkill.h"
#include "common/util/info/ball/ball.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"

#include "RoboCup_modules/RoboCupStrategyModule.h"

int PullBallOffWallSkill::skillNum = -1;

//===============================================================================
///Constructor.	Calls the base class's (Skill) constructor.
PullBallOffWallSkill::PullBallOffWallSkill(SystemParameters *sp,  
                                           VisionModule *vm, 
                                           RoboCupStrategyModule *strat,
                                           RobotIndex id, 
                                           SkillSet* set) : Skill(sp,vm,strat,id,set) 
{
	///Read in any parameters stored in the parameter file.
	loadValues();
}
//===============================================================================
///The prereqs of this skill are that I have not successfully pulled the ball off of the wall
///the ball the ball is less than WALL_THRESH from the side wall,
///more than OUR_CORNER_THRESH from our goal line wall
///more than THEIR_CORNER_THRESH from their goal line wall, moving slower than
///BALL_SPEED_THRESH, opponents do not have the ball, teammates do not have the ball.
bool PullBallOffWallSkill::isValid() 
{
  //never pull ball off walls in our goalie box!!!
  if(inOurGoalieBox(getBallLocation(*currentVisionData), *sp))
    return false;
 
  ///False if the ball is not in the valid zone along the side walls, not in corners
	if( !ballOnSideWall(*currentVisionData))
  {
		return false;
  }

  //Get a skill handle to pullBallOfftheCorner
  if(skillSet->getSkill(PullBallOffCornerSkill::skillNum)->isValid())
  {
    return true;
  }

	///False if the ball is moving too fast
	if( getBallSpeed( *(visionModule->getCurrentFrame()) ) > BALL_SPEED_THRESH )
  {
		return false;
  }

  //false if ball is in front of the goal
  if(inTheirGoalieBox(getBallLocation(*currentVisionData),*sp))
  {
    return false;
  }

	///False if my teammate (not me) has the ball
	for(RobotIndex i = ROBOT0; i<NUM_ROBOTS; i++)
	{
		if(i != robotID && 
       friendlyHasPossession(i, *sp) &&
       !friendlyHasPossession(robotID, *sp))
       return false;
	}

	///Finally, false if I have the ball, and have successfully removed it from the wall
	///Evaluate this criterion last because even if this is false, the skill shouldn't 
	///necessarily be valid.
	return( !ballPulledOffWall(*currentVisionData) );
}
//===============================================================================
///Perform any initializations for the skill.
void PullBallOffWallSkill::initialize() 
{
  //init aggressive version if that is the one we'll use
  if(sp->strategy2002.ENABLE_AGGRESSIVE_PULL_OFF_WALL)
  {
    skillSet->getSkill(AggressivePullBallOffWallSkill::skillNum)->initialize();
  }
  else
  {
    ///Counter of how many times we successfully get possession
    successes = 0;
    ///Counter of failures (bumping the ball away)
    failures = 0;

    frames=0;

    ///We presumably do not have the ball, so...
    haveBall = false;
  
    ///Reset timer which tracks the duration of activity for this skill
    timer->resetTimer();
    initialized=true;
    goingBehind = true;
  }
}
//===============================================================================
///Execute the skill - get possession, back and turn away from wall.
void PullBallOffWallSkill::execute()
{   
  //run aggresive version is turned on
  if(sp->strategy2002.ENABLE_AGGRESSIVE_PULL_OFF_WALL)
  {
    skillSet->getSkill(AggressivePullBallOffWallSkill::skillNum)->run();
    return;
  }

  ///If not active, dont do anything!
  if(!initialized) return;  
	///Otherwise
  
	/// If pullballoffcorner is valid call that
  //Get a skill handle to pullBallOfftheCorner
  pullCorner =(PullBallOffCornerSkill *)skillSet->getSkill(PullBallOffCornerSkill::skillNum);
  if(pullCorner->isValid())
  {
    if(!pullCorner->isInitialized())
      pullCorner->initialize();
    pullCorner->run();
  }
  else
  {
    ///Update knowledge of ball distance
	  float ballDist = distToRoboCupBall(robotID, *(visionModule->getCurrentFrame()), *sp);

	  ///Once we are fairly close to the ball, turn on the dribblers
	  ///unless they are already on.
	  if (ballDist < DRIBBLE_DIST) 
	  {
		  command->setDribble(FAST_DRIBBLE);
		  command->setVerticalDribble(FAST_V_DRIBBLE);
	  }



    Pair newDestination,ballPos,robotPos;
    float newTheta;
    ballPos = strategy->getCurrentRoboCupFrame()->getOffensiveBallLocation();
    
    //calculate which end and side it's on
    int end, side;
    if(ballPos.getX()>sp->field.HALF_LINE) end=1;
    else end=-1;
    if(ballPos.getY()>sp->field.SPLIT_LINE) side=1;
    else side=-1;

    //find out whether it's on a wall or sideine
    bool onside = MIN(ABS(sp->field.LEFT_SIDE_LINE - ballPos.getY()),
                      ABS(ballPos.getY() - sp->field.RIGHT_SIDE_LINE)) <
                  MIN(ABS(sp->field.THEIR_GOAL_LINE - ballPos.getX()),
                      ABS(ballPos.getX() - sp->field.OUR_GOAL_LINE));

    robotPos=getLocation(robotID,*currentVisionData,*sp);

    //if behind half line, pulling off side walls,  
    //and angle good, arm kicker to clear upfield
    if(
        robotPos.getX() < sp->field.HALF_LINE &&
        onside &&
        ABS(getRotation(robotID, *currentVisionData, *sp)) <= MAX_KICK_ANGLE
      )
    {
      command->setKick(KICK_PASS);
    }    


    newTheta=angleToBall(robotID, *currentVisionData, *sp);

    if(!friendlyHasPossession(robotID, 
                               *sp, 
                               *currentVisionData, 
                               *strategy->getCurrentRoboCupFrame(), 
                               true) ){
      frames=0;
    }else{
      frames++;
    }

    if(frames<POS_FRAMES)
    {
			command->setRotation(newTheta);

	    //if the angle is too wide with respect to the ball, go in front of it (16cm..)..
      float approachDistWall;
      float approachDistSide;


      //----------------------
      //----------------------
      //----------------------
		  approachDistWall=FAR_DIST*sp->general.DIST_TO_DRIBBLER;
  	  approachDistSide=FAR_DIST*sp->general.DIST_TO_DRIBBLER;
      if(onside)
      {
  		  behindLoc.setX(ballPos.getX() - approachDistSide);
        behindLoc.setY(ballPos.getY() - side*approachDistWall);
      }
      else
      {
  		  behindLoc.setY(ballPos.getY() + side*end*approachDistSide);
        behindLoc.setX(ballPos.getX() - end*approachDistWall);
      }

      //if desintation pushes robot off field, because can't get at it from behind, just
      //come at it straight
      wallsInWay = false;
      if(behindLoc.getY() < (sp->field.RIGHT_SIDE_LINE + sp->general.PLAYER_RADIUS))
      {
        newTheta = 0.0f;
        behindLoc.setY(sp->field.RIGHT_SIDE_LINE + sp->general.PLAYER_RADIUS);
        wallsInWay = true;
      }
      if(behindLoc.getY() > (sp->field.LEFT_SIDE_LINE - sp->general.PLAYER_RADIUS))
      {
        newTheta = 0.0f;
        behindLoc.setY(sp->field.LEFT_SIDE_LINE - sp->general.PLAYER_RADIUS);
        wallsInWay = true;
      }
      if(behindLoc.getX() < (sp->field.OUR_GOAL_LINE + sp->general.PLAYER_RADIUS))
      {
         if(ballPos.getY() > sp->field.SPLIT_LINE)
           newTheta = PI/2.0f;
         else
           newTheta = -PI/2.0f;

        behindLoc.setX(sp->field.OUR_GOAL_LINE + sp->general.PLAYER_RADIUS);
        wallsInWay = true;
      }
      //if into our goalie box
      if(!onside &&
         end == -1 &&
         behindLoc.getY() < sp->field.LEFT_GOALIE_BOX + sp->general.PLAYER_RADIUS &&
         behindLoc.getY() > sp->field.RIGHT_GOALIE_BOX - sp->general.PLAYER_RADIUS)
      {
        if(ballPos.getY() > sp->field.SPLIT_LINE)
        {
          behindLoc.setY(sp->field.LEFT_GOALIE_BOX + sp->general.PLAYER_RADIUS);
        }
        else
        {
          behindLoc.setY(sp->field.RIGHT_GOALIE_BOX - sp->general.PLAYER_RADIUS);
        }
        newTheta = PI;
        wallsInWay = true;
      }
      
      //----------------------
      //----------------------
      //----------------------
      if(
          goingBehind &&
          (ABS(angleDifference(getRotation(robotID, *currentVisionData, *sp), 
                               newTheta)) < FACING_ANGLE) &&
          (
            (onside && (robotPos.getX() < (ballPos.getX() - sp->general.PLAYER_RADIUS))) ||
            (!onside && (robotPos.getY()*side*end > (ballPos.getY()*side*end + sp->general.PLAYER_RADIUS*side))) ||
            (wallsInWay && (robotPos.distanceTo(behindLoc) < sp->general.PLAYER_RADIUS))
          )
        )
      {
        goingBehind = false;
      }

      if(
          !goingBehind &&
          (
            (onside && (robotPos.getX() > (ballPos.getX() + sp->general.PLAYER_RADIUS))) || 
            (!onside && (robotPos.getY()*side*end < (ballPos.getY()*side*end - sp->general.PLAYER_RADIUS*side)))
          ) &&
          !wallsInWay
        )
      {
        goingBehind = true;
      }

      if(!goingBehind)
      {
        //Head to the ball to pick it up.
        approachDistWall=NEAR_DIST*sp->general.DIST_TO_DRIBBLER;
        approachDistSide=SIDE_OFFSET;
        strategy->getCurrentFrame()->setMessage(robotID, "PullBallOffWall: Going At It");
        if(robotPos.distanceTo(ballPos) < 1.0f*sp->general.PLAYER_RADIUS)
        {
          //turn off obstacle avoidance when we're really close
          strategy->getCurrentFrame()->setMessage(robotID, "PullBallOffWall: Going At It No Avoid");
  		    command->setSpeed(CAREFUL_SPEED);
          command->setControl(OMNI_NO_AVOID);
        }
        command->setRotation(newTheta);
        if(onside)
        {
          //go to ball on side walls
  		    command->setXPos(ballPos.getX() - approachDistSide);
         
          if(ballPos.getY() > sp->field.SPLIT_LINE)
          {
            command->setYPos(sp->field.getLeftSideLine(ballPos.getX()) - approachDistWall);
          }
          else
          {
            command->setYPos(sp->field.getRightSideLine(ballPos.getX()) + approachDistWall);
          }
#if 1
          if(ballPos.getX() > sp->field.DEATH_ZONE_LINE){
            //see if an opponent's in the way
            for(RobotIndex i=ROBOT0; i< NUM_ROBOTS; i++){
              if(robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) &&
                 command->getPos().distanceTo(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData))<=
                 sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS)
              {
                Pair back=command->getPos();

                //calculate new spot to head to
                float btwnDistance=sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS;
                Pair C=getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
                float offset = (float)sqrt((btwnDistance * btwnDistance) - (ABS(C.getY() - command->getYPos())* ABS(C.getY() - command->getYPos())));

                back.setX(C.getX() - offset);

                //see if we should hold our current position
                if(back.distanceTo(ballPos) > robotPos.distanceTo(ballPos)){
                  back.setX(robotPos.getX());
                }
                command->setPos(back);
                command->setKick(KICK_PASS);
                command->setDribble(NO_DRIBBLE);
                command->setVerticalDribble(NO_V_DRIBBLE);
                break;
              }
            }
          }
#endif
        }
        else
        {
          //go to ball on end walls
  		    command->setYPos(ballPos.getY() + side*end*approachDistSide);
          if(ballPos.getX() > sp->field.HALF_LINE)
          {
            command->setXPos(sp->field.getTheirGoalLine(ballPos.getY()) - approachDistWall);
          }
          else
          {
            command->setXPos(sp->field.getOurGoalLine(ballPos.getY()) + approachDistWall);
          }
        }    
      }
	    else
      {
        // set up for going in to ball
        command->setPos(behindLoc);
        command->setRotation(newTheta);
        strategy->getCurrentFrame()->setMessage(robotID, "PullBallOffWall: Coming from behind");
	    }

      if(robotPos.distanceTo(ballPos) < 3*sp->general.PLAYER_RADIUS)
     		  command->setSpeed(CAREFUL_SPEED);

    }
    //if we have possession for this many frames, slowly pull ball off wall
    else
    {
      //go slowly, and turn dribbler to slow
		  command->setSpeed(CAREFUL_SPEED);
		  command->setDribble(SLOW_DRIBBLE);
		  command->setVerticalDribble(SLOW_V_DRIBBLE);
		  
      // If it is closer to the vertical walls than horizontal walls
      if(onside)
      {
        // pull ball in and forward
        command->setYPos(robotPos.getY() - side*SUCCESS_WALL_THRESH);
        command->setXPos(robotPos.getX() + PUSH_THROUGH_DIST);
			  command->setRotation(0.0f);
        strategy->getCurrentFrame()->setMessage(robotID, "PullBallOffWall: Side Wall");
      }
      else
      {
        // pull ball in, towards their goal, and away from our goal.
        command->setXPos(robotPos.getX() - end*SUCCESS_WALL_THRESH);
        command->setYPos(robotPos.getY() - side*end*PUSH_THROUGH_DIST);
			  command->setRotation(-end*side*PI/2.0f);
        strategy->getCurrentFrame()->setMessage(robotID, "PullBallOffWall: Front Wall");
	    }
    }
  }
}
//===============================================================================
///Counts the number of successes and failures of this skill.
bool PullBallOffWallSkill::evaluate() 
{
  ///For now, always return true
  return true;
}
//===============================================================================
///For tweaking the skill.  You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void PullBallOffWallSkill::update()
{
}
//===============================================================================
///returns true if the ball is not in our corner (> ourCornerThresh away from OUR_GOAL_LINE),
///not in their corner (> theirCornerThresh away from THEIR_GOAL_LINE)
///and ball is adjacent to a side wall (< wallThresh away)
bool PullBallOffWallSkill::ballOnSideWall(const VisionData& field)
{
  int walls = 0;
  Pair ballPos = strategy->getCurrentRoboCupFrame()->getOffensiveBallLocation();
  
  if(sp->field.THEIR_GOAL_LINE - WALL_THRESH <= ballPos.getX()) walls++;
  if(sp->field.OUR_GOAL_LINE + WALL_THRESH >= ballPos.getX()) walls++;
  if(sp->field.LEFT_SIDE_LINE - WALL_THRESH <= ballPos.getY()) walls++;
  if(sp->field.RIGHT_SIDE_LINE + WALL_THRESH >= ballPos.getY()) walls++;

  return (walls >= 1);
}
//===============================================================================
///This checks to see if I have possession of the ball and it is > successWallThresh away
///from a wall, and the ball is > successOurCornerThresh away from our goal line, and
///is > successTheirCornerThresh away from their goal line.
bool PullBallOffWallSkill::ballPulledOffWall(const VisionData& field)
{
  return (!ballOnSideWall(field) &&
           friendlyHasPossession(robotID, *sp)
         );
}
//===============================================================================
///Not yet implemented.  never finished
bool PullBallOffWallSkill::isFinished(float tolerance)
{
  return !isValid();
}
//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void PullBallOffWallSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/PullOffWallSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
  
  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  
  ///Read in a param
	READ_FLOAT(WALL_THRESH);
	READ_FLOAT(SUCCESS_WALL_THRESH);
	READ_FLOAT(BALL_SPEED_THRESH);
	READ_FLOAT(DRIBBLE_DIST);
	READ_FLOAT(CORNER_SPEED_DIST);
	READ_FLOAT(FAR_DIST );
	READ_FLOAT(NEAR_DIST );
	READ_FLOAT(FACING_ANGLE);
  READ_INT(POS_FRAMES);
  READ_FLOAT(PUSH_THROUGH_DIST);
  READ_FLOAT(SIDE_OFFSET);
  READ_FLOAT(MAX_KICK_ANGLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
}
//===============================================================================
//Sets initialized value to false, preformed when positions swtich, or plays transition
void PullBallOffWallSkill::unInitialize()
{ 
  //init aggressive version if that is the one we'll use
  if(sp->strategy2002.ENABLE_AGGRESSIVE_PULL_OFF_WALL)
  {
    skillSet->getSkill(AggressivePullBallOffWallSkill::skillNum)->unInitialize();
  }
  else
  {
    initialized = false; 
  }
}

