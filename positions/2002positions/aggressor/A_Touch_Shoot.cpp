/***********************************************************************************/
//	HEADER FILES
#include "Aggressor.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/ReboundOffenseSkill.h"
#include "Skillz/SimpleKickSkill.h"
#include "Skillz/ReceivePassSkill.h"
#include "Skillz/FacilatePassSkill.h"
#include "common/util/math/angle.h"
#include "common/util/pair/pair.h"
/***********************************************************************************/
using namespace ScratchVars;
//----------------------------------------------------------------------------
void Aggressor::touchShoot(RobotIndex ID,
                            BasePlay* play,
                            const VisionData& field,
                            RobocupStrategyData* sd)
{
  RobotIndex receiver;
  if(sd->getStrategyModule().playbook->getPlay(sd->getCurrentPlay())->getPlayState() == PASS_TO_SPECIAL_OP){
    receiver=sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(ID, "AGGRESSOR touch Shoot - SO");
  }else{
    receiver=sd->getRobotByPosition(CREATOR);
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(ID, "AGGRESSOR touch Shoot - C");
  }

  //===================
  //===================
  //Handle the Aggressor
  //===================
  //===================


  //-------------------
  //get a handle on skillset for this robot
  //-------------------
  SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

  //-------------------
  //get a handle on touch shoot aggressor skill
  //-------------------
  SimpleKickSkill* skillHandle = 
  (SimpleKickSkill*)skills->getSkill(SimpleKickSkill::skillNum);

  //-------------------
  //initialize skill if it is not initialized
  //-------------------
 if(!skillHandle->isInitialized())
 {
   
   //if friend found, and distance too short, then kick pass
   if(
       receiver != NO_ROBOT &&
       getLocation(ID,field,sd->getSystemParams()).distanceTo(getLocation(receiver, field, sd->getSystemParams())) < 
       sd->getSystemParams().strategy2002.PASS_TO_SHOT_DISTANCE
     )
   {
     skillHandle->initialize(KICK_PASS);
   }
   //else shot is long, or we never figured out who we are passing to, so kick is hard!
   else
   {
     skillHandle->initialize(KICK_SHOT);
   }

 }
 
 //-------------------
 //run skill
 //-------------------
 skillHandle->run();

  /*
  //===================
  //===================
  //Handle the Special Op Aggressor if he exists
  //===================
  //===================
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  if(SpecialOpID != NO_ROBOT)
  {
    //-------------------
    //Display message
    //-------------------
    sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR touch shoot");

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

    //-------------------
    //get a handle on touch shoot special Op skill
    //-------------------
    ReboundOffenseSkill* specialOpSkillHandle = 
    (ReboundOffenseSkill*)skills->getSkill(ReboundOffenseSkill::skillNum);

    //-------------------
    //initialize skill if it is not initialized
    //-------------------
    if(!specialOpSkillHandle->isInitialized())
  	  specialOpSkillHandle->initialize();

    //-------------------
    //run skill
    //-------------------
    specialOpSkillHandle->run();
  }
  */
  //===================
  //===================
  //Handle the Special Op Aggressor if he exists
  //===================
  //===================
  RobotIndex SpecialOpID = sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  if(SpecialOpID != NO_ROBOT)
  {
    if(sd->getStrategyModule().playbook->getPlay(sd->getCurrentPlay())->getPlayState() == PASS_TO_SPECIAL_OP){
      //-------------------
      //Display message
      //-------------------
      sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR Touch Shoot Receive");

      //-------------------
      //get a handle on skillset for this robot
      //-------------------
      SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

      //-------------------
      //get a handle on loose defense special Op skill
      //-------------------
      ReceivePassSkill* specialOpSkillHandle = 
      (ReceivePassSkill*)skills->getSkill(ReceivePassSkill::skillNum);

      //-------------------
      //initialize skill if it is not initialized
      //-------------------
      RobotIndex aggressorID = sd->getRobotByPosition(AGGRESSOR);
      if(!specialOpSkillHandle->isInitialized())
      {
        //This call should be changed in the future, but this makes it so we can run and test.
        if(SpecialOpID != NO_ROBOT  && aggressorID != NO_ROBOT)
        {
          //calculate a better place than the pass dest.
          Pair passDest=sd->getPassDest(SpecialOpID);
          rotateAboutPoint(passDest,getLocation(aggressorID,field,sd->getSystemParams()),
            angleDifference(getRotation(aggressorID,field,sd->getSystemParams()),
                            angleBetween(aggressorID,passDest,field,sd->getSystemParams())),
            passDest);

      
            specialOpSkillHandle->initialize(.5f,passDest,true,true);
        }
    
      }

      //-------------------
      //run skill
      //-------------------
      specialOpSkillHandle->run();

    }else{
      //-------------------
      //Display message
      //-------------------
      sd->setMessage(SpecialOpID, "SPECIAL OP AGGRESSOR Touch Shoot");

      //-------------------
      //get a handle on skillset for this robot
      //-------------------
      SkillSet* skills = sd->getStrategyModule().getSkillSet(SpecialOpID);

      //-------------------
      //get a handle on loose defense special Op skill
      //-------------------
      FacilatePassSkill* specialOpSkillHandle = 
      (FacilatePassSkill*)skills->getSkill(FacilatePassSkill::skillNum);

      //-------------------
      //initialize skill if it is not initialized
      //-------------------
      if(!specialOpSkillHandle->isInitialized())
  	    specialOpSkillHandle->initialize();

      //-------------------
      //run skill
      //-------------------
      specialOpSkillHandle->run();
    }
  }

}
//----------------------------------------------------------------------------