#include "OppositeReboundOffenseSkill.h"

#include "JamAndShootSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"

#include "common/util/math/cmath.h"

#include "common/util/math/dist.h"
#include "common/util/pair/pair.h"


int OppositeReboundOffenseSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
OppositeReboundOffenseSkill::OppositeReboundOffenseSkill(SystemParameters *sp,
                                                         VisionModule *vm, 
                                                         RoboCupStrategyModule *strat,
                                                         RobotIndex id, 
                                                         SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  loadValues();

  
  initialized = false;
}
//===============================================================================
//Call this function to check the prerequisites of the skill.  This will return 
//a bool indicating whether or not the skill is ciable in the present situation.
bool OppositeReboundOffenseSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void OppositeReboundOffenseSkill::initialize() 
{
  timer->resetTimer();
  SHOOTERID = NO_ROBOT;
  side = OUTSIDE;
  initialized = true;
}

//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void OppositeReboundOffenseSkill::initialize(RobotIndex shooterID) 
{
  timer->resetTimer();

  SHOOTERID = shooterID;
  side = OUTSIDE;
  initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void OppositeReboundOffenseSkill::execute()
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

	
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  float currentRot = angleToBall(robotID,*currentVisionData,*sp);

  //utility variables
  float closest = 32000;
  float ballDist = 0;
  RobotIndex closestRobot = NO_ROBOT;

  float AG = sp->field.THEIR_GOAL_LINE - BASE_AG ;
  float AC = RATIO_AC * AG ;
  float CG = RATIO_CG * AG ;

  // Get the  ID of the aggressor
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);

  if (aggressorID == NO_ROBOT)
  {
	  // Find the closest friendly robot to the ball and assume it to be the aggressor
	  for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
    {
		  ballDist=distanceToBall(*currentVisionData,sp->general.TEAM,i);
		  if(ballDist < closest)
      {
  		    closest = ballDist;
			    closestRobot = i;
		  }
	  }

    aggressorID = closestRobot;
  }

  /// If we want to emulate someone else as aggressor
  if(SHOOTERID != NO_ROBOT)
  { aggressorID = SHOOTERID ;}

  // Get the aggressor position
  Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp));

  // Get the intersection between shooting line and goal line. Joel's function
  Pair goalPos ;
  JamAndShootSkill *jamShoot =(JamAndShootSkill *)strategy->getSkillSet(aggressorID)->getSkill(JamAndShootSkill::skillNum);
  goalPos = jamShoot->getTargetPoint();

  if(goalPos.getX() != sp->field.THEIR_GOAL_LINE)
  { 
    goalPos.set(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE);
  }
  

  GUI_Record.debuggingInfo.setDebugPoint(robotID,0,goalPos);

  // The desired creator position
  Pair creatorPos(DEFAULT_X,DEFAULT_Y);
 
  // Get the distance between the aggressor and the goal-line
  float goalDist = dist(aggressorPos,goalPos);

  float radius0, radius1;
  int zone = -1;

  if(ABS(aggressorPos.getY() - sp->field.SPLIT_LINE) < 1.25f * 0.256f)
  {
	  radius0 = 0.2f*ABS(aggressorPos.getY() - sp->field.SPLIT_LINE) + 3.25f* 0.256f ;
	  radius1 = CG;
	  zone = 0;
  }

  if( ABS(aggressorPos.getY() - sp->field.SPLIT_LINE) > (1.25f * 0.256f) && ABS(aggressorPos.getY() - sp->field.SPLIT_LINE) < (3.75f * 0.256f) )
  {
	  radius0 =  0.2f*1.25f*0.256f + 3.25f* 0.256f +  1.2f*(ABS(aggressorPos.getY() - sp->field.SPLIT_LINE) - 1.25f * 0.256f) ;
	  radius1 =  CG +  0.2f*(ABS(aggressorPos.getY() - sp->field.SPLIT_LINE) - 1.25f * 0.256f) ;
	  zone = 1;
  }

  if(ABS(aggressorPos.getY() - sp->field.SPLIT_LINE) > 3.75f * 0.256f)
  {
    // Calculate the desired distance between aggressor and creator
	  radius0 =  0.2f*1.25f*0.256f + 3.25f* 0.256f +  1.2f*(3.75f* 0.256f - 1.25f * 0.256f) + 0.3333f*(ABS(aggressorPos.getY() - sp->field.SPLIT_LINE) - 3.75f * 0.256f);
	  radius1 =  CG +  0.2f*(3.75f* 0.256f - 1.25f * 0.256f);
	  zone = 2;
  }

  // Calculate the desired distance between goal_line and creator
  // Calculate the desired distance between aggressor and creator

  if(aggressorPos.getX()- BASE_AG > 0)
  {
    if(zone == 1)
    {
      radius0 =  radius0 + 0.2f*0.256f ;
    }
	  
    if(zone == 2)
    {
      radius0 = radius0 + 0.35f*0.256f ;
    }

    if(zone == 3)
    {
      radius0 = radius0 + 0.5f*0.256f ;
    }
  }

  if(aggressorPos.getX()- BASE_AG < 0)
  {
    if(zone == 1)
    {
      radius0 =  radius0 + 2.0f*0.256f ;
    }
	  
    if(zone == 2)
    {
      radius0 = radius0 + 1.4f*0.256f ;
    }

    if(zone == 3)
    {
      radius0 = radius0 + 0.8f*0.256f ;
    }
	  
  }

  // Intermediate calculations to find the desired location of the creator
  // Refer to http://astronomy.swin.edu.au/~pbourke/geometry/2circle/

  float a = ( (radius0*radius0)-(radius1*radius1) + goalDist*goalDist)/(2*goalDist);
  Pair P2(0,0);
  P2.setX(aggressorPos.getX() + (a*(goalPos.getX() - aggressorPos.getX())/goalDist));
  P2.setY(aggressorPos.getY() + (a*(goalPos.getY() - aggressorPos.getY())/goalDist));
  float h = SQRT(radius0*radius0 - a*a);

  // The two possible values for the creator's position
  float alt1X =  (P2.getX() + (h*(goalPos.getY()-aggressorPos.getY())/goalDist));
  float alt1Y =  (P2.getY() - (h*(goalPos.getX()-aggressorPos.getX())/goalDist));
  float alt2X =  (P2.getX() - (h*(goalPos.getY()-aggressorPos.getY())/goalDist));
  float alt2Y =  (P2.getY() + (h*(goalPos.getX()-aggressorPos.getX())/goalDist));

  if(aggressorPos.getY() > sp->field.SPLIT_LINE)
  {
  	if(alt1X <sp->field.THEIR_GOAL_LINE-sp->general.PLAYER_RADIUS && alt1X > sp->field.OUR_GOAL_LINE+sp->general.PLAYER_RADIUS && alt1Y < sp->field.SPLIT_LINE && alt1Y > sp->field.RIGHT_SIDE_LINE)
		{
			creatorPos.setX(alt1X);
			creatorPos.setY(alt1Y);
		}
		else 
    {
  		if(alt2X <sp->field.THEIR_GOAL_LINE-sp->general.PLAYER_RADIUS && alt2X > sp->field.OUR_GOAL_LINE+sp->general.PLAYER_RADIUS && alt2Y < sp->field.SPLIT_LINE && alt2Y > sp->field.RIGHT_SIDE_LINE)
      {
  			creatorPos.setX(alt2X);
    		creatorPos.setY(alt2Y);
      }
  		else
      {
		  	creatorPos.setX(DEFAULT_X);
		  	creatorPos.setY(-1*DEFAULT_Y);
      }
    }
  }
  else
  {
    if(alt1X <sp->field.THEIR_GOAL_LINE && alt1X > sp->field.OUR_GOAL_LINE && alt1Y > sp->field.SPLIT_LINE && alt1Y < sp->field.LEFT_SIDE_LINE)
		{
			creatorPos.setX(alt1X);
			creatorPos.setY(alt1Y);
		}
		else
    {
  		if(alt2X <sp->field.THEIR_GOAL_LINE && alt2X > sp->field.OUR_GOAL_LINE && alt2Y > sp->field.SPLIT_LINE && alt2Y < sp->field.LEFT_SIDE_LINE)
      { 
        creatorPos.setX(alt2X);
 			  creatorPos.setY(alt2Y);
  		}
  		else
      {
  			creatorPos.setX(DEFAULT_X);
   			creatorPos.setY(DEFAULT_Y);
      }

    }
  }

  command->setPos(creatorPos);
  command->setRotation(angleBetween(creatorPos,goalPos));
  //command->setRotation(findshootAngle(creatorPos,goalPos,aggressorPos,0.5f));
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=
  //if the angle is between 3PI/8 and -3PI/8 arm the kicker
  if(
      getRotation(robotID, *currentVisionData, *sp) < 3.0f*PI/8.0f &&
      getRotation(robotID, *currentVisionData, *sp) > -3.0f*PI/8.0f
    )
  {
      if(ABS(command->getRotation()) < (PI/2)*KICK_RATIO)
      {command->setKick(KICK_SHOT);}
   
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=

  // What if an opponent robot is in your location?
  Pair robot=getLocation(robotID,*currentVisionData,*sp);
  for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
  {
	  Pair B(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));

	  if((dist(command->getPos(),B) <= sp->general.MIN_SEPARATION_DISTANCE)  && (dist(command->getPos(),robot) > sp->general.MIN_SEPARATION_DISTANCE))
	  {  
      //Historesis: Check to see which side we should be on, and if we should switch
      if(side == INSIDE && dist(aggressorPos,command->getPos()) < 1.25f * 0.256f)
      {
        side = OUTSIDE;
      }
      else if(side == OUTSIDE && dist(aggressorPos,command->getPos()) > (1.25*0.256f) + sp->general.OPPONENT_RADIUS)
      {
        side = INSIDE;
      }

		  if(side == OUTSIDE)
      {
        if(command->getYPos() < sp->field.SPLIT_LINE)
        {
          command->setYPos(command->getYPos() - (sp->general.PLAYER_RADIUS + 2*sp->general.OPPONENT_RADIUS));
        }
        else
        {
          command->setYPos(command->getYPos() + (sp->general.PLAYER_RADIUS + 2*sp->general.OPPONENT_RADIUS));
        }
      }

      else
      {
        if(command->getYPos() < sp->field.SPLIT_LINE)
        {
          command->setYPos(command->getYPos() + (sp->general.PLAYER_RADIUS + 2*sp->general.OPPONENT_RADIUS));
        }
        else
        {
          command->setYPos(command->getYPos() - (sp->general.PLAYER_RADIUS + 2*sp->general.OPPONENT_RADIUS));
        }
      }

      command->setXPos(command->getXPos() - (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS));
    }
  }
}
//===============================================================================
bool OppositeReboundOffenseSkill::evaluate() 
{
	return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void OppositeReboundOffenseSkill::update() 
{
}
//===============================================================================
///test skill goes forever
bool OppositeReboundOffenseSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void OppositeReboundOffenseSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/OppositeReboundOffenseSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(BASE_AG);

  READ_FLOAT(RATIO_AC);

  READ_FLOAT(RATIO_CG);

  READ_FLOAT(DEFAULT_X);

  READ_FLOAT(DEFAULT_Y);

  READ_FLOAT(KICK_RATIO);


  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
