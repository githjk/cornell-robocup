#include "penalty_shot_us.h"

Play PenaltyShotUs::playNum = -1;

//==============================================
void PenaltyShotUs::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

};

//==============================================
void PenaltyShotUs::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  //==============================================
  BALL_X = rsd->getSystemParams().field.THEIR_GOAL_LINE - 
           rsd->getSystemParams().field.PENALTY_KICK_X_OFFSET;

  BALL_Y = rsd->getSystemParams().field.SPLIT_LINE;
  //==============================================
  //BLOCKER
  index = rsd->getRobotByPosition(BLOCKER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Penalty Shot Us Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + BLOCKER_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + BLOCKER_Y);
    rsd->getDestination(index)->setRotation(angleBetween
           (getLocation(index, *vision, rsd->getSystemParams()),
            Pair(BALL_X,
                 BALL_Y)));
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
  }
  //==============================================
  //DEFENDER
  index = rsd->getRobotByPosition(DEFENDER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Penalty Shot Us Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + DEFENDER_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + DEFENDER_Y);
    rsd->getDestination(index)->setRotation(angleBetween
           (getLocation(index, *vision, rsd->getSystemParams()),
            Pair(BALL_X,
                 BALL_Y)));
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
  }
  //==============================================
  //AGGRESSOR
  index = rsd->getRobotByPosition(AGGRESSOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Penalty Shot Us Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.THEIR_GOAL_LINE -
                                       rsd->getSystemParams().field.PENALTY_KICK_X_OFFSET +
                                       AGGRESSOR_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + AGGRESSOR_Y);
    rsd->getDestination(index)->setRotation(AGGRESSOR_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setDribble(SLOW_DRIBBLE);
    rsd->getDestination(index)->setVerticalDribble(SLOW_V_DRIBBLE);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
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
    rsd->setMessage(index, "Penalty Shot Us Formation");
    rsd->getDestination(index)->setPos(BALL_X + SPECIAL_OP_X - rsd->getSystemParams().general.PLAYER_RADIUS,
                                       rsd->getSystemParams().field.SPLIT_LINE + SPECIAL_OP_Y);
    rsd->getDestination(index)->setRotation(angleBetween
           (getLocation(index, *vision, rsd->getSystemParams()),
            Pair(BALL_X,
                 BALL_Y)));
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
  }
  //==============================================
  //CREATOR
  index = rsd->getRobotByPosition(CREATOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Penalty Shot Us Formation");
    rsd->getDestination(index)->setPos(BALL_X + 
                                       CREATOR_X - 
                                       rsd->getSystemParams().general.PLAYER_RADIUS,
                                       rsd->getSystemParams().field.SPLIT_LINE + CREATOR_Y);
    rsd->getDestination(index)->setRotation(angleBetween
           (getLocation(index, *vision, rsd->getSystemParams()),
            Pair(BALL_X,
                 BALL_Y)));
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
  }
  //==============================================
}
//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void PenaltyShotUs::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/formationPlays/PenaltyShotUsFormationParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(BLOCKER_X);
  READ_FLOAT(BLOCKER_Y);
  READ_FLOAT(BLOCKER_ROTATION);
  READ_FLOAT(DEFENDER_X);
  READ_FLOAT(DEFENDER_Y);
  READ_FLOAT(DEFENDER_ROTATION);
  READ_FLOAT(AGGRESSOR_X);
  READ_FLOAT(AGGRESSOR_Y);
  READ_FLOAT(AGGRESSOR_ROTATION);
  READ_FLOAT(CREATOR_X);
  READ_FLOAT(CREATOR_Y);
  READ_FLOAT(CREATOR_ROTATION);
  READ_FLOAT(SPECIAL_OP_X);
  READ_FLOAT(SPECIAL_OP_Y);
  READ_FLOAT(SPECIAL_OP_ROTATION);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//====================================
