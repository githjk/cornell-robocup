#include "FacilatePassSkill.h"
#include <iostream.h>
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/pair/pair.h"
#include "common/util/math/dist.h"
#include "common/util/info/robot/robot.h"
#include "SimpleScreenSkill.h"
#include "OppositeReboundOffenseSkill.h"


int FacilatePassSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
FacilatePassSkill::FacilatePassSkill(SystemParameters *sp,
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
bool FacilatePassSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void FacilatePassSkill::initialize() 
{
  timer->resetTimer();
  
  initialized = true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void FacilatePassSkill::execute()
{    
  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("FacilatePassSkill not initialized!");    
    return;  
  }
  
  // Get the aggressor and creator Position
  RobotIndex aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);
  RobotIndex creatorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(CREATOR);
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
  Pair decisionPoint,betweenShot ;

  if((aggressorID != NO_ROBOT) && (creatorID != NO_ROBOT))
  {
     Pair goalPos(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE);
     Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp));
     Pair creatorPos(getLocation(creatorID,*currentVisionData,*sp));
     extrapolateForX(aggressorPos,creatorPos,currentPos.getY(),decisionPoint);
     extrapolateForY(creatorPos,goalPos,currentPos.getX(),betweenShot);
     GUI_Record.debuggingInfo.setDebugPoint(robotID,0,decisionPoint);
     Pair midpoint((aggressorPos.getX() + creatorPos.getX())/2.0f,(aggressorPos.getY() + creatorPos.getY())/2.0f);
     
     float referenceAngle = angleBetween(aggressorPos,creatorPos);
     float beginAngle, endAngle, opponentAngle;
     RobotIndex theirGoalie;
     float closest = 32000.0f;
     Pair oppLoc;
     Pair screenedOpp;
     RobotIndex screenedID = NO_ROBOT;

     // Get the skill handle for the robot
     SkillSet* skills = strategy->getCurrentRoboCupFrame()->getStrategyModule().getSkillSet(robotID);
     
     //We will screen if we are ahead of pass and their is opponent that can obstruct the pass
     SimpleScreenSkill* screenSkill = 
    (SimpleScreenSkill*)skills->getSkill(SimpleScreenSkill::skillNum);

     //We will do oppositeReboundSkill thinking creator as aggressor
     OppositeReboundOffenseSkill* reboundSkill = 
     (OppositeReboundOffenseSkill*)skills->getSkill(OppositeReboundOffenseSkill::skillNum);


     
     getTheirGoalie(*currentVisionData,*sp,&theirGoalie);


     /// Are we ahead or behind the pass ?
     if(decisionPoint.getX() < currentPos.getX())
     {
          
          /// Make sure our robot will not cross the shooting line of creator towards goal
          if((currentPos.getY() > betweenShot.getY() && aggressorPos.getY() > betweenShot.getY()) 
             ||
             (currentPos.getY() < betweenShot.getY() && aggressorPos.getY() < betweenShot.getY()))
          {
             
          
                  if(aggressorPos.getY() > creatorPos.getY())
                  {
                    beginAngle = referenceAngle + (PI/180.0f)*OFFSET;
                    endAngle = beginAngle + (PI/180.0f)*CONE_SIZE;


                    // Find the guy who is in the cone and closest to the aggressor and screen him
                    for( RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
                    {
                      if(i != theirGoalie)
                      {

                         oppLoc = getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
                         opponentAngle = angleBetween(aggressorPos,oppLoc);
               
              
                         if((opponentAngle > beginAngle) && (opponentAngle < endAngle))
                         {
                           if(dist(oppLoc,aggressorPos) < closest)
                           {
                             closest = dist(oppLoc,aggressorPos);
                             screenedOpp = oppLoc;
                             screenedID = i;
                           }
                         }
                      }

                    }



                  }
                  else
                  {
                    beginAngle = referenceAngle - (PI/180.0f)*OFFSET;
                    endAngle = beginAngle - (PI/180.0f)*CONE_SIZE;

                    // Find the guy who is in the cone and closest to the aggressor and screen him
                    for( RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
                    {
                      if(i != theirGoalie)
                      {

                         oppLoc = getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
                         opponentAngle = angleBetween(aggressorPos,oppLoc);
               
              
                         if((opponentAngle < beginAngle) && (opponentAngle > endAngle))
                         {
                           if(dist(oppLoc,aggressorPos) < closest)
                           {
                             closest = dist(oppLoc,aggressorPos);
                             screenedOpp = oppLoc;
                             screenedID = i;
                           }
                         }
                      }

                    }
                  }


          


                  // Find the guy who is in the cone and closest to the aggressor and screen him
                  for( RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
                  {
                    if(i != theirGoalie)
                    {

                       oppLoc = getLocation(sp->general.OTHER_TEAM,i,*currentVisionData);
                       opponentAngle = angleBetween(aggressorPos,oppLoc);
               
              
                       if((opponentAngle > beginAngle) && (opponentAngle < endAngle))
                       {
                         if(dist(oppLoc,aggressorPos) < closest)
                         {
                           closest = dist(oppLoc,aggressorPos);
                           screenedOpp = oppLoc;
                           screenedID = i;
                         }
                       }
                    }

                  }

          
                  // If there is someone in the cone
                  if(closest != 32000.0f)
                  {
                    GUI_Record.debuggingInfo.setDebugPoint(robotID,1,screenedOpp);
                    strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Ahead screening opponent.");
            
                    if(dist(aggressorPos,screenedOpp) > 2*(sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS) + sp->general.OBSTACLE_CLEARANCE)
                    {
                      screenSkill->initialize(SCREEN_OPPONENT_ROBOT,screenedID,SCREEN_FRIENDLY_ROBOT,aggressorID,SCREEN_BALL,0,sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS);
                      screenSkill->run();
                    }
                    else
                    {

                       strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Ahead staying behind opponent.");
                       Pair behindPos;
                       extendPoint(aggressorPos,screenedOpp,sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS,behindPos);
                       command->setPos(behindPos);
                       command->setRotation(angleToBall(behindPos,*currentVisionData));
                    }
                  }
                  else
                  {
                      // Ok Just let d0 oppositeoffenserebound
                      reboundSkill->initialize(creatorID);
                      reboundSkill->run();
                      strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Ahead inside Ready for rebound");
                  }
            }
            else
            {
             
                      if(currentPos.getY() > creatorPos.getY())
                      {
                        command->setYPos(sp->field.LEFT_SIDE_LINE - 5*sp->general.PLAYER_RADIUS);
                      }
                      else
                      {
                        command->setYPos(sp->field.RIGHT_SIDE_LINE + 5*sp->general.PLAYER_RADIUS);
                      }  
                      command->setXPos(sp->field.THEIR_GOAL_LINE - 5*sp->general.PLAYER_RADIUS);
            }
       
             
     }
     else
     {
      
        strategy->getCurrentRoboCupFrame()->setMessage(robotID,"Behind staying at good position");
        Pair perpendicularPoint;
        if(aggressorPos.getY() > creatorPos.getY())
        {
           rotateAboutPoint(aggressorPos,midpoint,PI/2,perpendicularPoint);
        }
        else
        {

           rotateAboutPoint(aggressorPos,midpoint,-PI/2,perpendicularPoint);
        }

        Pair goodPos;
        extendPoint(perpendicularPoint,midpoint,-OFFSET_Y,goodPos);

        command->setPos(goodPos);
        command->setRotation(angleToBall(robotID,*currentVisionData,*sp));



        // If the opponent is found in your location extend more towards the aggressor
        Pair robot=getLocation(robotID,*currentVisionData,*sp);
        Pair correctedTarget;
        for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
        {
	        Pair B(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));

          if((dist(command->getPos(),B) < sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS  && 
             (dist(command->getPos(),robot) > sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS)))
		      {
             extendPoint(aggressorPos,command->getPos(),(sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS),correctedTarget);
             command->setPos(correctedTarget);
             strategy->getCurrentFrame()->setMessage(robotID, " Behind Opponent in intended position");
		      }
        }

     }

  }

}

///If the timer goes past 3 secs, reverse rotation
bool FacilatePassSkill::evaluate() 
{
  ///If not initialized, dont do anything!
  if(!initialized) return true;  


  return true;
}




//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void FacilatePassSkill::update() 
{
  ///If not initialized, dont do anything!
  if(!initialized) return;    
}



//===============================================================================
///test skill goes forever
bool FacilatePassSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void FacilatePassSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/FacilatePassSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(OFFSET);
  READ_FLOAT(CONE_SIZE);
  READ_FLOAT(OFFSET_Y);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close(); 
}
//===============================================================================
