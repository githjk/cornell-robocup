#include "CageDefenseSkill.h"
#include "BlockerGainPossessionSkill.h"
#include "info/robot/robot.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "common/util/pair/pair.h"
#include "common/util/shoot/shoot.h"
#include "common/util/math/dist.h"

int CageDefenseSkill::skillNum = -1;
//===============================================================================
///Constructor.  Calls the base class Skill constructor
CageDefenseSkill::CageDefenseSkill(SystemParameters *sp,              
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
bool CageDefenseSkill::isValid() 
{ 
  return true; 
}
//===============================================================================
//Perform any initializations for the skill, such as reseting the timer.
void CageDefenseSkill::initialize() 
{
  timer->resetTimer();
  initialized = true;
  frames = 0;

}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void CageDefenseSkill::execute()
{    
	///If not active, dont do anything!
	if(!initialized)
	{
		return;  
  }

	
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));

  Pair ballLoc=strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();
  
  //figure out the location from which we want to offset location
  Pair usedLoc;
  // Choose a destination to put
  Pair dest;
  // Place the robot at certain offset_X and certain offset_Y from the ball
  int side=1;
  
  // Get the Blocker Location
  RobotIndex blockerID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER);
  
  if(
      blockerID != NO_ROBOT && 
      ABS(ballLoc.getY() - getLocation(blockerID,*currentVisionData,*sp).getY()) < sp->general.PLAYER_RADIUS
    )
  {
    usedLoc = getLocation(blockerID,*currentVisionData,*sp);
  }
  else
  {
    usedLoc = ballLoc;
  }
 

  //Figure out which side we should be on.
  if(usedLoc.getY() < sp->field.SPLIT_LINE) side=-side;
  if(strategy->getCurrentRoboCupFrame()->getPositionOfRobot(robotID) == DEFENDER) side=-side;

  //if not too far back =-)
  if((usedLoc.getX() + OFFSET_X) > (sp->field.OUR_GOALIE_BOX + 
                                    sp->general.PLAYER_RADIUS + 
                                    0.01f))
  {
    dest.setX( usedLoc.getX() + OFFSET_X);
  }
  else
  {
    dest.setX(sp->field.OUR_GOALIE_BOX + 
              sp->general.PLAYER_RADIUS + 
              0.01f);
  }

  //Find out if blocker is backing up
   SkillSet* skills = strategy->getCurrentRoboCupFrame()->getStrategyModule().getSkillSet(blockerID);
 
   BlockerGainPossessionSkill* skillHandle = 
   (BlockerGainPossessionSkill*)skills->getSkill(BlockerGainPossessionSkill::skillNum);
  
   float minOpDistance = -1.0f;
   //Pair ballLoc = getBallLocation(*currentVisionData);

   for(int ri=0; ri<NUM_ROBOTS; ri++)
   {
     if(
         robotFound(sp->general.OTHER_TEAM, (RobotIndex)ri, *currentVisionData, *sp) &&
         (
           minOpDistance < 0.0f ||
           ballLoc.distanceTo(getLocation(sp->general.OTHER_TEAM, (RobotIndex)ri, *currentVisionData)) < minOpDistance
         )
       )
     {
       minOpDistance = ballLoc.distanceTo(getLocation(sp->general.OTHER_TEAM, (RobotIndex)ri, *currentVisionData));
     }
   }

   //if blocker backing up and behind us with the ball
   if(
       (
         skillHandle->isBackingUp() && 
         strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation().getX() < 
            dest.getX() - sp->general.PLAYER_RADIUS
       ) ||
       (
         minOpDistance > 0.0f &&
         minOpDistance < 3.0f*sp->general.PLAYER_RADIUS
       )
     )
   {
  
      dest.setY(usedLoc.getY() +side*(OFFSET_Y/2.0f));
      strategy->getCurrentFrame()->setMessage(robotID,"Closing the cage");
   }
   else
   {
      dest.setY( usedLoc.getY() +side*OFFSET_Y);
      strategy->getCurrentFrame()->setMessage(robotID,"Forming the cage");
   }



  command->setPos(dest);
  if(dest.distanceTo(currentPos) < TURN_DIST){
    command->setRotation(-side*OFFSET_ANGLE);
  }else{
    command->setRotation(getRotation(robotID,*currentVisionData,*sp));
  }

  //avoid the ball if we're coming from the side.
  if(ABS(currentPos.getY() - ballLoc.getY()) < ABS(currentPos.getY() - dest.getY())){
    command->setXPos(ballLoc.getX() + sp->general.PLAYER_RADIUS + 2*sp->general.BALL_RADIUS);
  }

  // if the opponent robot is present at our desired location stay behind it
 Pair robot=getLocation(robotID,*currentVisionData,*sp);
 for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++){

	Pair B(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));
 
	if((dist(command->getPos(),B) < (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS))  && (dist(command->getPos(),robot) > (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS)))
 
	{  
		
    float intermediate = ((sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS) * (sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS))
                   - (B.squareDistanceTo(Pair(sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS,B.getX())));
    float offset;
    
    if(intermediate >= 0)
    {
      offset = (float) sqrt(intermediate);
    }
    else
    {
      offset = sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS;
    }
    
    command->setYPos(B.getY() + side*offset);
    command->setXPos(sp->field.OUR_GOALIE_BOX + sp->general.PLAYER_RADIUS);

	}
  
 }
  
  
 
}
//===============================================================================
bool CageDefenseSkill::evaluate() 
{
	return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void CageDefenseSkill::update() 
{
}
//===============================================================================
///test skill goes forever
bool CageDefenseSkill::isFinished(float tolerance)
{
  return false;
}
//===============================================================================
void CageDefenseSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/CageDefenseSkillParams.txt", ios::nocreate);
  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################  
  READ_FLOAT(OFFSET_X);
  READ_FLOAT(OFFSET_Y);
  READ_FLOAT(OFFSET_ANGLE);
  READ_INT(POS_FRAMES);
  READ_FLOAT(TURN_DIST);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}
//===============================================================================
