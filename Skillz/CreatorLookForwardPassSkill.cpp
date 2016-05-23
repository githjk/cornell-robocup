#include "CreatorLookForwardPassSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/pass/pass.h"
#include "common/util/math/dist.h"
#include "common/util/pair/pair.h"
#include "common/util/shoot/shoot.h"

int CreatorLookForwardPassSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
CreatorLookForwardPassSkill::CreatorLookForwardPassSkill(SystemParameters *sp,
                                                         VisionModule *vm, 
                                                         RoboCupStrategyModule *strat,
                                                         RobotIndex id, 
                                                         SkillSet* set) : 
                                                         Skill(sp,vm,strat,id,set) 
{
  initialized = false;
}
//=====================================================
///Call this function to check the prerequisites of the skill.  This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool CreatorLookForwardPassSkill::isValid() 
{ 
  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void CreatorLookForwardPassSkill::initialize()
{
  //reset timer
  timer->resetTimer();
  initialized = true;

  //figure out whether we should use line mode or not.
  line_mode= ((float)rand())/RAND_MAX <= LINE_CHANCE;

  //calculate the random stuff
  x=((rand()*2.0f)/RAND_MAX - 1.0f)*SPOT_SIZE + sp->field.THEIR_GOALIE_BOX + X_POS;
  y=((rand()*2.0f)/RAND_MAX - 1.0f)*SPOT_SIZE + sp->field.HALF_LINE + Y_POS;
  y_line_rand=((rand()*2.0f)/RAND_MAX - 1.0f)*Y_ZONE_WIDTH;

  // Get the ID of aggressor
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);

  // If no robot assigned as aggressor
  if (aggressorID == NO_ROBOT)
  {
    float closest = 32000;
    float ballDist = 0;
    RobotIndex closestRobot = NO_ROBOT;

    // Find the closest friendly robot (not me) to the ball and assume it to be the aggressor
    for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
    {
      ballDist=distanceToBall(*currentVisionData,sp->general.TEAM,i);
      if(
          (ballDist < closest || closestRobot == NO_ROBOT) && 
          (i != robotID)
        )
      {
        closest = ballDist;
        closestRobot = i;
       }
    }

    aggressorID = closestRobot;
  }

  // Get the aggressor Position
  Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp)) ;

  // Find the xzone in which the aggressor is
  if( aggressorPos.getX() <= zone1Line )
  {
	  xzone = 1;
  }
  else if( aggressorPos.getX() > zone1Line  &&  aggressorPos.getX() <= zone2Line)
  {
	  xzone = 2;
  }

  else if( aggressorPos.getX() > zone2Line && aggressorPos.getX() <= zone3Line)
  {
	  xzone = 3;
  }
  else
  {
	  xzone = 4;
  }
  
  // Find the yzone in which the aggressor is
  if( aggressorPos.getY() > sp->field.SPLIT_LINE)
  {
	  yzone = 1;
  }
  else
  {
	  yzone = 0;
  }
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void CreatorLookForwardPassSkill::execute()
{    
  ///If not active, dont do anything!
  if(!initialized)
  {
    return;  
  }
  
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  

  // Maximum X distance
  float maximum = sp->field.THEIR_GOAL_LINE - sp->field.HALF_LINE ;
  // Middle X distance
  float middle = (2.0f/3.0f)*maximum ;
  // Low X distance
  float low = (1.0f/3.0f)*maximum ;

  // Indicates whether it is possible to pass or not
  bool isPass,isShot;

  // The Xdistance
  float addlineX ;

  // the constant YLine;
  float yLine;

  // The two Y boundaries
  float yleft, yright;

  // The two X boundaries
  float xleft, xright ;
  //utility variables

  float closest = 32000;
  float ballDist = 0;
  RobotIndex closestRobot = NO_ROBOT;

  // The final point calculated for creator
  Pair target,correctedTarget;

  // The shot calculated for the creator
  Pair goalPos;

  // Get the ID of aggressor
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);

  // If no robot assigned as aggressor
  if (aggressorID == NO_ROBOT)
  {
    // Find the closest friendly robot to the ball and assume it to be the aggressor
    for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
    {
      ballDist=distanceToBall(*currentVisionData,sp->general.TEAM,i);
      if(
          (ballDist < closest || closestRobot == NO_ROBOT) && 
          (i != robotID)
        )
      {
        closest = ballDist;
        closestRobot = i;
       }
    }

    aggressorID = closestRobot;
  }

  // Get the aggressor Position
  Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp)) ;

  //Rotation of robot for zone-1 and zone-2
  float currentRot = angleBetween(currentPos,aggressorPos);

  //Hysterisis on aggressor's position
  if( xzone == 1 && aggressorPos.getX() > ((zone1Line - hyster)))
  {xzone = 2;}
  
  if( xzone == 2 && aggressorPos.getX() < ((zone1Line + hyster)))
  {xzone = 1;}
  
  if( xzone == 2 && aggressorPos.getX() > ((zone2Line - hyster)))
  {xzone = 3;}
  
  if( xzone ==3 && aggressorPos.getX() < ((zone2Line + hyster)))
  {xzone = 2;}
  
  if( xzone ==3 && aggressorPos.getX() > zone3Line)
  {xzone = 4;}

  if( xzone ==4 && aggressorPos.getX() < zone3Line)
  {xzone = 3;}

  if(yzone == 1 && aggressorPos.getY() < sp->field.SPLIT_LINE - sp->general.PLAYER_RADIUS)
  {yzone = 0;}
  else if(yzone == 0 && aggressorPos.getY() > sp->field.SPLIT_LINE + sp->general.PLAYER_RADIUS)
  {yzone = 1;}

  // Do the stuff you need to do for the xzone stuff
  if(xzone == 1)
  {
    addlineX = maximum + aggressorPos.getX();
    if(yzone == 1){
      yleft = sp->field.LEFT_SIDE_LINE;
      yright = sp->field.LEFT_GOAL_POST; 
    }else{
      yleft = sp->field.RIGHT_GOAL_POST;
      yright = sp->field.RIGHT_SIDE_LINE; 
    }
    strategy->getCurrentFrame()->setMessage(robotID, "Looking for pass backfield");
  }
  else if( xzone == 2 )
  {
		  addlineX = middle + aggressorPos.getX();
      strategy->getCurrentFrame()->setMessage(robotID, "Looking for pass lower midfield");

          if(yzone == 1)
          {
		          yleft = sp->field.SPLIT_LINE;
		          yright = sp->field.RIGHT_SIDE_LINE;
          }
          else
          {
              yleft = sp->field.LEFT_SIDE_LINE;
              yright = sp->field.SPLIT_LINE;
          }

  }
  else 
  {
  //  For zone 3 and zone 4 we will be calculating shadow on vertical Y line    
      if( xzone == 3)
      {
    	    xleft = sp->field.THEIR_GOAL_LINE;
          xright = zone2Line + (aggressorPos.getX() - zone2Line);
          strategy->getCurrentFrame()->setMessage(robotID, "Looking for pass upper midfield");
      }
      else if( xzone == 4)
      {
          xleft = sp->field.THEIR_GOAL_LINE;
          xright = zone3Line;
          strategy->getCurrentFrame()->setMessage(robotID, "Looking for pass Upfield");
      }


  }
  
  
  // For zone 1 and zone 2 we want to find the best possible pass on some X-Line
  if(xzone == 1 || xzone == 2)
  {
    isPass = calcShot(aggressorID,addlineX,sp->strategy2002.PASS_LANE_THRESH,
                      yright,yleft,robotID,
                      *currentVisionData,*sp,&target);

    if(isPass) 
    {command->setPos(target);}
    else
    {
      command->setXPos(addlineX);
      command->setYPos((yleft + yright)/2);
    }
    command->setRotation(currentRot);
    if (xzone==2 && !line_mode){
      ///go to spot in front of goal.
      command->setPos(x,y);

      command->setRotation(currentRot);
      isPass=isLane(aggressorPos,command->getPos(),currentPos,
                    sp->strategy2002.PASS_LANE_THRESH/3.0f,
                    *currentVisionData,*sp,true);
      target=command->getPos();
      strategy->getCurrentFrame()->setMessage(robotID, "Heading in front of goal");
    }
  }
  else
  {

  // For zone 3 and zone 4 we want to find the best possible pass on some line Y
      if(yzone == 1)
      {
		       yLine = sp->field.RIGHT_SWEET_SECTOR - yoffset + y_line_rand;
      }
      else
      { 
           yLine = sp->field.LEFT_SWEET_SECTOR +yoffset+y_line_rand ;
      }

      isPass = calcYShot(aggressorID,yLine,sp->strategy2002.PASS_LANE_THRESH,
                         xright,xleft,robotID,
                         *currentVisionData,*sp,&target);

      if(isPass) 
      {command->setPos(target);}
      else
      {
          command->setXPos((xleft + xright)/2);
          command->setYPos(yLine);
      }
  }

  // While In zone-4 we want the kicker to be ON 

  if(xzone >= 3)
  {
    isShot = calcShot(robotID,sp->field.THEIR_GOAL_LINE,
                      sp->strategy2002.SHOOT_LANE_THRESH,sp->field.RIGHT_GOAL_POST,
                      sp->field.LEFT_GOAL_POST,NO_ROBOT,
                      *currentVisionData,*sp,&goalPos);
    
    if(!isShot)
    {
      goalPos.setX(sp->field.THEIR_GOAL_LINE);
      goalPos.setY((sp->field.LEFT_GOAL_POST + sp->field.RIGHT_GOAL_POST)/2);
    }

    //command->setRotation(findshootAngle(command->getPos(),goalPos,aggressorPos,0.5f));
    command->setRotation(findshootAngle(currentPos,goalPos,aggressorPos,0.5f));
    if(ABS(command->getRotation()) < ((PI/2)*FACTOR))
    {command->setKick(KICK_SHOT);}
  }

  
  

  
  
  // If the opponent is found in your location extend more towards the aggressor
  Pair robot=getLocation(robotID,*currentVisionData,*sp);
  for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
  {
	  Pair B(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));

    if((dist(command->getPos(),B) < sp->general.MIN_SEPARATION_DISTANCE  && 
       (dist(command->getPos(),robot) > sp->general.MIN_SEPARATION_DISTANCE)))
		{
       extendPoint(aggressorPos,command->getPos(),-sp->general.MIN_SEPARATION_DISTANCE,correctedTarget);
       command->setPos(correctedTarget);
       strategy->getCurrentFrame()->setMessage(robotID, "Opponent in intended position");
		}
  }



  /// If passing is ON and if there is a pass possible
  if(PASSING == 1)
  {
    if(isPass)
    {
      strategy->getCurrentRoboCupFrame()->setPassDest(robotID,target);
      strategy->getCurrentRoboCupFrame()->setPassValue(robotID,true);
    }
    else
    {
      strategy->getCurrentRoboCupFrame()->setPassValue(robotID,false);
    }
  }
}
//=====================================================
bool CreatorLookForwardPassSkill::evaluate() 
{
  return true;
}
//=====================================================
///For tweaking the skill.  You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void CreatorLookForwardPassSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool CreatorLookForwardPassSkill::isFinished(float tolerance)
{
  return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void CreatorLookForwardPassSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/CreatorLookForwardPassSkillParams.txt", ios::nocreate); 
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
  
  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################ 
  READ_FLOAT(zone1Line);
  READ_FLOAT(zone2Line);
  READ_FLOAT(zone3Line);
  READ_FLOAT(yoffset);
  READ_FLOAT(hyster);
  READ_INT(PASSING);
  READ_FLOAT(LINE_CHANCE);
  READ_FLOAT(Y_ZONE_WIDTH);
  READ_FLOAT(X_POS);
  READ_FLOAT(Y_POS);
  READ_FLOAT(SPOT_SIZE);
  READ_FLOAT(FACTOR);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
}
//=====================================================
