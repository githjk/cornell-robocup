#include "singleAssistOffenseTransition.h"
#include "common/util/math/dist.h"
#include "common/util/shoot/shoot.h"
#include "common/util/info/robot/robot.h"
#include "common/util/pair/pair.h"
#include "common/util/math/angle.h"
#include "common/util/defense/defending.h"
#include "info/ball/ball.h"

#include "skillz/CrossCreaseCreatorSkill.h"
#include "skillz/CreatorPassSkill.h"
#include "skillz/BackPassSkill.h"
#include "skillz/PullBallOffWallSkill.h"
#include "skillz/JamAndShootSkill.h"
#include "skillz/AdvanceBallUpfieldSkill.h"
#include "plays/2002plays/kickUpfield.h"
#include "plays/2002plays/doublePickOffense.h"
#include "plays/2002plays/jamAndShoot.h"
#include "plays/2002plays/creatorAssistOffense.h"
#include "plays/2002plays/crossCreasePass.h"
#include "plays/2002plays/CreatorPass.h"
#include "plays/2002plays/BackPass.h"
#include "plays/2002plays/upfieldDefense.h"

//===============================================================================
void SingleAssistOffenseTransition::executePlayTransition(VisionData& field, 
                                                          RobocupStrategyData* rsd, 
                        			  									        BasePlay* currentPlay)
{
  //-----------------------------------------------------------------------------------
  //calculate two extreme lines
  PASSING_LINE = rsd->getSystemParams().field.THEIR_GOAL_LINE + PASSING_LINE_THEIR_GOALLINE_OFFSET;
  DOUBLE_PICK_LINE = rsd->getSystemParams().field.HALF_LINE + DOUBLE_PICK_OFFSET;
  KICK_LINE=rsd->getSystemParams().field.OUR_GOAL_LINE + KICK_LINE_OFFSET;
  //-----------------------------------------------------------------------------------
  Pair ballPos = rsd->getOffensiveBallLocation();
  //-----------------------------------------------------------------------------------
  bool openShot = false;
  Pair aggressorLoc;
  float aggressorRot;
  RobotIndex AggressorID = rsd->getRobotByPosition(AGGRESSOR);
  RobotIndex CreatorID = rsd->getRobotByPosition(CREATOR);
  
  RobotIndex SpecialOpID = rsd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
  if(SpecialOpID == NO_ROBOT)
    SpecialOpID = rsd->getRobotByPosition(SPECIAL_OP_CREATOR);
  if(SpecialOpID == NO_ROBOT)
    SpecialOpID = rsd->getRobotByPosition(SPECIAL_OP_DEFENDER);
  

  //-----------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------
  RobotIndex activeRobotID;
  //If no robot (But blocker) has possesion of ball, and ball is not between the aggrssor and
  //his destination then go to upfield defense
  if(
      (
        !weHavePossession(field,rsd->getMutableSystemParams(),*rsd, &activeRobotID) ||
        activeRobotID == rsd->getRobotByPosition(BLOCKER) ||
        activeRobotID == rsd->getRobotByPosition(DEFENDER)
      ) &&
      (
        rsd->getRobotByPosition(AGGRESSOR) == NO_ROBOT ||
        (
         !ballIsBetween(getLocation(rsd->getRobotByPosition(AGGRESSOR), field, rsd->getSystemParams()),
                        rsd->getDestination(rsd->getRobotByPosition(AGGRESSOR))->getPos(),
                        field,
                        rsd->getSystemParams(),
                        2*rsd->getSystemParams().general.PLAYER_RADIUS,
                        1.5f*rsd->getSystemParams().general.PLAYER_RADIUS) &&
         isRobocupBallFound(field)
        )
      )
    )
  {
    setPlay(UpfieldDefense::playNum, rsd);
  }
  //Only transition out if not pulling off wall.
  else if(AggressorID != NO_ROBOT && 
          !rsd->getStrategyModule().getSkillSet(AggressorID)->getSkill(PullBallOffWallSkill::skillNum)->isValid())
  {
    if(AggressorID != NO_ROBOT)
    {
      aggressorLoc = getLocation(AggressorID,field, rsd->getSystemParams());
  	  aggressorRot = getRotation(AggressorID,field, rsd->getSystemParams());
      openShot=rsd->getStrategyModule().getSkillSet(AggressorID)->getSkill(JamAndShootSkill::skillNum)->isValid();
    }
    //-----------------------------------------------------------------------------------
    //start timer if ball is in offensive zone
	  if(ballPos.getX() > rsd->getSystemParams().field.HALF_LINE)
	  {
		  if(timerOn == false)
		  {
		    timer->resetTimer();
		  }
		  timerOn = true;
	  }
    //-----------------------------------------------------------------------------------
    //if there is a clear shot on the goal by the aggressor then transition to jam and shoot
    // if we are ahead of the half line and facing towards the goal
    if (AggressorID != NO_ROBOT &&
             aggressorLoc.getX() > rsd->getSystemParams().field.HALF_LINE &&
             openShot /*&&
             towardGoal(aggressorLoc,
                        aggressorRot,
                        rsd->getSystemParams().field.THEIR_GOAL_LINE,
                        rsd->getSystemParams().field.LEFT_GOAL_POST,
                        rsd->getSystemParams().field.RIGHT_GOAL_POST)*/
       )
    {
		    setPlay(JamAndShoot::playNum, rsd);
    }
    //-----------------------------------------------------------------------------------
    //if ball ahead of pasing line
    else if(ballPos.getX() >= PASSING_LINE &&
            AggressorID != NO_ROBOT)
    {
      //-----
      //Get Aggressor's Location
      Pair aggressorLocation;
      if(AggressorID != NO_ROBOT)
      {
        aggressorLocation.set(getLocation(AggressorID,
                                           field,
                                           rsd->getSystemParams()));
      }
      //-----
      //Get Creator's Location
      Pair creatorLocation;
      if(CreatorID != NO_ROBOT)
      {
        creatorLocation.set(getLocation(CreatorID,
                                      field,
                                      rsd->getSystemParams()));
      }
        //-----------------------------------------------------------------------------------
      Skill* crossCrease;
      if(CreatorID !=NO_ROBOT) 
      {
        crossCrease = rsd->getStrategyModule().getSkillSet(CreatorID)->getSkill(CrossCreaseCreatorSkill::skillNum);
      }

      //-----
      //if lane between aggressor and creator, go to cross crease pass
      if(AggressorID != NO_ROBOT &&
         CreatorID != NO_ROBOT &&
         crossCrease->isValid() &&
         aggressorLocation.distanceTo(creatorLocation) > rsd->getSystemParams().strategy2002.MIN_PASS_DEST)
      {
		    setPlay(CrossCreasePass::playNum, rsd);
      }   
      
      //else if creator is being blocked by closely, go to creator assist offense
      else if(rsd->getSystemParams().strategy2002.ENABLE_CREATOR_ASSIST &&
              AggressorID != NO_ROBOT &&
              CreatorID != NO_ROBOT &&
              SpecialOpID != NO_ROBOT &&
              manOnCreator(field, rsd, aggressorLocation, creatorLocation) &&
              aggressorLocation.distanceTo(creatorLocation) >= MIN_SEP_DIST &&
              aggressorLocation.distanceTo(creatorLocation) > rsd->getSystemParams().strategy2002.MIN_PASS_DEST
              )
      {
  		  setPlay(CreatorAssistOffense::playNum, rsd);
      }
      
      //check for a back pass 
      else if(rsd->getSystemParams().strategy2002.ENABLE_BACK_PASS &&
              aggressorLocation.getX() > rsd->getSystemParams().field.KILL_ZONE_LINE &&
              (aggressorLocation.getY() > rsd->getSystemParams().field.LEFT_MIDDLE_SECTOR ||
               aggressorLocation.getY() < rsd->getSystemParams().field.RIGHT_MIDDLE_SECTOR) &&
              SpecialOpID != NO_ROBOT &&
              rsd->getStrategyModule().getSkillSet(SpecialOpID)->getSkill(BackPassSkill::skillNum)->isValid() &&
              aggressorLocation.distanceTo(getLocation(SpecialOpID,field,rsd->getSystemParams())) > 
                  rsd->getSystemParams().strategy2002.MIN_PASS_DEST
             )
      {
          setPlay(BackPass::playNum, rsd);
      }else{
        //else go to jam and shoot
        setPlay(JamAndShoot::playNum, rsd);

      }
    }
    //else behind passing line, if ahead of rear line, consider a double picks
    else if(ballPos.getX() >= DOUBLE_PICK_LINE &&
            AggressorID != NO_ROBOT &&
            CreatorID != NO_ROBOT &&
            SpecialOpID != NO_ROBOT)
    {
      if(twoOpponentsFound(field, rsd)) //does a lot more than just find 2 opponents
      {
  		  setPlay(DoublePickOffense::playNum, rsd);
      }
    }
    //ball behind rear line, go into kick upfield
    else if(AggressorID != NO_ROBOT &&
            ballPos.getX() <= KICK_LINE &&
            ABS(getRotation(rsd->getRobotByPosition(AGGRESSOR),field,rsd->getSystemParams()))
            <= KICK_UPFIELD_ANGLE)
    {
      Pair lanePos;
      //Get Aggressor's Location
      RobotIndex AggressorID = rsd->getRobotByPosition(AGGRESSOR);
      Pair aggressorLocation = getLocation(AggressorID,
                                           field,
                                           rsd->getSystemParams());                                                                                  
      float angle=getRotation(AggressorID,field,rsd->getSystemParams());
      //if the ball on the side of the field;
      if((ballPos.getY() > rsd->getSystemParams().field.RIGHT_SIDE_LINE - EDGE_DIST) ||
        (ballPos.getY() < rsd->getSystemParams().field.LEFT_SIDE_LINE - EDGE_DIST)){
        lanePos.set(ballPos.getX() + EXTEND_OFFSET, ballPos.getY());
      //else it's somewhere in the middle
      }else{
        lanePos.set(aggressorLocation.getX() + EXTEND_OFFSET, aggressorLocation.getY());
        rotateAboutPoint(lanePos,aggressorLocation,angle,lanePos);
      }
    
      //see if we're facing a good direction  and there is a lane
      if(isLane(aggressorLocation,lanePos, KICK_UPFIELD_LANE_WIDTH,field,rsd->getSystemParams(),true)){
  		  setPlay(KickUpfield::playNum, rsd);
      }
    }
    else
    {
      Skill* creatorPass;
      Pair dest;
      if(CreatorID !=NO_ROBOT){
        creatorPass = rsd->getStrategyModule().getSkillSet(CreatorID)->getSkill(CreatorPassSkill::skillNum);
        dest=rsd->getPassDest(CreatorID);
      }
      //-----
      //if lane between aggressor and creator, go to cross crease pass
      // also, constrain so that we never pass in front of our goal.
      float nearPost;
      int side;
      if(aggressorLoc.getY() > rsd->getSystemParams().field.SPLIT_LINE){
        nearPost=rsd->getSystemParams().field.LEFT_GOAL_POST + rsd->getSystemParams().general.PLAYER_RADIUS;
        side=1;
      }else{
        nearPost=rsd->getSystemParams().field.RIGHT_GOAL_POST - rsd->getSystemParams().general.PLAYER_RADIUS;
        side=-1;
      }

      //-----
      //Get Aggressor's Location
      Pair aggressorLocation;
      if(AggressorID != NO_ROBOT)
      {
        aggressorLocation.set(getLocation(AggressorID,
                                           field,
                                           rsd->getSystemParams()));
      }
      //-----
      //Get Creator's Location
      Pair creatorLocation;
      if(CreatorID != NO_ROBOT)
      {
        creatorLocation.set(getLocation(CreatorID,
                                      field,
                                      rsd->getSystemParams()));
      }

      if(AggressorID != NO_ROBOT &&
         CreatorID != NO_ROBOT &&
         creatorPass->isValid() &&
         ( //check for safe pass
          (aggressorLoc.getX() > rsd->getSystemParams().field.OUR_GOAL_LINE + PASSING_DANGER_ZONE &&
           dest.getX() > rsd->getSystemParams().field.HALF_LINE
          ) ||
          (side * dest.getY() > side * nearPost)
         ) &&
         aggressorLocation.distanceTo(creatorLocation) > rsd->getSystemParams().strategy2002.MIN_PASS_DEST
        )
      {
		    setPlay(CreatorPass::playNum, rsd);
      }              
    }
  } //end if pullballoffwall

}
//===============================================================================
//returns true if there is an opponent very close to the creator
bool SingleAssistOffenseTransition::manOnCreator(const VisionData& field, 
                                                 RobocupStrategyData* rsd,
                                                 Pair aggressorLocation,
                                                 Pair creatorLocation)
{
  Pair opLoc;
  //if no creator, no
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    opLoc.set(getTheirRobotLocation(i, field, rsd->getSystemParams()));
    if(creatorLocation.distanceTo(opLoc) < (5.0f*rsd->getSystemParams().general.PLAYER_RADIUS) &&
       ABS(angleDifference(angleBetween(creatorLocation,
                                        aggressorLocation),
                           angleBetween(creatorLocation,
                                        opLoc)
                          )
          ) < MAN_ON_CONE_ANGLE)
    {
      return true;
    }
  }
  return false;
}
//===============================================================================
//Returns true if there are two opponents between the split line and the side
//closer to the ball but are not in the opponent goalie box and less than 2 on oppsite side
bool SingleAssistOffenseTransition::twoOpponentsFound(const VisionData& field, 
                                                      RobocupStrategyData* rsd) 
{ 

 
  bool consider;
  bool leftSide;
  
  int oppsSame = 0;
  int oppsOpposite = 0;
  
  Pair ballPos = rsd->getOffensiveBallLocation();
    
	//if the ball is not far enough to the outside, return false
  if(ABS(ballPos.getY() - rsd->getSystemParams().field.SPLIT_LINE) < DOUBLE_PICK_DIST){
    return false;
  }
  
  //find if ball is closer to right side or left side
	if(ballPos.getY() > rsd->getSystemParams().field.SPLIT_LINE)
	{
		leftSide = true;
	}
	else
	{
		leftSide = false;
	}

  //Iterate through all opponents and check if we should ignore them or not
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) 
  {
    //assume valid
    consider = true;
    
    float oppX = getTheirXLocation(i, field, rsd->getSystemParams());
    float oppY = getTheirYLocation(i, field, rsd->getSystemParams());


        //opponent robot must exist
    if(theirRobotFound(i, field, rsd->getSystemParams()) &&
        //not in goalie box
		    !(oppY < rsd->getSystemParams().field.LEFT_GOALIE_BOX &&
            oppY > rsd->getSystemParams().field.RIGHT_GOALIE_BOX &&
            oppX > rsd->getSystemParams().field.THEIR_GOALIE_BOX) &&
        //must be forward of ball
        (oppX > ballPos.getX())){
      if(leftSide && oppY >= rsd->getSystemParams().field.SPLIT_LINE ||
         !leftSide && oppY < rsd->getSystemParams().field.SPLIT_LINE)
      {
        oppsSame++;
      }
      else
      {
        oppsOpposite++;
      }
    } 
  }
  
  return (oppsSame >= 2 && oppsOpposite < 2) ; 
}
//*********************************************************
///Reads in the values of the parameters from the plays's parameter text file.
void SingleAssistOffenseTransition::loadValues()
{
  timerOn = false;  //initialize timer
  ///Open the paramater file:
  ifstream mainFile;
  mainFile.open("Params/Play_Params/playTransition/SingleAssistOffenseTransitionParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(MAX_ELAPSED_TIME);
  READ_FLOAT(PASSING_LINE_THEIR_GOALLINE_OFFSET);
  READ_FLOAT(DOUBLE_PICK_OFFSET);
  READ_FLOAT(KICK_LINE_OFFSET);
  READ_FLOAT(EDGE_DIST);
  READ_FLOAT(EXTEND_OFFSET);
  READ_FLOAT(KICK_UPFIELD_LANE_WIDTH);
  READ_FLOAT(KICK_UPFIELD_ANGLE);
  READ_FLOAT(DOUBLE_PICK_DIST);
  READ_FLOAT(MIN_SEP_DIST);
  READ_FLOAT(MAN_ON_CONE_ANGLE);
  READ_FLOAT(PASSING_DANGER_ZONE);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//*********************************************************



