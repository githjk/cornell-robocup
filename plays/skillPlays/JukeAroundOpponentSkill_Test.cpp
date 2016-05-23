#include "JukeAroundOpponentSkill_Test.h"
#include "Skillz/JukeAroundOpponentSkill.h"
#include "info/robot/robot.h"
#include "math/dist.h"
#include "GUI/GUI.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"

Play JukeAroundOpponentSkillTest::playNum = -1;
//====================================
void JukeAroundOpponentSkillTest::initializePlay(const VisionData& vision, 
                                           RobocupStrategyData* rsd)
{
  //Have robot0 juke
   ID = ROBOT0;
   
  //Have him juke towards this goal desitnation
   Pair goalDest = getLocation((RobotIndex)1, vision, rsd->getSystemParams());

  //have robot juke to this destin
  //Get a handle on the skill set.  Always useful to have...
    SkillSet* skills = rsd->getStrategyModule().getSkillSet(ID);

  //Get a handle on the screen skill
  jukeAroundOpponentSkill = (JukeAroundOpponentSkill*)(skills->getSkill(JukeAroundOpponentSkill::skillNum));

  jukeAroundOpponentSkill->initialize(goalDest);
}
//====================================
ActionFunction JukeAroundOpponentSkillTest::getActionFunction(Position pos)
{
  return stopRobot;
}
//====================================
void JukeAroundOpponentSkillTest::executePlay(VisionData* vision, 
                                        RobocupStrategyData* rsd)
{
  if(skills->getSkill(JukeAroundOpponentSkill::skillNum)->isValid())
    skills->getSkill(JukeAroundOpponentSkill::skillNum)->run();
}
//====================================
