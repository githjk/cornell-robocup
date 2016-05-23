#include "creatorAssistOffenseTransition.h"
#include "common/util/math/dist.h"
#include "common/util/shoot/shoot.h"
#include "info/ball/ball.h"

#include "plays/2002plays/upfieldDefense.h"
#include "plays/2002plays/jamAndShoot.h"
#include "plays/2002plays/touchShoot.h"

#include "Skillz/CreatorCreateSkill.h"
#include "Skillz/PassToCreatorSkill.h"
#include "common/util/lane/lane.h"
#include "common/util/info/ball/possession.h"
#include "common/util/pair/pair.h"
//*********************************************************
void CreatorAssistOffenseTransition::executePlayTransition(VisionData& field, 
                                                        RobocupStrategyData* rsd, 
			  									                              BasePlay* currentPlay)
{
  if(rsd->getRobotByPosition(AGGRESSOR)!= NO_ROBOT  &&  rsd->getRobotByPosition(CREATOR)!= NO_ROBOT)
  {
        SkillSet* skills = rsd->getStrategyModule().getSkillSet
          (rsd->getRobotByPosition(CREATOR));
        Skill* skillHandle = skills->getSkill(CreatorCreateSkill::skillNum);
        skills = rsd->getStrategyModule().getSkillSet
          (rsd->getRobotByPosition(AGGRESSOR));
        Skill* AggSkill = skills->getSkill(PassToCreatorSkill::skillNum);
        //Aggressor doesn't have the ball and ball not between aggressor and creator
        //then go to upfield defense
        if(
            !friendlyHasPossession(rsd->getRobotByPosition(AGGRESSOR),
                                   rsd->getSystemParams(),
                                   field,
                                   *rsd,
                                   true) &&
             !ballIsBetween(getLocation(rsd->getRobotByPosition(AGGRESSOR), 
                                        field, 
                                        rsd->getSystemParams()),
                            getLocation(rsd->getRobotByPosition(CREATOR),
                                        field, 
                                        rsd->getSystemParams()),
                            field,
                            rsd->getSystemParams(),
                            2*rsd->getSystemParams().general.PLAYER_RADIUS,
                            1.5f*rsd->getSystemParams().general.PLAYER_RADIUS)
          )
        {
          creatorTime=true;
		      setPlay(UpfieldDefense::playNum, rsd);
	      }
        else if(!creatorTime)
        { //Creator's there, check and see how aggressor's doing.
          if(AggSkill->isFinished()){
            creatorTime=true;
            setPlay(TouchShoot::playNum, rsd);
          }else if(currentPlay->readTimer() >= MAX_AGGRESSOR_TIME){
            creatorTime=true;
		        setPlay(JamAndShoot::playNum, rsd);
          }
        }
        //if creator finished (at his final state)
        else if(skillHandle->isFinished())
        {
          /*//if a lane is open, go to touch shoot and pass
          if(AggSkill->isFinished() )/*laneHalfWidth(getLocation(rsd->getRobotByPosition(AGGRESSOR),
                                       field,
                                       rsd->getSystemParams()),
                           getLocation(rsd->getRobotByPosition(CREATOR),
                                       field,
                                       rsd->getSystemParams()),
                           field,
                           rsd->getSystemParams(),
                           true) > rsd->getSystemParams().strategy2002.PASS_LANE_THRESH/2.0f)*/
          //{
            creatorTime=false;
            currentPlay->startTimer();
          /*}
          //else creator made it around, but aggressor was unable to turn.
          else
          {

		        setPlay(JamAndShoot::playNum, rsd);
          }*/
        }
        //else if max time elapsed, go to jam and shoot
        else if(currentPlay->readTimer() >= MAX_CREATOR_TIME)
	      {
          creatorTime=true;
		      setPlay(JamAndShoot::playNum, rsd);
	      }
  }
  else
  {
    creatorTime=true;
    setPlay(UpfieldDefense::playNum, rsd);
  }
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void CreatorAssistOffenseTransition::loadValues()
{
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/CreatorAssistOffenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(MAX_CREATOR_TIME);
  READ_FLOAT(MAX_AGGRESSOR_TIME);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************

