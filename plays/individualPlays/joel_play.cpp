
#include "joel_play.h"
#include "../basic_actions.h"
#include "info/robot/robot.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "defense/defending.h"
#include "shoot/shoot.h"
#include "skillz/ThreeManBlockerSkill.h"
#include "skillz/ThreeManSideSkill.h"
#include "GUI/GUI.h"

Play JoelPlay::playNum = -1;

JoelPlay::JoelPlay(VisionModule *vm) : BasePlay(vm)
{
  loadValues();
}

void JoelPlay::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)  
{
}

ActionFunction JoelPlay::getActionFunction(Position pos)
{
  return stopRobot;
}



void JoelPlay::executePlay(VisionData* vision, RobocupStrategyData* rsd)
{
  //TOWARD GOAL TEST
  Pair robotPos=getLocation(ROBOT0,*vision,rsd->getSystemParams());
  float robotAngle=getRotation(ROBOT0,*vision,rsd->getSystemParams());
  
  float line;
  line=rsd->getSystemParams().field.THEIR_GOAL_LINE;
  GUI_Record.debuggingInfo.setDebugPoint(0,towardGoal(robotPos,robotAngle,line,rsd->getSystemParams().field.LEFT_GOAL_POST,rsd->getSystemParams().field.RIGHT_GOAL_POST),line,rsd->getSystemParams().field.SPLIT_LINE);
  line=rsd->getSystemParams().field.OUR_GOAL_LINE;
  GUI_Record.debuggingInfo.setDebugPoint(1,towardGoal(robotPos,robotAngle,line,rsd->getSystemParams().field.LEFT_GOAL_POST,rsd->getSystemParams().field.RIGHT_GOAL_POST),line,rsd->getSystemParams().field.SPLIT_LINE);


  /*
  //THREE MAN TEST
  RobotIndex blocker=rsd->getRobotByPosition(BLOCKER);
  RobotIndex defender=rsd->getRobotByPosition(DEFENDER);
  RobotIndex specialOp=rsd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  
  if(blocker != NO_ROBOT && defender != NO_ROBOT && specialOp != NO_ROBOT) 

  if(!rsd->getStrategyModule().getSkillSet(blocker)->getSkill(ThreeManBlockerSkill::skillNum)->isInitialized()){
    rsd->getStrategyModule().getSkillSet(blocker)->getSkill(ThreeManBlockerSkill::skillNum)->initialize();
  }
  rsd->getStrategyModule().getSkillSet(blocker)->getSkill(ThreeManBlockerSkill::skillNum)->run();
  if(!rsd->getStrategyModule().getSkillSet(defender)->getSkill(ThreeManSideSkill::skillNum)->isInitialized()){
    rsd->getStrategyModule().getSkillSet(defender)->getSkill(ThreeManSideSkill::skillNum)->initialize();
  }
  rsd->getStrategyModule().getSkillSet(defender)->getSkill(ThreeManSideSkill::skillNum)->run();
  if(!rsd->getStrategyModule().getSkillSet(specialOp)->getSkill(ThreeManSideSkill::skillNum)->isInitialized()){
    rsd->getStrategyModule().getSkillSet(specialOp)->getSkill(ThreeManSideSkill::skillNum)->initialize();
  }
  rsd->getStrategyModule().getSkillSet(specialOp)->getSkill(ThreeManSideSkill::skillNum)->run();
  */
  /*Pair target;
  const Parameters *p=&(rsd->getParams());
  //int shot=calcShot(ROBOT0,p->THEIR_GOAL_LINE,p->SHOOT_LANE_THRESH,
  //  p->RIGHT_GOAL_POST,p->LEFT_GOAL_POST,NO_ROBOT,*vision,*p,&target);
  int shot=calcYShot(ROBOT0,p->RIGHT_GOAL_POST,p->PASS_LANE_THRESH,
    p->THEIR_GOAL_LINE,p->OFFENSE_ZONE_LINE,NO_ROBOT,*vision,*p,&target);
  GUI_Record.debuggingInfo.setDebugPoint(ROBOT0,shot,target);*/
  
  /*float x1,x2;
  Pair robotLoc=getLocation(ROBOT0,*vision,*p);

  findShadowY(robotLoc,getLocation(0,ROBOT1,*vision),p->SPLIT_LINE, *p,x1,x2);
  
  Pair interest0(x1,p->SPLIT_LINE);
  Pair interest(x2,p->SPLIT_LINE);

  GUI_Record.debuggingInfo.setDebugPoint(0,1,interest0);
  GUI_Record.debuggingInfo.setDebugPoint(0,0,interest);
*/
  
/*  float y1, y2;
  float x=rsd->getParams().OUR_GOAL_LINE;
  findShadow(getBallLocation(*vision),getLocation(ROBOT0,*vision,rsd->getParams()),x,rsd->getParams(),y1,y2);


  if(y1<rsd->getParams().RIGHT_SIDE_LINE) y1=rsd->getParams().RIGHT_SIDE_LINE;
  if(y1>rsd->getParams().LEFT_SIDE_LINE) y1=rsd->getParams().LEFT_SIDE_LINE;
  if(y2<rsd->getParams().RIGHT_SIDE_LINE) y2=rsd->getParams().RIGHT_SIDE_LINE;
  if(y2>rsd->getParams().LEFT_SIDE_LINE) y2=rsd->getParams().LEFT_SIDE_LINE;
  GUI_Record.debuggingInfo.setDebugPoint(ROBOT0,1,x,y1);
  GUI_Record.debuggingInfo.setDebugPoint(ROBOT0,2,x,y2);
  char msg[80];
  double st=vision->getStartTime();
  double ft=vision->getFinishTime();

  sprintf(msg,"Start: %f, Finish: %f",st,ft);
  //GUI_Record.debuggingInfo.addDebugMessage(msg);
*/ 
}
