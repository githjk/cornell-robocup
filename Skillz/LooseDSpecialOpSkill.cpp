#include "LooseDSpecialOpSkill.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"
#include "common/util/defense/defending.h"
#include "common/util/intersect/intersect.h"

int LooseDSpecialOpSkill::skillNum = -1;

//===============================================================================
///Constructor.  Calls the base class Skill constructor
LooseDSpecialOpSkill::LooseDSpecialOpSkill(SystemParameters *sp,
                                           VisionModule *vm, 
                                           RoboCupStrategyModule *strat,
                                           RobotIndex id, 
                                           SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
 
  loadValues();
  initialized = false;
}


//===============================================================================
bool LooseDSpecialOpSkill::isValid() {   
  currentVisionData = visionModule->getCurrentFrame();
  return isRobocupBallFound(*currentVisionData);
}


//===============================================================================
void LooseDSpecialOpSkill::initialize() {
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  //If the ball is on the right side, cover the left side
  if(strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation().getY() < sp->field.SPLIT_LINE)
    side = LEFT;
  //Otherwise cover the right side
  else side = RIGHT;

  initialized = true;
}





//===============================================================================
void LooseDSpecialOpSkill::execute()
{      
  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("LooseDSpecOP skill not initialized!");    
    return;  
  }

  //If ball not found, do nothing
  
  strategy->getCurrentFrame()->setMessage(robotID, "LooseDSpecialOpSkill");     
  ballLoc = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation();  
  
  //See if we want to switch sides... switch if the ball is a certain threshold
  //past the split line.
  //If we are covering the left side and the ball is sufficiently far on
  //the left side, cover the right side (we want to cover side opposite from 
  //ball's side)
  if(side == LEFT && ballLoc.getY() > sp->field.SPLIT_LINE + SWITCH_THRESH)
    side = RIGHT;
  //And vice versa...
  else if(side == RIGHT && ballLoc.getY() < sp->field.SPLIT_LINE - SWITCH_THRESH)
    side = LEFT;



  //Always face ball
  command->setRotation(angleToBall(robotID, *currentVisionData, *sp));
  //Find opponents op1 and op2.  numOpps ranges from 0-2, depending on which we've found
  numOpps = findOpponents(op1,op2);  


  //If we found both op1 and op2, call findOp2Shot() now so we don't have to 
  //call it twice later.  We now know if op2 has a shot on goal
  if(numOpps == 2) {
    op2HasShot = findOpShot(op2,robotID, leftShot, rightShot, *currentVisionData, *sp);    
  }
  
  //If only op1 is found, or op2 is found but has no shot, cover op1 as close 
  //as possible.  Ignore op2.
  if(numOpps == 1 || (numOpps == 2 && !op2HasShot) ) {
    if(!theirRobotFound(op1, *currentVisionData, *sp)) {
      strategy->getCurrentFrame()->setMessage(robotID, "Error: no op1 found");
    }
    else {
      op1Loc = getTheirRobotLocation(op1, *currentVisionData, *sp);
      command->setXPos( op1Loc.getX() + 2.3f*sp->general.PLAYER_RADIUS*COS(angleToBall(op1Loc,*currentVisionData)) );
      command->setYPos( op1Loc.getY() + 2.3f*sp->general.PLAYER_RADIUS*SIN(angleToBall(op1Loc,*currentVisionData)) );
 
      char message[50];
      sprintf(message, "Covering robot %d", op1);
      strategy->getCurrentFrame()->setMessage(robotID, message);     
    }  
  }

  //Both op1 and op2 are found, and op2 has a shot.  Cover the pass to op1, 
  //and cover op2's potential shot (should he get passed to).  To do both 
  //of these things, position robot in the intersection of op1's pass line 
  //and op2's shot line.
  else if(numOpps == 2) {
    //Normal case: op1 how lowest x-coordinate, op2 is closer to ball
    //If op2 is not blocking a pass to op1, do shot block on op2 and pass block on op1.
    //Otherwise, do shot block on op2
    op1Loc = getTheirRobotLocation(op1, *currentVisionData, *sp);
    op2Loc = getTheirRobotLocation(op2, *currentVisionData, *sp);
    op2Shot.set( sp->field.OUR_GOAL_LINE , (leftShot+rightShot)/2 );
    
    //CASE 1: (Normal case) op2 is closer to ball.  See if op2 is blocking pass to op1
    if( dist(op2Loc, ballLoc) < dist(op1Loc, ballLoc) ) {
      //Op2 is not blocking a pass to op1. Op1 is open!
      if( distFromPointToLine(ballLoc,op1Loc,op2Loc) > 1.5f*sp->general.PLAYER_RADIUS) 
      {
        char message[50];
        sprintf(message, "Covering robot %d and %d", op1, op2);
        strategy->getCurrentFrame()->setMessage(robotID, message);     

        command->setPos(findLineIntersection(ballLoc, op1Loc, op2Loc, op2Shot));
    
        GUI_Record.debuggingInfo.setDebugPoint(robotID,0,op2Shot);
        GUI_Record.debuggingInfo.setDebugPoint(robotID,1,Pair(sp->field.OUR_GOAL_LINE,leftShot) );
        GUI_Record.debuggingInfo.setDebugPoint(robotID,2,Pair(sp->field.OUR_GOAL_LINE,rightShot) );
      }
      //Op2 is blocking a pass to op1.  Do shot block on op2
      else {
        strategy->getCurrentFrame()->setMessage(robotID, "op1 not open. ShotBlock on op2");
        command->setXPos( op2Loc.getX() + 2.3f*sp->general.PLAYER_RADIUS*COS(angleBetween(op2Loc,op2Shot)) );
        command->setYPos( op2Loc.getY() + 2.3f*sp->general.PLAYER_RADIUS*SIN(angleBetween(op2Loc,op2Shot)) );
      }
    }

    //CASE 2: op1 has lowest x-coord and is still closer to ball than op2.  In this case, do
    //shot block on op1, ignoring op2.
    else 
    {
      char message[50];
      sprintf(message, "PassBlock on robot %d", op1);
      strategy->getCurrentFrame()->setMessage(robotID, message);     

      //Op1 has a shot.  Block it!
      if(findOpShot(op1,robotID, leftShot, rightShot, *currentVisionData, *sp)) {
        op1Shot.set( sp->field.OUR_GOAL_LINE , (leftShot+rightShot)/2 );        
      }
      //Block a shot to center of goal
      else {
        op1Shot.set( sp->field.OUR_GOAL_LINE , sp->field.SPLIT_LINE );
      }
      command->setXPos( op1Loc.getX() + 2.3f*sp->general.PLAYER_RADIUS*COS(angleBetween(op1Loc,ballLoc)) );
      command->setYPos( op1Loc.getY() + 2.3f*sp->general.PLAYER_RADIUS*SIN(angleBetween(op1Loc,ballLoc)) );
    }
  }


  //We didn't find op1 or op2, so go to the default location.
  else 
  {
    strategy->getCurrentFrame()->setMessage(robotID, "Going to default loc");  
    
    //ultimate default destination
    Pair dest;
    dest.setX((sp->field.DEATH_ZONE_LINE + sp->field.DEFENSE_ZONE_LINE)/2.0f);
    
    //current robot location
    Pair robotLoc = getLocation(robotID, *currentVisionData, *sp);
    
    if(side == LEFT)
      dest.setY((sp->field.LEFT_MIDDLE_SECTOR + sp->field.LEFT_SWEET_SECTOR)/2.0f);
    else
      dest.setY((sp->field.RIGHT_MIDDLE_SECTOR + sp->field.RIGHT_SWEET_SECTOR)/2.0f);

    command->setPos(dest);

    //split angle between ball and target locations, based on current position of robot
    float angleToBall = angleBetween(robotLoc, ballLoc);
    Pair goalTarget(sp->field.THEIR_GOAL_LINE,
                    sp->field.SPLIT_LINE);
    float angleToTarget = angleBetween(robotLoc, goalTarget);

    command->setRotation((angleToBall + angleToTarget)/2.0f);
  }

}





//===============================================================================
///Finds the opponents that we must cover, returns the number (0-2) found
int LooseDSpecialOpSkill::findOpponents(RobotIndex &op1, RobotIndex &op2) 
{
  currentVisionData = visionModule->getCurrentFrame();
  
  ballX = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation().getX();
  ballY = strategy->getCurrentRoboCupFrame()->getDefensiveBallLocation().getY();

  //Iterate through all opponents and check if we should ignore them or not
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) {
    //At first, we assume we ignore no opponents
    ignore[i]=false;
    
    //Ignore robot if it doesn't exist!
    if(!theirRobotFound(i, *currentVisionData, *sp)) {
      ignore[i] = true;
      continue;
    }
    
    //Also ignore if the robot has possession, is sufficiently far behind
    //the ball (in x-direction only), or opponent is not on the side we
    //are covering
    oppX = getTheirXLocation(i, *currentVisionData, *sp);
    oppY = getTheirYLocation(i, *currentVisionData, *sp);
    if(dist(oppX,oppY,ballX,ballY) < 2.0f*sp->general.OPPONENT_RADIUS
      || oppX > ballX + GRACE 
      || (oppY >= sp->field.SPLIT_LINE && side == RIGHT) 
      || (oppY <  sp->field.SPLIT_LINE && side == LEFT ) ) {
      ignore[i] = true;
    }
  }
  
  //See how many opponents are left to consider guarding
  oppsLeft = 0;
  for(i=ROBOT0; i<NUM_ROBOTS; i++) {
    if(!ignore[i]) oppsLeft++;
  }
  
  //If none are left to guard, return 0
  if(oppsLeft == 0) return 0;
  
  //Find op1: the opponent closest to goal in the x-direction only.
  op1 = NO_ROBOT;
  minX = 10000.0f;
  for(i=ROBOT0; i<NUM_ROBOTS; i++) {
    if(!ignore[i]) {
      oppX = getTheirXLocation(i, *currentVisionData, *sp);
      if(oppX < minX) {
        op1 = i;
        minX = oppX;
      }
    }
  }
  //We should not consider op1 anymore, since we've already found him.
  //We also don't want a situation where op1 == op2
  ignore[op1] = true;

  //If only op1 exists, return op1
  if(oppsLeft == 1) {
    return 1;
  }

  //Otherwise we also have to find op2, who is the opponent closest to
  //the ball
  else {
    op2 = NO_ROBOT;
    minBall = 10000.0f;
    for(i=ROBOT0; i<NUM_ROBOTS; i++) {
      if(!ignore[i]) {
        op2Loc = getTheirRobotLocation(i, *currentVisionData, *sp);
        distToBall = dist(op2Loc.getX(), op2Loc.getY(), ballX, ballY);
        if(distToBall < minBall) {
          op2 = i;
          minBall = distToBall;
        }
      }
    }
  
    return 2;
  }

}


//===============================================================================
bool LooseDSpecialOpSkill::evaluate() {
  ///If not initialized, dont do anything!
  if(!initialized) return true;  

  return true;
}





//===============================================================================
void LooseDSpecialOpSkill::update() {
  ///If not initialized, dont do anything!
  if(!initialized) return;  
}

///Not yet implemented.  never finished
bool LooseDSpecialOpSkill::isFinished(float tolerance){
  return false;
}




//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void LooseDSpecialOpSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/LooseDSpecialOpSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  
  READ_FLOAT(GRACE);
  READ_FLOAT(GOAL_LINE_OFFSET);
  READ_FLOAT(SIDELINE_OFFSET);
  READ_FLOAT(SWITCH_THRESH);
 
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}



//===============================================================================

