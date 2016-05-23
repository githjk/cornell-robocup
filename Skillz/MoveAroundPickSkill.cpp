#include "MoveAroundPickSkill.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"
#include "common/util/shoot/shoot.h"


int MoveAroundPickSkill::skillNum = -1;


///Constructor.  Calls the base class Skill constructor
MoveAroundPickSkill::MoveAroundPickSkill(SystemParameters *sp,
                                         VisionModule *vm, 
                                         RoboCupStrategyModule *strat,
                                         RobotIndex id, 
                                         SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  
  loadValues();
  initialized = false;
  sideLineLaneWidth = 3.5f*sp->general.PLAYER_RADIUS;
}
//Destructor.  Deletes all pointers in this skill
//===============================================================================
MoveAroundPickSkill::~MoveAroundPickSkill() {
  delete maneuverSkill;
}





//===============================================================================
bool MoveAroundPickSkill::isValid() { return true; }





//===============================================================================
// This initialize should never be called.  Call the next one, where we set the picker
// and the opponent
void MoveAroundPickSkill::initialize() { 
  GUI_Record.debuggingInfo.addDebugMessage("MoveAroundPick: wrong initialize() called!"); 
  initialized = false;
}






//===============================================================================
///Set the picker and opponent robots, and make sure they exist
void MoveAroundPickSkill::initialize(RobotIndex pickerRobot, RobotIndex opponentRobot,
                int pickerTeamNum, int opponentTeamNum, Pair handlerDest) 
{
  GUI_Record.debuggingInfo.addDebugMessage("MoveAroundPick: initialize called!");    
  
  //We are going to be doing some maneuvering with the ball
  maneuverSkill = (ManeuverWithBallSkill*)(skillSet->getSkill(ManeuverWithBallSkill::skillNum));

  // Friend who is picking opponent
  picker = pickerRobot;
  pickerTeam = pickerTeamNum;
  // Opponent robot being picked
  opponent = opponentRobot;
  opponentTeam = opponentTeamNum;
  // Save our destination
  dest = handlerDest;
  
  // See if the initialization causes any errors (robots not found)
  // Set initialized to false until we know there are no errors
  initialized = false;
  if(!robotFound(pickerTeam, picker, *currentVisionData, *sp)) {
    GUI_Record.debuggingInfo.addDebugMessage("MoveAroundPick: picker not found!");    
    return;
  }  
  if(!robotFound(opponentTeam, opponent, *currentVisionData, *sp)) {
    GUI_Record.debuggingInfo.addDebugMessage("MoveAroundPick: opponent not found!");    
    return;
  }
  // No errors, so set initialized to true
  initialized = true;  
}





//===============================================================================
void MoveAroundPickSkill::execute()
{      
  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("MoveAroundPickSkill not initialized!");    
    return;  
  }

  // Check for the picker and opponent robots
  if(!robotFound(pickerTeam, picker, *currentVisionData, *sp)) {
    GUI_Record.debuggingInfo.addDebugMessage("MoveAroundPick: picker not found!");    
    return;
  }  
  if(!robotFound(opponentTeam, opponent, *currentVisionData, *sp)) {
    GUI_Record.debuggingInfo.addDebugMessage("MoveAroundPick: opponent not found!");    
    return;
  }
  
  //Get locations of the robots
  handlerLoc = getLocation(robotID, *currentVisionData, *sp);
  opponentLoc = getLocation(opponentTeam,opponent, *currentVisionData);
  pickerLoc = getLocation(pickerTeam, picker, *currentVisionData); 

  
    

  // 1) If you can move forward, do so.  First, find a temporary destination that is in the 
  // same direction as the real destination, but closer.  This avoids the problem of an 
  // opponent far away and near the destination point messing up the call to isLane().  The 
  // handler still has most of the lane to his destination in this case, and should move 
  // around the pick.
  theta = angleBetween(handlerLoc, dest);
  tempDest.setX(handlerLoc.getX() + 5*sp->general.PLAYER_RADIUS*COS(theta));
  tempDest.setY(handlerLoc.getY() + 5*sp->general.PLAYER_RADIUS*SIN(theta));
  
  // See if there is an open space in front of you (the handler).  If so, move forward.
  if(isLane(handlerLoc,tempDest,1.3f*sp->general.PLAYER_RADIUS,*currentVisionData,*sp,false)) {    
    //GUI_Record.debuggingInfo.addDebugMessage("lane upfield exists"); 
    if( maneuverSkill->isValid() ) {
      maneuverSkill->initialize(dest, dest);
      strategy->getCurrentFrame()->setMessage(robotID, "Moving upfield around pick");  
      maneuverSkill->run();
    }
    else strategy->getCurrentFrame()->setMessage(robotID, "MoveAroundPick: Don't have ball!");
  }

  // 2) Else if there is enough room between picker and closest sideline, move to the 
  //    sideline.  The skill will then go to 1) above, and move forward.
  else if(pickerLoc.getY() > 0.0f && pickerLoc.getY() > opponentLoc.getY() &&
          sp->field.LEFT_SIDE_LINE - pickerLoc.getY() > sideLineLaneWidth) {

    //Set our temp destination to be next to the sideline
    tempDest.set(handlerLoc.getX(), sp->field.LEFT_SIDE_LINE - sideLineLaneWidth/2);
    //If we are close enough to the sideline, set our temp destination to be upfield
    if( dist(handlerLoc,tempDest) < 0.03 ) {    
      tempDest.setX(tempDest.getX() + 5*sp->general.PLAYER_RADIUS);
    }
    //Go to the temp destination
    maneuverSkill->initialize(tempDest, tempDest);
    strategy->getCurrentFrame()->setMessage(robotID, "Moving to sideline around pick");  
    maneuverSkill->run();
  }

  else if(pickerLoc.getY() <= 0.0f && pickerLoc.getY() < opponentLoc.getY() &&
          pickerLoc.getY() - sp->field.RIGHT_SIDE_LINE > sideLineLaneWidth) {
    
    //Set our temp destination to be next to the sideline
    tempDest.set(handlerLoc.getX(), sp->field.RIGHT_SIDE_LINE + sideLineLaneWidth/2);
    //If we are close enough to the sideline, set our temp destination to be upfield
    if( dist(handlerLoc,tempDest) < 0.03 ) {    
      tempDest.setX(tempDest.getX() + 5*sp->general.PLAYER_RADIUS);
    }
    //Go to the temp destination
    maneuverSkill->initialize(tempDest, tempDest);
    strategy->getCurrentFrame()->setMessage(robotID, "Moving to sideline around pick");  
    maneuverSkill->run();
  }

  // 3) Else move to the center to get open.  This is for two reasons: First, the 
  //    picker is picking the opponent on the center side, so we go to the center to get 
  //    around the picker.  Otherwise, the picker is picking closer to the sideline, but 
  //    there is not enough room to go up the sideline, so we go to the center.  This will 
  //    either lure the opponent to the center and create a hole on the sideline, or will 
  //    allow us to get around the opponent if he's lazy.
  else {
    //Find the angle from the handler to the side to which it should move to get around the picker
    theta = angleBetween(handlerLoc, dest);
    if(pickerLoc.getY() > 0.0f) theta =- PI/2;
    else theta =+ PI/2;

    //Find dest to go to to get around the picker (or opponent).  Another way is to 
    //find a point 90  or more degrees around the picker.
    tempDest.setX(handlerLoc.getX() + 4*sp->general.PLAYER_RADIUS*COS(theta));
    tempDest.setY(handlerLoc.getY() + 4*sp->general.PLAYER_RADIUS*SIN(theta));
    
    //Go to the temp destination
    maneuverSkill->initialize(tempDest, tempDest);
    strategy->getCurrentFrame()->setMessage(robotID, "Moving to sideline around pick");  
    maneuverSkill->run();
  }
  

  //What happens if we move to the side of the picker, but then can't get by bc there
  //is no good lane?
}





//===============================================================================
bool MoveAroundPickSkill::evaluate() {
  ///If not initialized, dont do anything!
  if(!initialized) return true;  

  return true;
}





//===============================================================================
void MoveAroundPickSkill::update() {
  ///If not initialized, dont do anything!
  if(!initialized) return;  
}

///Not yet implemented.  never finished
bool MoveAroundPickSkill::isFinished(float tolerance){
  return false;
}






//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void MoveAroundPickSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/MoveAroundPickSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  


  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}





