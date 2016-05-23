#include "PickSkill.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/dist.h"
#include "common/util/math/angle.h"

int PickSkill::skillNum = -1;

//===============================================================================
///Constructor.  Calls the base class Skill constructor
PickSkill::PickSkill(SystemParameters *sp,
                     VisionModule *vm, 
                     RoboCupStrategyModule *strat,
                     RobotIndex id, 
                     SkillSet* set) : Skill(sp,vm,strat,id, set) 
{
  handlerInitialized = false;
  enemyInitialized = false;
  loadValues();
  initialized = false;
  //Give 4 player radii as sideline width.  The fifth is for half of the picker's body.
  sidelineLaneWidth = 5*sp->general.PLAYER_RADIUS;
  pickDist = sp->general.PLAYER_RADIUS + sp->general.OPPONENT_RADIUS + 3*sp->general.BALL_RADIUS;  
}


//===============================================================================
bool PickSkill::isValid() {   
  return true; 
}


//===============================================================================
void PickSkill::initialize() {
  handlerInitialized = false;
  enemyInitialized = false;
}




//===============================================================================
void PickSkill::initialize(RobotIndex handlerRobot, RobotIndex opponentRobot,
                int handlerTeamNum, int opponentTeamNum, 
                Pair handlerDest, bool goToCenter) 
{
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------
  
  // Initialize the handler and opponent
  handlerInitialized = false;
  enemyInitialized = false;

  handler = handlerRobot;
  opponent = opponentRobot;
  handlerTeam = handlerTeamNum;
  opponentTeam = opponentTeamNum;
  dest = handlerDest;
  goCenter = goToCenter;

  // See if the initialization causes any errors (robots not found)
  if(!robotFound(handlerTeam, handler, *currentVisionData, *sp)) {
    GUI_Record.debuggingInfo.addDebugMessage("Pick skill: handler not found");    
    return;
  }
  else{
    handlerInitialized = true;
  }

  if(!robotFound(opponentTeam, opponent, *currentVisionData, *sp)) {
    GUI_Record.debuggingInfo.addDebugMessage("Pick skill: opponent not found");    
    return;
  }
  else{
    enemyInitialized = true; 
    opponentLoc = getLocation(opponentTeam, opponent, *currentVisionData);  
  }

  // Initialize the "old" pick location to be where we are standing now.  That way, if 
  // we are close to the opponent and pickLoc is not updated with the new pick location
  // (pickLoc1), we will stay where we are and hold our ground.
  pickLoc.set(command->getPos());
  //Finally, indicate that we have initialized the skill
  initialized = true;


  //Now we must find the correct angle at which to pick the opponent.  This will be 
  //used as basis for the current pick angle while the skill is running

  // CHOOSING A PICK LOCATION
  // ------------------------
  // We can pick on either side of the opponent.  To choose which side to pick on:
  //0) If goCenter = true, we must choose the pick loc closest to middle of field.
  //    In this case, we're done choosing a pick loc.
  //1) Otherwise: Draw a line from handler to handler's dest.  Choose pick location
  //    closest to that line.
  //2) Alternatively, we could just choose the pick location closest to the picker,
  //    thus avoiding having to move around the opponent to get to the pick loc
  //3) After doing either 1 or 2, check to make sure there is a large enough path
  //    between picker and sideline for handler to travel through (this is 
  //    explained immediately below.

  pickAngle = angleBetween(opponentLoc, dest);
  x = opponentLoc.getX();
  y = opponentLoc.getY();  
  // Find pick location 1
  pickLoc1.setX(x + COS(pickAngle - PI/2) * pickDist);
  pickLoc1.setY(y + SIN(pickAngle - PI/2) * pickDist);
  // Find pick location 2
  pickLoc2.setX(x + COS(pickAngle + PI/2) * pickDist);
  pickLoc2.setY(y + SIN(pickAngle + PI/2) * pickDist);
   
  //  Draw a line from handler to handler's destination.  First try a pick location 
  //closest to that line, so the handler doesn't have to make a big arc around the 
  //picker.  Then, check to see if there is enough room between picker and sideline
  //for the handler to move;  
  //  If we want the handler to move towards the center or the field, swap pickLoc 1 
  //and 2 if 2 is closer to the center.  We dont have to check for room between 
  //picker and sideline since we are picking toward the middle
  if(goCenter) {
    if( ABS(pickLoc2.getY()) < ABS(pickLoc1.getY()) )
      pickAngle =  normalizeAngle(pickAngle + PI/2);
    else pickAngle =  normalizeAngle(pickAngle - PI/2);
    return;
  }
  else {
    //Get distance from picker to line between handler and handler's destination
    pickLocDist1 = distFromPointToLine(handlerLoc, dest, pickLoc1);
    pickLocDist2 = distFromPointToLine(handlerLoc, dest, pickLoc2);

    //Alternatively, we could choose the pick loc closest to the picker
    //pickLocDist1 = dist(handlerLoc, pickLoc1);
    //pickLocDist2 = dist(handlerLoc, pickLoc2);

    if(pickLocDist2 < pickLocDist1) {
      pickLoc1.swap(pickLoc2);
      pickAngle = normalizeAngle(pickAngle + PI/2);
    }
    else pickAngle =  normalizeAngle(pickAngle - PI/2);

    // Check to see if there is enough room between the opponent and the sidelines.
    //If not, use the other pick loc (which is 180 degrees from the current pickAngle).
    if(pickLoc1.getY() > opponentLoc.getY() ) {
      if(sp->field.LEFT_SIDE_LINE - pickLoc1.getY() < sidelineLaneWidth)
        pickAngle = normalizeAngle(pickAngle + PI);
    }
    else {
      if(pickLoc1.getY() - sp->field.RIGHT_SIDE_LINE < sidelineLaneWidth)
        pickAngle = normalizeAngle(pickAngle + PI);
    }
  }
  //Now we have the pick angle!
  
}
//===============================================================================
void PickSkill::execute()
{      
  //----------------------------
  ///ALWAYS call the following 2 functions to get the latest data
  currentVisionData = visionModule->getCurrentFrame();
  command = strategy->getCurrentFrame()->getDestination(robotID);
  ///ALWAYS call the previous 2 functions to get the latest data
  //----------------------------

  ///If not initialized, dont do anything!
  if(!initialized) {
    GUI_Record.debuggingInfo.addDebugMessage("Pick skill not initialized!");    
    return;  
  }
  
  //Print out any errors that may have occurred:  
  //Check for opponent and handler and get current locations.  If they aren't found,
  //use old location
  if(!robotFound(opponentTeam, opponent, *currentVisionData, *sp) ) {
    strategy->getCurrentFrame()->setMessage(robotID, "Pick Not Initialized");
    GUI_Record.debuggingInfo.addDebugMessage("Pick skill: opponent not found");    
  }
  else {
    enemyInitialized = true; 
    opponentLoc = getLocation(opponentTeam, opponent, *currentVisionData);  
  }
  if(!robotFound(handlerTeam, handler, *currentVisionData, *sp) ) {
    strategy->getCurrentFrame()->setMessage(robotID, "Pick Not Initialized");
    GUI_Record.debuggingInfo.addDebugMessage("Pick skill: handler not found");    
  }
  else {
    handlerInitialized = true;
    handlerLoc = getLocation(handler, *currentVisionData, *sp);
  }
  //If the opponent or handler are still not initialized, we can't do anything.
  //We must return
  if(!enemyInitialized || !handlerInitialized) return;




  // Get data needed for determining pick locations
  x = opponentLoc.getX();
  y = opponentLoc.getY();  
    
  // Check to see if there is enough room between the opponent and the sidelines.
  //If not, use the other pick loc (which is 180 degrees from the current pickAngle).
  //Take away 3 ball radii, since the opponent could get close to the picker, but there
  //is still enough room for the handler to move up the sideline
  if(pickLoc1.getY() > opponentLoc.getY() ) {
    if(sp->field.LEFT_SIDE_LINE - pickLoc1.getY() < sidelineLaneWidth - 3*sp->general.BALL_RADIUS)
      pickAngle = normalizeAngle(pickAngle + PI);
  }
  else {
    if(pickLoc1.getY() - sp->field.RIGHT_SIDE_LINE < sidelineLaneWidth - 3*sp->general.BALL_RADIUS)
      pickAngle = normalizeAngle(pickAngle + PI);
  }


  // If the handler is closer to the destination than the opponent, pick between
  // the handler and opponent so the opponent can't chase the handler
  if( dist(handlerLoc,dest) < dist(opponentLoc,dest) ) {
    // Find pick location 1
    theta = angleBetween(opponentLoc, handlerLoc);
    // Set location to stand while picking
    pickLoc1.setX(x + COS(theta) * pickDist);
    pickLoc1.setY(y + SIN(theta) * pickDist);
  }
  // Otherwise, we have work to do... We must pick the opponent on one side so that 
  // the handler can move around him
  else {  
    // Set location to stand while picking
    pickLoc1.setX(x + COS(pickAngle) * pickDist);
    pickLoc1.setY(y + SIN(pickAngle) * pickDist);
  }


  // At this point, pickLoc1 is the new pick location, we can either go to it if we are
  // far away from the opponent, or we can stay at the old pickLoc (Which is where 
  // we are now). 

  // Pick the opponent.  If we are far away from the opponent OR far away from our pick loc,
  // set the new pick location that we calculated.  Otherwise, stay where we are.
  if(distToOpponent(robotID, opponent, *currentVisionData, *sp) > pickDist ||
     dist(robotID,pickLoc1,*currentVisionData, *sp) > BUFFER_DIST) {

    strategy->getCurrentFrame()->setMessage(robotID, "PickSkill: going to pickLoc");
    //Update the pick location
    pickLoc = pickLoc1;
    
    // Move to picking location only if far away from it (to prevent robot
    // from moving back and forth near that location)
//    if(dist(robotID,pickLoc,*currentVisionData, *sp) > BUFFER_DIST) {
      command->setXPos( pickLoc.getX() );
      command->setYPos( pickLoc.getY() );
//    }
  }
  // Otherwise we are close to the opponent, so stay at our pick location.  If he pushes
  // us, we will push back by trying to go to the pick location
  else {
    // Get our location
    pickerLoc = getLocation(robotID, *currentVisionData, *sp);  
    strategy->getCurrentFrame()->setMessage(robotID, "PickSkill: holding steady");
    //If the handler is closer to its dest than the opponent is:
    //Imagine a line from opponent to handler.  We want the picker to be
    //crossing this line AND be closer to the handler dest than the opponent.  
    //If this is not the case, we need to go to our new pick loc.
    if(!( distFromPointToLine(opponentLoc, handlerLoc, pickerLoc) < 0.5*sp->general.PLAYER_RADIUS &&
          dist(pickerLoc,dest) < dist(opponentLoc,dest) ) &&
          dist(handlerLoc,dest) < dist(opponentLoc,dest) ) {
      command->setPos(pickLoc1);
    }
    //Otherwise stay where we are
    else command->setPos(pickLoc);
  }

  
}









//===============================================================================
bool PickSkill::evaluate() {
  ///If not initialized, dont do anything!
  if(!initialized) return true;  

  return true;
}





//===============================================================================
void PickSkill::update() {
  ///If not initialized, dont do anything!
  if(!initialized) return;  
}

///Not yet implemented.  never finished
bool PickSkill::isFinished(float tolerance){
  return false;
}




//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void PickSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/PickSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  
  READ_FLOAT(BUFFER_DIST);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}



//===============================================================================
///Set opponent to pick
void PickSkill::setOpponent(RobotIndex opponentRobot) { 
  opponent = opponentRobot;
  enemyInitialized = true;
}


Pair PickSkill::getPickLoc() { return pickLoc; }
