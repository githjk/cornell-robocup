#include "kickoff_them.h"
#include "Skillz/TandemBlockerSkill.h"
#include "Skillz/TandemDefenderSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"


Play KickoffThem::playNum = -1;

//==============================================
void KickoffThem::initializePlay(const VisionData& vision, RobocupStrategyData* rsd)
{

}
//==============================================
void KickoffThem::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  //==============================================
  float graceOffset = 0.0f;
  index = rsd->getRobotByPosition(AGGRESSOR);
  if(index != NO_ROBOT)
  {
    Pair dest(rsd->getSystemParams().field.CENTER_X + AGGRESSOR_X,
              rsd->getSystemParams().field.CENTER_Y + AGGRESSOR_Y);
    Pair aLoc = getLocation(index, *vision, rsd->getSystemParams());
  
    if(dest.distanceTo(aLoc) > rsd->getSystemParams().general.PLAYER_RADIUS)
    {
      graceOffset = 2.0f*rsd->getSystemParams().general.PLAYER_RADIUS;
    }
  }
  //==============================================
  //BLOCKER
  index = rsd->getRobotByPosition(BLOCKER);
  if (index != NO_ROBOT) 
  {
      /** New Formation */
      //-------------------
      //get a handle on skillset for this robot
      //-------------------
        SkillSet* skills = rsd->getStrategyModule().getSkillSet(index);

      //-------------------
      //get a handle on upfield defense blocker skill
      //-------------------
      TandemBlockerSkill* skillHandle = 
      (TandemBlockerSkill*)skills->getSkill(TandemBlockerSkill::skillNum);

      //-------------------
      //initialize skill if it has not been initialized.
      //-------------------
      if(!skillHandle->isInitialized())
      {
        skillHandle->initialize(Pair(rsd->getSystemParams().field.CENTER_X,
			                         rsd->getSystemParams().field.CENTER_Y));
      }
  
      //-------------------
      //run Blocker upfield Defense skill
      //-------------------
      skillHandle->run();   
      rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
      rsd->getDestination(index)->setKick(NO_KICK);
  }
  //==============================================
  //DEFENDER
  index = rsd->getRobotByPosition(DEFENDER);
  if (index != NO_ROBOT) 
  {
    
      /** New Formation */
      //-------------------
      //get a handle on skillset for this robot
      //-------------------
        SkillSet* skills = rsd->getStrategyModule().getSkillSet(index);

      //-------------------
      //get a handle on upfield defense blocker skill
      //-------------------
      TandemDefenderSkill* skillHandle = 
      (TandemDefenderSkill*)skills->getSkill(TandemDefenderSkill::skillNum);

      //-------------------
      //initialize skill if it has not been initialized.
      //-------------------
      if(!skillHandle->isInitialized())
      {
        skillHandle->initialize(Pair(rsd->getSystemParams().field.CENTER_X,
			                         rsd->getSystemParams().field.CENTER_Y));
      }
  
      //-------------------
      //run Blocker upfield Defense skill
      //-------------------
      skillHandle->run();
      rsd->getDestination(index)->setSpeed(FORMATION_SPEED);
      rsd->getDestination(index)->setKick(NO_KICK);
  }
  //==============================================
  //AGGRESSOR
  index = rsd->getRobotByPosition(AGGRESSOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Kickoff Them Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.CENTER_X + AGGRESSOR_X,
                                       rsd->getSystemParams().field.CENTER_Y + AGGRESSOR_Y);
    rsd->getDestination(index)->setRotation(angleBetween(index, 
                                                         Pair(rsd->getSystemParams().field.CENTER_X,
                                                              rsd->getSystemParams().field.CENTER_Y), 
                                                         *vision, 
                                                         rsd->getSystemParams()) );
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);

    if(getLocation(index, *vision, rsd->getSystemParams()).distanceTo(rsd->getDestination(index)->getPos()) < 
       rsd->getSystemParams().general.PLAYER_RADIUS)
    {
      rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
    }
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
    rsd->setMessage(index, "Kickoff Them Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.CENTER_X + 
                                       SPECIAL_OP_X,
                                       rsd->getSystemParams().field.CENTER_Y + 
                                       SPECIAL_OP_Y + 
                                       graceOffset);
    rsd->getDestination(index)->setRotation(angleBetween(index, 
                                                         Pair(rsd->getSystemParams().field.CENTER_X,
                                                              rsd->getSystemParams().field.CENTER_Y), 
                                                         *vision, 
                                                         rsd->getSystemParams()) );
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);

    if(getLocation(index, *vision, rsd->getSystemParams()).distanceTo(rsd->getDestination(index)->getPos()) < 
       rsd->getSystemParams().general.PLAYER_RADIUS)
    {
      rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
    }
  
  }
  //==============================================
  //CREATOR
  index = rsd->getRobotByPosition(CREATOR);
  if (index != NO_ROBOT) 
  {
    rsd->setMessage(index, "Kickoff Them Formation");
    rsd->setMessage(index, "Kickoff Them Formation");
    rsd->getDestination(index)->setPos(rsd->getSystemParams().field.CENTER_X + 
                                       CREATOR_X,
                                       rsd->getSystemParams().field.CENTER_Y + 
                                       CREATOR_Y - 
                                       graceOffset);
    rsd->getDestination(index)->setRotation(angleBetween(index, 
                                                         Pair(rsd->getSystemParams().field.CENTER_X,
                                                              rsd->getSystemParams().field.CENTER_Y), 
                                                         *vision, 
                                                         rsd->getSystemParams()) );
    rsd->getDestination(index)->setControl(OMNI_NORMAL_ENTERBOX);
    rsd->getDestination(index)->setSpeed(FORMATION_SPEED);

    if(getLocation(index, *vision, rsd->getSystemParams()).distanceTo(rsd->getDestination(index)->getPos()) < 
       rsd->getSystemParams().general.PLAYER_RADIUS)
    {
      rsd->getDestination(index)->setControl(OMNI_NO_AVOID);
    }

  }
  //==============================================
}
//====================================
///Reads in the values of the parameters from the plays's parameter text file.
void KickoffThem::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/formationPlays/KickoffThemFormationParams.txt", ios::nocreate);  
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
