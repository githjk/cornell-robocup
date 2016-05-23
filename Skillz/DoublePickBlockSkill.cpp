#include "DoublePickBlockSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/math/angle.h"
#include "common/util/math/dist.h"

int DoublePickBlockSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
DoublePickBlockSkill::DoublePickBlockSkill(SystemParameters *sp,
                                           VisionModule *vm, 
                                           RoboCupStrategyModule *strat,
                                           RobotIndex id, 
                                           SkillSet* set) : Skill(sp,vm,strat,id,set) 
{
  initialized = false;
}
//=====================================================
///Call this function to check the prerequisites of the skill.	This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool DoublePickBlockSkill::isValid() 
{ 
  return initialized;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void DoublePickBlockSkill::initialize()
{
	//reset timer
	timer->resetTimer();
	initialized = true;
}

///Initialize with a parameter
void DoublePickBlockSkill::initialize(Position rp, float RO)
{
	//reset timer
	timer->resetTimer();
	initialized = true;
	posOfInterest = rp;
  robotOffset = RO;

  if(posOfInterest == NO_POSITION){
    
    // Get goalie ID
    RobotIndex goalieID;
    getTheirGoalie(*currentVisionData,*sp,&goalieID);
    // Get aggressor location
	  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);
	  
    if(aggressorID != NO_ROBOT)
    {
            Pair aggressorLoc(getLocation(aggressorID,*currentVisionData,*sp));
    
            Pair rightmost;
		        float mostRight = 32000.0f;
            pickedOpponent = NO_ROBOT;
  		        // find the rightmost defender in the pick area
		        for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++){

			        Pair B(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));
              // if the opponent is behind the ball
			        if(robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) &&B.getX() > aggressorLoc.getX())	{
				        // If the ball and player are on same side (width-wise)
				        if( (B.getY() > sp->field.SPLIT_LINE && aggressorLoc.getY() >sp->field.SPLIT_LINE) || (B.getY()<sp->field.SPLIT_LINE && aggressorLoc.getY() < sp->field.SPLIT_LINE) )	{
					        if(ABS(B.getY() - sp->field.SPLIT_LINE) < mostRight && (i != goalieID))	{
						        mostRight = ABS(B.getY()- sp->field.SPLIT_LINE);
						        rightmost = B;
                    pickedOpponent = i;
					        }
				        }
			        }
		        }
            if(aggressorLoc.getY() > sp->field.SPLIT_LINE) side=1;
            else side=-1;
          }
  }

}
//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void DoublePickBlockSkill::execute()
{		 
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
	}
  
  // Get aggressor location
	RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);
	if(aggressorID != NO_ROBOT)
  {
        Pair aggressorLoc(getLocation(aggressorID,*currentVisionData,*sp));


          // If no position passed to initialize
        if(posOfInterest == NO_POSITION){

          Pair opLoc=getLocation(sp->general.OTHER_TEAM,pickedOpponent,*currentVisionData);
		      
		      char str[50];
          sprintf(str,"Setting up Double Pick on %d.",pickedOpponent);
    
          // Place the robot on the right of rightmost defender
		      //if(mostRight != 32000.0f){
          strategy->getCurrentRoboCupFrame()->setMessage(robotID, str);
          command->setXPos(opLoc.getX() + robotOffset);
		      command->setYPos(opLoc.getY() - side*(sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS + 1.1f*sp->general.OBSTACLE_CLEARANCE));
      
          /*}else{
      
            

                        // find the rightmost defender in the non pick area
		                    for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++){

			                    Pair C(getLocation(params->OTHER_TEAM,i,*currentVisionData));
                          // if the opponent is behind the ball
			                    if(robotFound(params->OTHER_TEAM,i,*currentVisionData,*params) &&C.getX() > aggressorLoc.getX())	{
				                    // If the ball and player are on same side (width-wise)
				                    if( (C.getY() > params->SPLIT_LINE && aggressorLoc.getY() <params->SPLIT_LINE) || (C.getY()<params->SPLIT_LINE && aggressorLoc.getY() > params->SPLIT_LINE) )	{
					                    if(ABS(C.getY() - params->SPLIT_LINE) < mostRight)	{
						                    mostRight = ABS(C.getY()- params->SPLIT_LINE);
						                    rightmost = C;
					                    }
				                    }
			                    }
		                    }

		                    command->setXPos(params->THEIR_GOALIE_BOX - params->PLAYER_RADIUS - FIRST_GAP);
                        // Place the robot on the right of rightmost defender
		                    if(mostRight != 32000.0f){
                          strategy->getCurrentRoboCupFrame()->setMessage(robotID, "Setting up Double Pick in non pick area");
                          command->setXPos(rightmost.getX() + robotOffset);
			                    if (rightmost.getY() > params->SPLIT_LINE)			{
				                    command->setYPos(rightmost.getY() - (params->PLAYER_RADIUS + params->OPPONENT_RADIUS + 2*params->OBSTACLE_CLEARANCE));
			                    }	else{
				                    command->setYPos(rightmost.getY() + (params->PLAYER_RADIUS + params->OPPONENT_RADIUS + 2*params->OBSTACLE_CLEARANCE));
                          }
                        }else{
      
                              strategy->getCurrentRoboCupFrame()->setMessage(robotID, "Going to default location");
                              command->setXPos(params->THEIR_GOALIE_BOX - params->PLAYER_RADIUS - FIRST_GAP);
                              if(aggressorLoc.getY() > params->SPLIT_LINE)
                                  command->setYPos(params->RIGHT_GOAL_POST);
                              else
                                  command->setYPos(params->LEFT_GOAL_POST);
                        }
          }*/
        }else{ //we've been given a position

          RobotIndex interestID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(posOfInterest);
          if(interestID != NO_ROBOT){
		        Pair interest(strategy->getCurrentFrame()->getDestination(interestID)->getPos());

		        command->setYPos(interest.getY());
		        command->setXPos(interest.getX() - GAP -2*sp->general.PLAYER_RADIUS );
            strategy->getCurrentRoboCupFrame()->setMessage(robotID, "Following leader into pick");
          }else{
            strategy->getCurrentRoboCupFrame()->setMessage(robotID, "Leader not found. :(");
          }

	      }




        // If opponent in ur place just be on its outside side
      /*
        Pair robot=getLocation(robotID,*currentVisionData,*params);
        for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++){
          Pair C(getLocation(params->OTHER_TEAM,i,*currentVisionData));

          if(robotFound(sp->general.OTHER_TEAM,i,*currentVisionData,*sp) && (dist(command->getPos(),C) < sp->general.MIN_SEPARATION_DISTANCE)  && (dist(command->getPos(),robot) > sp->general.MIN_SEPARATION_DISTANCE)){  
		          command->setYPos(command->getYPos() -side*(sp->general.MIN_SEPARATION_DISTANCE));
          }
        }
        */
        command->setRotation(angleBetween(getLocation(robotID, *currentVisionData, *sp),
                                          getBallLocation(*currentVisionData)));
  }
}
//=====================================================
bool DoublePickBlockSkill::evaluate() 
{
	return true;
}
//=====================================================
///For tweaking the skill.	You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void DoublePickBlockSkill::update() 
{
  //I am primitive, I do not learn. :(
}
//=====================================================
bool DoublePickBlockSkill::isFinished(float tolerance)
{
	return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void DoublePickBlockSkill::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	ifstream mainFile;
	mainFile.open("Params/Skill_Params/DoublePickBlockSkillParams.txt", ios::nocreate);	
	ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
	
	READ_FLOAT(GAP);
  READ_FLOAT(FIRST_GAP);
    
	//################	ADD PARAMETERS ABOVE ################
	mainFile.close();
}
//=====================================================
