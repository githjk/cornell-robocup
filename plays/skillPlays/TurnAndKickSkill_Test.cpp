#include "TurnAndKickSkill_test.h"
#include "Skillz/TurnAndKickSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "action/action.h"
Play TurnAndKickSkillTest::playNum = -1;
//====================================
void TurnAndKickSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  Pair ballLoc;
//  RobotIndex xID; //dummy index opponent should not be given ball
  //Have robot with ball spin
//  getRobotClosestToBall(vision,rsd,ballLoc,ID,xID);
      
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);
  rsd->setMessage(ID, "Kick: Chosen to Maneuver");  


  //Get a handle on the screen skill
  kickSkill = (TurnAndKickSkill*)(skills->getSkill(TurnAndKickSkill::skillNum));

  //Initialize the skill to screen points A, B, look to C, and grace factor
  
  //Initialize the skill to:
  //-screen opponent robot 0
  //-from robot friendly robot 0
  //-and look toward the ball at all times
  //-with grace distance 0.2 meters
  center.setX(0.0f);
  center.setY(0.0f);
  kickSkill->initialize(center, KICK_SHOT);
    

  //Set the skill as active
  dribbleTimer.markStartTime();
}
//====================================
ActionFunction TurnAndKickSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void TurnAndKickSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{

  if(dribbleTimer.getElapsedTime()<3.0f){
    rsd->getDestination(ID)->setDribble(FAST_DRIBBLE);
    rsd->getDestination(ID)->setVerticalDribble(SLOW_V_DRIBBLE);
    rsd->setMessage(ID, "Kick: Dribbling");  

  }else{

    ///Always check to see if the skill is valid.  If it isn't, we would make some
    ///high-level decision about what to do.
    if( kickSkill->isValid() ) 
    {
      ///run the skill
      kickSkill->run();
    }else rsd->setMessage(ID, "Kick: Does not have ball");  
  }

  
}
//====================================
