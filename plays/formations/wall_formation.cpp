#include "wall_formation.h"

Play WallFormation::playNum = -1;

//==============================================
void WallFormation::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

};
//==============================================
void WallFormation::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  //==============================================
  //BLOCKER/GOALIE
  index = rsd->getRobotByPosition(BLOCKER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Wall Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.FIELD_LENGTH / 
                                       6.0f * 
                                       BLOCKER_X,
                                       rsd->getSystemParams().field.RIGHT_SIDE_LINE +
                                       rsd->getSystemParams().general.PLAYER_RADIUS * BLOCKER_Y);
    rsd->getDestination(index)->setRotation(BLOCKER_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
  }
  //==============================================
  //DEFENDER/ACTIVE_DEFENDER
  index = rsd->getRobotByPosition(DEFENDER);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Wall Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.FIELD_LENGTH / 
                                       6.0f * 
                                       DEFENDER_X,
                                       rsd->getSystemParams().field.RIGHT_SIDE_LINE +
                                       rsd->getSystemParams().general.PLAYER_RADIUS * DEFENDER_Y);
    rsd->getDestination(index)->setRotation(DEFENDER_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
  }
  //==============================================
  //AGGRESSOR/MIDFIELDER
  index = rsd->getRobotByPosition(AGGRESSOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Wall Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.FIELD_LENGTH / 
                                       6.0f * 
                                       AGGRESSOR_X,
                                       rsd->getSystemParams().field.RIGHT_SIDE_LINE +
                                       rsd->getSystemParams().general.PLAYER_RADIUS * AGGRESSOR_Y);
    rsd->getDestination(index)->setRotation(AGGRESSOR_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
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
    rsd->setMessage(index, "Wall Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.FIELD_LENGTH / 
                                       6.0f * 
                                       SPECIAL_OP_X,
                                       rsd->getSystemParams().field.RIGHT_SIDE_LINE +
                                       rsd->getSystemParams().general.PLAYER_RADIUS * SPECIAL_OP_Y);
    rsd->getDestination(index)->setRotation(SPECIAL_OP_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
  }
  //==============================================
  //CREATOR/PASS_FORWARD
  index = rsd->getRobotByPosition(CREATOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Wall Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.FIELD_LENGTH / 
                                       6.0f * 
                                       CREATOR_X,
                                       rsd->getSystemParams().field.RIGHT_SIDE_LINE +
                                       rsd->getSystemParams().general.PLAYER_RADIUS * CREATOR_Y);
    rsd->getDestination(index)->setRotation(CREATOR_ROTATION);
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
  }
  //==============================================
}
//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void WallFormation::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/formationPlays/WallFormationParams.txt", ios::nocreate);  
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
