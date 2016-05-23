#include "DribblePointSkill_Test.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/DribblePointSkill.h"
#include "GUI/GUI.h"
#include "common/util/info/ball/ball.h"

Play DribblePointSkillTest::playNum = -1;

//====================================
void DribblePointSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{     
  //robot we'll have dribbling
  DribblePointSkillTest::DRIBBLE_ROBOT = ROBOT0;
  
  //Get a handle on the skill set.  Always useful to have...
  skills = rsd->getStrategyModule().getSkillSet(DRIBBLE_ROBOT);

  //Get a handle on the screen skill
  dribblePointSkill = (DribblePointSkill*)(skills->getSkill(DribblePointSkill::skillNum));

  
  Pair goalDest(rsd->getSystemParams().field.THEIR_GOAL_LINE, 0.0f);

  //actually, follow the ball
  goalDest.set(rsd->getSystemParams().field.THEIR_GOAL_LINE,
	           getRobocupBall(vision).getPos().getY());
  
  
  int numIgnoreIDs = 0;
  RobotIndex* ignoreIDs = NULL;
  
  /*
  int numIgnoreIDs = 2;
  RobotIndex* ignoreIDs = new RobotIndex[numIgnoreIDs];
  ignoreIDs[0] = ROBOT2;
  ignoreIDs[1] = ROBOT4;
  */

  //Initialize the skill
  dribblePointSkill->initialize(goalDest,
                                ignoreIDs,
                                numIgnoreIDs);
  delete[] ignoreIDs;

}
//====================================
ActionFunction DribblePointSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void DribblePointSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  ///Always check to see if the skill is valid.  If it isn't, we would make some
  ///high-level decision about what to do.
  if( skills->getSkill(DribblePointSkill::skillNum)->isValid() ) 
  {
    ///run the skill
    skills->getSkill(DribblePointSkill::skillNum)->run();
  } 
  else
  {
    rsd->setMessage(DRIBBLE_ROBOT, "Dribble Point Not Valid...");  
  }
}
//====================================
