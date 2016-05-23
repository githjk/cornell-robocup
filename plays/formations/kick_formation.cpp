#include "kick_formation.h"

Play KickFormation::playNum = -1;

//==============================================
void KickFormation::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

};
//==============================================
void KickFormation::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{     //==============================================
  //BLOCKER/GOALIE
  index = rsd->getRobotByPosition(BLOCKER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Kick Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + BLOCKER_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + BLOCKER_Y);
    rsd->getDestination(index)->setRotation(BLOCKER_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    rsd->getDestination(index)->setKick(KICK_SHOT);
  }
  //==============================================
  //DEFENDER/ACTIVE_DEFENDER
  index = rsd->getRobotByPosition(DEFENDER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Kick Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + DEFENDER_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + DEFENDER_Y);
    rsd->getDestination(index)->setRotation(DEFENDER_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setKick(KICK_SHOT);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    //rsd->getDestination(index)->setDribble(FAST_DRIBBLE);
  }
  //==============================================
  //AGGRESSOR/MIDFIELDER
  index = rsd->getRobotByPosition(AGGRESSOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Kick Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + AGGRESSOR_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + AGGRESSOR_Y);
    rsd->getDestination(index)->setRotation(AGGRESSOR_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    rsd->getDestination(index)->setKick(KICK_SHOT);
  }
  //==============================================
  //SPECIAL_OP/MAIN_FORWARD
  index = rsd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  if (index == NO_ROBOT) 
    index = rsd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);	
  if (index == NO_ROBOT) 
    index = rsd->getRobotByPosition(SPECIAL_OP_CREATOR);	
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Kick Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + SPECIAL_OP_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + SPECIAL_OP_Y);
    rsd->getDestination(index)->setRotation(SPECIAL_OP_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    rsd->getDestination(index)->setKick(KICK_SHOT);
  }
  //==============================================
  //CREATOR/PASS_FORWARD
  index = rsd->getRobotByPosition(CREATOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Kick Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.HALF_LINE + CREATOR_X,
                                       rsd->getSystemParams().field.SPLIT_LINE + CREATOR_Y);
    rsd->getDestination(index)->setRotation(CREATOR_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
    rsd->getDestination(index)->setKick(KICK_SHOT);
  }
  //==============================================
}
//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void KickFormation::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/formationPlays/KickFormationParams.txt", ios::nocreate);  
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
