#include "ManeuverWithBallSkill_Test.h"
#include "Skillz/ManeuverSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
#include "action/action.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play ManeuverWithBallSkillTest::playNum = -1;
//====================================
void ManeuverWithBallSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  Pair ballLoc;
//  RobotIndex xID; //dummy index opponent should not be given ball
  //Have robot with ball spin
//  getRobotClosestToBall(vision,rsd,ballLoc,ID,xID);
      
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(ID);
  rsd->setMessage(ID, "Maneuver: Chosen to Maneuver");  


  //Get a handle on the screen skill
  maneuverSkill = (ManeuverWithBallSkill*)(skills->getSkill(ManeuverWithBallSkill::skillNum));

  //Initialize the skill to screen points A, B, look to C, and grace factor
  
  //Initialize the skill to:
  //-screen opponent robot 0
  //-from robot friendly robot 0
  //-and look toward the ball at all times
  //-with grace distance 0.2 meters
  center.setX(0.0f);
  center.setY(0.0f);
  maneuverSkill->initialize(center, Pair(1.0f, .5f));
    

  //Set the skill as active
  dribbleTimer.markStartTime();
}
//====================================
ActionFunction ManeuverWithBallSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void ManeuverWithBallSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{

  if(dribbleTimer.getElapsedTime()<3.0f){
    rsd->getDestination(ID)->setDribble(FAST_DRIBBLE);
    rsd->getDestination(ID)->setVerticalDribble(SLOW_V_DRIBBLE);
    rsd->setMessage(ID, "Spin: Dribbling");  

  }else{

    ///Always check to see if the skill is valid.  If it isn't, we would make some
    ///high-level decision about what to do.
      ///Set the message of screening robot
        rsd->setMessage(ID, "Maneuver: going...");  
      
      ///run the skill
      maneuverSkill->run();
  }

  
}
//====================================
