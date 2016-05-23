//----------------------------------------------------------------------------
/***********************************************************************************/
//	HEADER FILES
#include "Creator.h"                  
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "Skillz/ReceivePassSkill.h"
#include "Skillz/FacilatePassSkill.h"
#include "Skillz/OppositeReboundOffenseSkill.h"
#include "common/util/pair/pair.h"
#include "common/util/math/angle.h"
/***********************************************************************************/
using namespace ScratchVars;

//----------------------------------------------------------------------------
void Creator::touchShoot(RobotIndex ID,
                             BasePlay* play,
                             const VisionData& field,
                             RobocupStrategyData* sd)
{
  if(sd->getStrategyModule().playbook->getPlay(sd->getCurrentPlay())->getPlayState() == PASS_TO_CREATOR){
    //Display Message
    sd->setMessage(ID, "CREATOR touch shoot");   

    //-------------------
    //get a handle on skillset for this robot
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

    //-------------------
    //get a handle on touch shoot creator skill
    //-------------------
    ReceivePassSkill* skillHandle = 
    (ReceivePassSkill*)skills->getSkill(ReceivePassSkill::skillNum);

    //-------------------
    //initialize skill if it has not been initialized.
    //-------------------
    RobotIndex aggressorID = sd->getRobotByPosition(AGGRESSOR);
    if(!skillHandle->isInitialized())
    {
      //This call should be changed in the future, but this makes it so we can run and test.
      if(sd->getRobotByPosition(CREATOR) != NO_ROBOT  && aggressorID != NO_ROBOT)
      {
        //calculate a better place than the pass dest.
        Pair passDest=sd->getPassDest(sd->getRobotByPosition(CREATOR));
        rotateAboutPoint(passDest,getLocation(aggressorID,field,sd->getSystemParams()),
          angleDifference(getRotation(aggressorID,field,sd->getSystemParams()),
                          angleBetween(aggressorID,passDest,field,sd->getSystemParams())),
          passDest);

      
          ((ReceivePassSkill*)skillHandle)->initialize(.5f,passDest,true,true);
      }
    
    }
  
    //-------------------
    //run Creator touch shoot skill
    //-------------------
    skillHandle->run();
  }else{


    //-------------------
    //set default message
    //-------------------
    sd->setMessage(ID, "CREATOR Touch Shoot");

    //-------------------
    //get a handle on this robot ID's skillset
    //-------------------
    SkillSet* skills = sd->getStrategyModule().getSkillSet(ID);

    //-------------------
    //get a handle on the tight defense blocker skill
    //-------------------
    OppositeReboundOffenseSkill* skillHandle = 
    (OppositeReboundOffenseSkill*)skills->getSkill(OppositeReboundOffenseSkill::skillNum);

    //-------------------
    //initialize skill if unitialized
    //-------------------
    if(!skillHandle->isInitialized())
	    skillHandle->initialize();

    //-------------------
    //run skill
    //-------------------
    skillHandle->run();
  }







}
//----------------------------------------------------------------------------