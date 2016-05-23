#include "SpinAroundBallSkill_Test.h"
#include "Skillz/SpinSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
#include "action/action.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
Play SpinAroundBallSkillTest::playNum = -1;
//====================================
void SpinAroundBallSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  Pair ballLoc;
//  RobotIndex xID; //dummy index opponent should not be given ball
  //Have robot with ball spin
//  getRobotClosestToBall(vision,rsd,ballLoc,ID,xID);
      
  ID = ROBOT0;

  //Get a handle on the skill set.  Always useful to have...
    skills = rsd->getStrategyModule().getSkillSet(ID);
//  rsd->setMessage(ID, "Spin: Chosen to Spin");  


  //Get a handle on the screen skill
  spinSkill = (SpinAroundBallSkill*)(skills->getSkill(SpinAroundBallSkill::skillNum));
  //rotate=(RotateAroundBallSkill *)(skills->getSkill(RotateAroundBallSkill::skillNum));


  //Initialize the skill to screen points A, B, look to C, and grace factor
  
  //Initialize the skill to:
  //-screen opponent robot 0
  //-from robot friendly robot 0
  //-and look toward the ball at all times
  //-with grace distance 0.2 meters
  center.setX(rsd->getSystemParams().field.THEIR_GOAL_LINE);
  center.setY(rsd->getSystemParams().field.SPLIT_LINE);
  spinSkill->initialize(center);
  //rotate->initialize(center);
    

  //Set the skill as active
  dribbleTimer.markStartTime();
}
//====================================
ActionFunction SpinAroundBallSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void SpinAroundBallSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{

  if(dribbleTimer.getElapsedTime()<3.0f)
  {
    rsd->getDestination(ID)->setDribble(FAST_DRIBBLE);
    rsd->getDestination(ID)->setVerticalDribble(SLOW_V_DRIBBLE);
   // rsd->setMessage(ID, "Spin: Dribbling");  

  }
  else
  {
      ///Always check to see if the skill is valid.  If it isn't, we would make some
      ///high-level decision about what to do.
      if( skills->getSkill(SpinAroundBallSkill::skillNum)->isValid() ) 
      {
        ///Set the message of screening robot
      //    rsd->setMessage(ID, "Spin: Spinning around the ball");  
    
          //cout<<SpinAroundBallSkillNum<<endl;
          //cout<<SimpleScreenSkillNum<<endl;
          //cout<<spinSkill<<endl;
      
        ///run the skill

        skills->getSkill(SpinAroundBallSkill::skillNum)->run();
      }
     // else rsd->setMessage(ID, "Spin: Does not have ball");  
  }

  
}
//====================================
