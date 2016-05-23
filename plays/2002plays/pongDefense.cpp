#include "pongDefense.h"
#include "common/util/info/ball/ball.h"

Play PongDefense::playNum = -1;

//==============================================
void PongDefense::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

};
//==============================================
void PongDefense::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  Pair ballLoc = getBallLocation(*vision);
  Pair dest;
  //==============================================
  //BLOCKER
  index = rsd->getRobotByPosition(BLOCKER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Pong Defense");
    dest.setY(ballLoc.getY());
    dest.setX(rsd->getSystemParams().field.OUR_GOAL_LINE +
              rsd->getSystemParams().general.PLAYER_RADIUS +
              rsd->getSystemParams().general.BALL_RADIUS);

    //if ball past goal post, then stop at post
    if(dest.getY() > rsd->getSystemParams().field.LEFT_GOAL_POST)
      dest.setY(rsd->getSystemParams().field.LEFT_GOAL_POST);
    if(dest.getY() < rsd->getSystemParams().field.RIGHT_GOAL_POST)
      dest.setY(rsd->getSystemParams().field.RIGHT_GOAL_POST);

    rsd->getDestination(index)->setPos(dest);   
    rsd->getDestination(index)->setRotation(0.0f);
    rsd->getDestination(index)->setControl(OMNI_GOALIE);
    rsd->getDestination(index)->setSpeed(GOALIE_SPEED);
  }
  //==============================================
  //count how many people you have
  int numPlayers = 0;
  if(rsd->getRobotByPosition(AGGRESSOR) != NO_ROBOT)
    numPlayers++;
  if(rsd->getRobotByPosition(CREATOR) != NO_ROBOT)
    numPlayers++;
  if(rsd->getRobotByPosition(DEFENDER) != NO_ROBOT)
    numPlayers++;
  if(rsd->getRobotByPosition(SPECIAL_OP_DEFENDER) != NO_ROBOT)
    numPlayers++;
  if(rsd->getRobotByPosition(SPECIAL_OP_AGGRESSOR) != NO_ROBOT)
    numPlayers++;
  if(rsd->getRobotByPosition(SPECIAL_OP_CREATOR) != NO_ROBOT)
    numPlayers++;

  //center of pong paddle
  dest.setY(ballLoc.getY());
  dest.setX(rsd->getSystemParams().field.OUR_GOALIE_BOX + 
            rsd->getSystemParams().general.PLAYER_RADIUS +
            2.0f*rsd->getSystemParams().general.BALL_RADIUS);
  
  //push left based on number of robots in paddle
  float halfPaddleSize = numPlayers*rsd->getSystemParams().general.PLAYER_RADIUS +
                         2*(numPlayers-1)*rsd->getSystemParams().general.BALL_RADIUS;
  dest.setY(
             dest.getY() + 
             halfPaddleSize - 
             rsd->getSystemParams().general.PLAYER_RADIUS
           );

  //bound left side
  if(dest.getY() > rsd->getSystemParams().field.LEFT_SIDE_LINE - 
                   rsd->getSystemParams().general.PLAYER_RADIUS)
  {
    dest.setY(rsd->getSystemParams().field.LEFT_SIDE_LINE -
              rsd->getSystemParams().general.PLAYER_RADIUS);
  }
  
  //bound right side
  if(
      (
        dest.getY() - 
        2*halfPaddleSize + 
        rsd->getSystemParams().general.PLAYER_RADIUS) <
      (
        rsd->getSystemParams().field.RIGHT_SIDE_LINE + 
        rsd->getSystemParams().general.PLAYER_RADIUS
      )
    )
  {
    dest.setY(rsd->getSystemParams().field.RIGHT_SIDE_LINE +
              2*halfPaddleSize - 
              rsd->getSystemParams().general.PLAYER_RADIUS);
  }
  //=====================================
  //set destinations
  index = rsd->getRobotByPosition(CREATOR);
  if (index != NO_ROBOT) 
  {
    rsd->getDestination(index)->setPos(dest);   
    rsd->getDestination(index)->setRotation(0.0f);
    dest.setY(dest.getY() - 2*rsd->getSystemParams().general.PLAYER_RADIUS);
    rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
  }
  index = rsd->getRobotByPosition(DEFENDER);
  if (index != NO_ROBOT) 
  {
    rsd->getDestination(index)->setPos(dest);   
    rsd->getDestination(index)->setRotation(0.0f);
    dest.setY(dest.getY() - 2*rsd->getSystemParams().general.PLAYER_RADIUS);
    rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
  }
  index = rsd->getRobotByPosition(AGGRESSOR);
  if (index != NO_ROBOT) 
  {
    rsd->getDestination(index)->setPos(dest);   
    rsd->getDestination(index)->setRotation(0.0f);
    dest.setY(dest.getY() - 2*rsd->getSystemParams().general.PLAYER_RADIUS);
    rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
  }
  index = rsd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  if (index != NO_ROBOT) 
  {
    rsd->getDestination(index)->setPos(dest);   
    rsd->getDestination(index)->setRotation(0.0f);
    dest.setY(dest.getY() - 2*rsd->getSystemParams().general.PLAYER_RADIUS);
    rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
  }
  index = rsd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  if (index != NO_ROBOT) 
  {
    rsd->getDestination(index)->setPos(dest);   
    rsd->getDestination(index)->setRotation(0.0f);
    dest.setY(dest.getY() - 2*rsd->getSystemParams().general.PLAYER_RADIUS);
    rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
  }
  index = rsd->getRobotByPosition(SPECIAL_OP_CREATOR);
  if (index != NO_ROBOT) 
  {
    rsd->getDestination(index)->setPos(dest);   
    rsd->getDestination(index)->setRotation(0.0f);
    dest.setY(dest.getY() - 2*rsd->getSystemParams().general.PLAYER_RADIUS);
    rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
  }
  //=====================================
/*
  index = rsd->getRobotByPosition(BLOCKER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Pong Defense");
    dest.setY(ballLoc.getY());
    dest.setX(rsd->getSystemParams().field.OUR_GOAL_LINE +
              rsd->getSystemParams().general.PLAYER_RADIUS +
              rsd->getSystemParams().general.BALL_RADIUS);

    //if ball past goal post, then stop at post
    if(dest.getY() > rsd->getSystemParams().field.LEFT_GOAL_POST)
      dest.setY(rsd->getSystemParams().field.LEFT_GOAL_POST);
    if(dest.getY() < rsd->getSystemParams().field.RIGHT_GOAL_POST)
      dest.setY(rsd->getSystemParams().field.RIGHT_GOAL_POST);

    rsd->getDestination(index)->setPos(dest);   
    rsd->getDestination(index)->setRotation(0.0f);
    rsd->getDestination(index)->setControl(OMNI_GOALIE);
    rsd->getDestination(index)->setSpeed(GOALIE_SPEED);
*/






  /*
  //DEFENDER
  index = rsd->getRobotByPosition(DEFENDER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Dribble Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + DEFENDER_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + DEFENDER_Y);
    rsd->getDestination(index)->setRotation(DEFENDER_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    rsd->getDestination(index)->setDribble(FAST_DRIBBLE);
    rsd->getDestination(index)->setVerticalDribble(FAST_V_DRIBBLE);
  }
  //==============================================
  //AGGRESSOR
  index = rsd->getRobotByPosition(AGGRESSOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Dribble Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + AGGRESSOR_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + AGGRESSOR_Y);
    rsd->getDestination(index)->setRotation(AGGRESSOR_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setDribble(FAST_DRIBBLE);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    rsd->getDestination(index)->setVerticalDribble(FAST_V_DRIBBLE);
  }
  //==============================================
  //SPECIAL_OP
  index = rsd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  if (index == NO_ROBOT) 
    index = rsd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);	
  if (index == NO_ROBOT) 
    index = rsd->getRobotByPosition(SPECIAL_OP_CREATOR);	
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Dribble Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + SPECIAL_OP_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + SPECIAL_OP_Y);
    rsd->getDestination(index)->setRotation(SPECIAL_OP_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setDribble(FAST_DRIBBLE);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    rsd->getDestination(index)->setVerticalDribble(FAST_V_DRIBBLE);
  }
  //==============================================
  //CREATOR
  index = rsd->getRobotByPosition(CREATOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Dribble Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + CREATOR_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + CREATOR_Y);
    rsd->getDestination(index)->setRotation(CREATOR_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setDribble(FAST_DRIBBLE);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    rsd->getDestination(index)->setVerticalDribble(FAST_V_DRIBBLE);
  }
  //==============================================
  */
}
//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void PongDefense::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/2002plays/PongDefenseParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//====================================
