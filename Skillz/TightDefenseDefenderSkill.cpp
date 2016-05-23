#include "TightDefenseDefenderSkill.h"
#include "TightDefenseSpecialOpDSkill.h"
#include "OppositeCoverSkill.h"
#include "TandemDefenderSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/math/dist.h"
#include "common/util/defense/defending.h"
#include "common/util/pair/pair.h"
#include "GUI/GUI.h"

int TightDefenseDefenderSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
TightDefenseDefenderSkill::TightDefenseDefenderSkill(SystemParameters *sp,
                                                     VisionModule *vm, 
                                                     RoboCupStrategyModule *strat,
                                                     RobotIndex id, 
                                                     SkillSet* set) : 
                                                     Skill(sp,vm,strat,id,set) 
{
  initialized = false;
}
//=====================================================
///Call this function to check the prerequisites of the skill.	This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool TightDefenseDefenderSkill::isValid() 
{ 
  return true;
}
//=====================================================
///Default initialize
void TightDefenseDefenderSkill::initialize()
{
	// This skill might be used for shot block

	shotBlock=(ShotBlockSkill *)skillSet->getSkill(ShotBlockSkill::skillNum);
	
	//reset timer
	timer->resetTimer();
	initialized = true;
}

//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void TightDefenseDefenderSkill::execute()
{		 

	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
	}

	
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  Pair ball = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();
  float currentRot = getRotation(robotID,*currentVisionData,*sp);

  // Find number of opponent robots in the consideration zone
  int numInConsiderationZone = 0;
  float closest = 32000;
  float farthest= 0;
  RobotIndex opponentToBlock = NO_ROBOT;
  RobotIndex otherOpponent = NO_ROBOT;
  float ballDist=0;
  for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
  {
	  if(isInConsiderationZone(sp->general.OTHER_TEAM,i,strategy->getCurrentRoboCupFrame(),*currentVisionData,*sp))
    {
		  numInConsiderationZone++;
		  GUI_Record.debuggingInfo.setDebugPoint(robotID,i,getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));
      ballDist=ball.distanceTo(getLocation(sp->general.OTHER_TEAM, i, *currentVisionData));
      if(ballDist < closest){
			  closest = ballDist;
			  opponentToBlock = i;
      }
      if(ballDist > farthest){
        farthest=ballDist;
        otherOpponent=i;
      }
    }
  }
  
	command->setXPos(sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS + MORE_OFFSET);
	RobotIndex blockerID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER);
  if(blockerID != NO_ROBOT)
  {
	    Pair loc = getLocation(blockerID,*currentVisionData,*sp);
      //keep defender from getting stuck on goalie.
      if(loc.getX() > sp->field.OUR_GOALIE_BOX-sp->general.PLAYER_RADIUS)
      {
        command->setXPos(loc.getX() + 2*sp->general.PLAYER_RADIUS + MORE_OFFSET);
      }
      //if goalie way out of wack, and ball not too far back, just stick it out yourself
      if(loc.getX() > sp->field.OUR_GOALIE_BOX + 3*sp->general.PLAYER_RADIUS)
      {
        if(ball.getX() > sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS)
        {
          Skill* skillHandle = skillSet->getSkill(TandemDefenderSkill::skillNum);

          if(!skillHandle->isInitialized())
        	  skillHandle->initialize();
          skillHandle->run();
          strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Blocker must be crazy, cutting down angle");
          return;
        }
        else
        {
          command->setXPos(ball.getX());
          if(ball.getY() > sp->field.SPLIT_LINE)
          {
            command->setYPos(sp->field.LEFT_GOALIE_BOX + sp->general.PLAYER_RADIUS + 0.01f);
          }
          else
          {
            command->setYPos(sp->field.RIGHT_GOALIE_BOX - sp->general.PLAYER_RADIUS - 0.01f);
          }
          command->setRotation(angleBetween(getLocation(robotID, *currentVisionData, *sp), ball));
          strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Blocker must be crazy, but can't go into box");
          return;
        }
      }
	    
      strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Setting itself along with blocker");
      command->setYPos(loc.getY());
  }

  if(currentPos.getY() > sp->field.SPLIT_LINE)
  {
    command->setRotation(FACE_ANGLE);
  }
  else
  {
    command->setRotation(-FACE_ANGLE);
  }

  //arm the kicker if angle good and near dest, so won't kick across goal, or into goal
  if(
      currentPos.distanceTo(command->getPos()) < KICK_DIST &&
      ABS(angleDifference(currentRot,command->getRotation())) < KICK_ANGLE &&
      kickSafe(robotID, *currentVisionData, *sp, *strategy->getCurrentRoboCupFrame())
    )
  {
    command->setKick(KICK_SHOT);
  }
    
	//If robot is in that position try to stay right behind it 
  for( i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
  {	  
		Pair B(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));
    float btwnDistance = sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS;
		if(
        (dist(command->getPos(),B) < btwnDistance)  && 
        (dist(command->getPos(),currentPos) > btwnDistance) &&
        robotFound(sp->general.OTHER_TEAM, i, *currentVisionData, *sp)
      )
		{  		
      float offset = (float)sqrt((btwnDistance * btwnDistance) - (ABS(B.getX() - command->getXPos())* ABS(B.getX() - command->getXPos())));
      strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Avoiding opponent");
      if(command->getYPos() > sp->field.SPLIT_LINE){
			  command->setYPos(B.getY() - offset);
      }else{
			  command->setYPos(B.getY() + offset);
      }
		}

    Pair C(getLocation(sp->general.TEAM,i,*currentVisionData));
    btwnDistance = sp->general.PLAYER_RADIUS + sp->general.PLAYER_RADIUS;
		if(
        (i != robotID ) && 
        (i != blockerID) &&
        (dist(command->getPos(),C) < btwnDistance) &&
        (dist(command->getPos(),currentPos) > btwnDistance) &&
        robotFound(sp->general.TEAM, i, *currentVisionData, *sp)
      )
		{  
			strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Avoiding our own robot");      
      
      float offset = (float)sqrt((btwnDistance * btwnDistance) - (ABS(C.getX() - command->getXPos())* ABS(C.getX() - command->getXPos())));
      if(command->getYPos() > sp->field.SPLIT_LINE){
			  command->setYPos(C.getY() - offset);
      }else{
			  command->setYPos(C.getY() + offset);
      }

		}
  }
  
  //see if there is a specialOpDefender and if so call him properly
	// Set the Id of specialOpDefender
  RobotIndex specialOpDefenderID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(SPECIAL_OP_DEFENDER);;
   
  //exists?	
  if(specialOpDefenderID != NO_ROBOT)
  {
      //cover center of goal, anticipating a cross across by opposite team
      if(sp->strategy2002.ENABLE_OPPOSITE_COVER_SKILL)
      {
	      Skill* skillHandle = strategy->getSkillSet(specialOpDefenderID)->
             getSkill(OppositeCoverSkill::skillNum);
        if(!skillHandle->isInitialized())
          skillHandle->initialize();
        skillHandle->run();
      }
      //block passes, shots, from opponnets on opposite side of goal
      else
      {
        TightDefenseSpecialOpDSkill* skillHandle = (TightDefenseSpecialOpDSkill*)
          strategy->getSkillSet(specialOpDefenderID)->
          getSkill(TightDefenseSpecialOpDSkill::skillNum);

  	    if(numInConsiderationZone == 0)
        {
          skillHandle->initialize(NO_OPPONENTS,NO_ROBOT); 
        }
        // special op shot blocks on the only man in consideraion zone
        else
        {
  		    skillHandle->initialize(ONE_OPPONENT,opponentToBlock); 
        }

        skillHandle->run();
      }
  }
}
//=====================================================
bool TightDefenseDefenderSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void TightDefenseDefenderSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool TightDefenseDefenderSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void TightDefenseDefenderSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/TightDefenseDefenderSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################	
  READ_FLOAT(FACE_ANGLE);
  READ_FLOAT(KICK_DIST);
  READ_FLOAT(KICK_ANGLE);
  READ_FLOAT(MORE_OFFSET);
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
