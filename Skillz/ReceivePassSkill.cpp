#include "ReceivePassSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "common/util/math/angle.h"
#include "GUI/GUI.h"
#include "common/util/pair/pair.h"
#include "common/util/math/cmath.h"
#include "common/util/math/dist.h"
#include "common/util/intersect/intersect.h"
#include "common/util/shoot/shoot.h"
#include "common/util/pass/pass.h"
#include "common/util/info/ball/possession.h"

int ReceivePassSkill::skillNum = -1;

//=====================================================
///Constructor.  Calls the base class Skill constructor
ReceivePassSkill::ReceivePassSkill(SystemParameters *sp,
                                   VisionModule *vm, 
                                   RoboCupStrategyModule *strat,
                                   RobotIndex id, 
                                   SkillSet* set) : Skill(sp,vm,strat,id,set) 
{
  initialized = false;
  loadValues();
}
//=====================================================
///Call this function to check the prerequisites of the skill.  This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool ReceivePassSkill::isValid() 
{ 
  return true;
}


//===============================================================================
///resets timer
void ReceivePassSkill::initialize() 
{
   //Get the current robot position
   Pair currentPos(getLocation(robotID,*currentVisionData,*sp));
   initialize(DEFAULT_MINIMUM_DISTANCE,currentPos,false,false);
}

//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void ReceivePassSkill::initialize(float MIN_DISTANCE, Pair position, bool KICKER_ON, bool to_extend)
{
  minDist=MIN_DISTANCE;
  //reset timer
  timer->resetTimer();
  initialized = true;

  // Get the ID of aggressor
  aggressorID = strategy->getCurrentRoboCupFrame()->getRobotByPosition(AGGRESSOR);

  // If no robot assigned as aggressor
  if (aggressorID == NO_ROBOT)
  {
    float closest = 32000;
    float ballDist = 0;
    RobotIndex closestRobot = NO_ROBOT;

    // Find the closest friendly robot to the ball and assume it to be the aggressor
    for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++)
    {
      ballDist=distanceToBall(*currentVisionData,sp->general.TEAM,i);
      if(ballDist < closest)
      {
        closest = ballDist;
        closestRobot = i;
      }
    }
    aggressorID = closestRobot;
  }
    
     // Get the aggressor Position
     Pair aggressorPos(getLocation(aggressorID,*currentVisionData,*sp)) ;

     //Line between the aggressor and creator
     Line AC(aggressorPos,position);

     // Point on the line perpendicular to AC
     Pair perpendicularPoint ;

     // Find line perpendicular to AC
     rotateAboutPoint(aggressorPos,position,PI/2,perpendicularPoint);
     GUI_Record.debuggingInfo.setDebugPoint(robotID,1,perpendicularPoint);
     Line perpendicularAC(position,perpendicularPoint);

     // This is the line we will be using for further calculations
     LineOfInterest.setA(perpendicularAC.getA());
     LineOfInterest.setB(perpendicularAC.getB());

     // Time to find the constraints
     Line leftLine(0,sp->field.LEFT_SIDE_LINE,1,sp->field.LEFT_SIDE_LINE);
     Line rightLine(0,sp->field.RIGHT_SIDE_LINE,1,sp->field.RIGHT_SIDE_LINE);
     Line topLine(sp->field.OUR_GOAL_LINE,0,sp->field.OUR_GOAL_LINE,1);
     Line bottomLine(sp->field.THEIR_GOAL_LINE,0,sp->field.THEIR_GOAL_LINE,1);
     Line goalBoxLine(sp->field.THEIR_GOALIE_BOX,0,sp->field.THEIR_GOALIE_BOX,1);
     Line rightGoalLine(0,sp->field.RIGHT_GOALIE_BOX,1,sp->field.RIGHT_GOALIE_BOX);
     Line leftGoalLine(0,sp->field.LEFT_GOALIE_BOX,1,sp->field.LEFT_GOALIE_BOX);

     Pair intersectionPoints[7];
	   bool validPoint[7];

     /// Find all the intersection points with different borders
     findLineIntersection(LineOfInterest,leftLine,&intersectionPoints[0]);
     findLineIntersection(LineOfInterest,rightLine,&intersectionPoints[1]);
     findLineIntersection(LineOfInterest,topLine,&intersectionPoints[2]);
     findLineIntersection(LineOfInterest,bottomLine,&intersectionPoints[3]);
     findLineIntersection(LineOfInterest,goalBoxLine,&intersectionPoints[4]);
     findLineIntersection(LineOfInterest,rightGoalLine,&intersectionPoints[5]);
     findLineIntersection(LineOfInterest,leftGoalLine,&intersectionPoints[6]);

     /// Find validity of the intersected points
	 if((intersectionPoints[0].getX() > sp->field.THEIR_GOAL_LINE) || (intersectionPoints[0].getX() <sp->field.OUR_GOAL_LINE))
	 {
		 validPoint[0] = false;
	 }
	 else
	 {
		 validPoint[0] = true;
	 }
    
	 if((intersectionPoints[1].getX() > sp->field.THEIR_GOAL_LINE) || (intersectionPoints[1].getX() <sp->field.OUR_GOAL_LINE))
	 {
		 validPoint[1] = false;
	 }
	 else
	 {
		 validPoint[1] = true;
	 }

	 if((intersectionPoints[2].getY() > sp->field.LEFT_SIDE_LINE) || (intersectionPoints[2].getY() <sp->field.RIGHT_SIDE_LINE))
	 {
		 validPoint[2] = false;
	 }
	 else
	 {
		 validPoint[2] = true;
	 }

	 if((intersectionPoints[3].getY() > sp->field.LEFT_SIDE_LINE) || (intersectionPoints[3].getY() <sp->field.RIGHT_SIDE_LINE))
	 {
		 validPoint[3] = false;
		 isExtension = false;
	 }
	 else
	 {
		 validPoint[3] = true;
		 
     if(to_extend)
     {
        isExtension = true;
		    extensionBoundary.setX(intersectionPoints[3].getX());
		    extensionBoundary.setY(intersectionPoints[3].getY());
     }
     else
     {
        isExtension = false;
     }
	 }

	 if((intersectionPoints[4].getY() > sp->field.LEFT_GOALIE_BOX) || (intersectionPoints[4].getY() <sp->field.RIGHT_GOALIE_BOX))
	 {
		 validPoint[4] = false;
	 }
	 else
	 {
		 validPoint[4] = true;
	 }
   
   if((intersectionPoints[5].getX() > sp->field.THEIR_GOAL_LINE) || (intersectionPoints[5].getX() <sp->field.THEIR_GOALIE_BOX))
	 {
		 validPoint[5] = false;
	 }
	 else
	 {
		 validPoint[5] = true;
	 }

	 if((intersectionPoints[6].getX() > sp->field.THEIR_GOAL_LINE) || (intersectionPoints[6].getX() <sp->field.THEIR_GOALIE_BOX))
	 {
		 validPoint[6] = false;
	 }
	 else
	 {
		 validPoint[6] = true;
	 }

	 /// The first point will be less than aggressor X  while second point will be greater than aggressor X
	 float leastPlusXDiff = 32000.0f;
	 float leastNegXDiff = 32000.0f;
	 float leastPlusYDiff = 32000.0f;
	 float leastNegYDiff = 32000.0f;

	 float pXdiff = 0;
	 float nXdiff = 0;
	 float pYdiff = 0;
	 float nYdiff = 0;
	 
	 /// Reset the boundaries
   boundary1.setX(32000.0f);
   boundary2.setX(32000.0f);
	 
	 for (int x = 0; x<7;x++)
	 {
		 if(validPoint[x])
		 {
			 if(intersectionPoints[x].getX() != position.getX())
			 {
				 if(intersectionPoints[x].getX() >  position.getX())
				 {
            pXdiff = intersectionPoints[x].getX() - position.getX();
					  if(pXdiff < leastPlusXDiff)
					  {
						  boundary2.setX(intersectionPoints[x].getX());
						  boundary2.setY(intersectionPoints[x].getY());
						  leastPlusXDiff = pXdiff;
					  }
				 }
				 else
				 {
					  nXdiff = position.getX() - intersectionPoints[x].getX();
                      if(nXdiff < leastNegXDiff)
					  {
						  boundary1.setX(intersectionPoints[x].getX());
						  boundary1.setY(intersectionPoints[x].getY());
						  leastNegXDiff = nXdiff;
					  }
				 }
			 }
			 else
			 {
				 if(intersectionPoints[x].getY() >  position.getY())
				 {
            pYdiff = intersectionPoints[x].getY() - position.getY();
					  if(pYdiff < leastPlusYDiff)
					  {
						  boundary2.setX(intersectionPoints[x].getX());
						  boundary2.setY(intersectionPoints[x].getY());
						  leastPlusYDiff = pYdiff;
					  }
				 }
				 else
				 {
          nYdiff = position.getY() - intersectionPoints[x].getY();
          if(nYdiff < leastNegYDiff){
            boundary1.setX(intersectionPoints[x].getX());
            boundary1.setY(intersectionPoints[x].getY());
            leastNegYDiff = nYdiff;
          }
        }
      }
    }
  }

  /// If any boundaries not found set it to the current position of robot
  if(boundary1.getX() == 32000.0f)
  {
   boundary1.setX(position.getX());
   boundary1.setY(position.getY());
  }

  if(boundary2.getX() == 32000.0f)
  {
   boundary2.setX(position.getX());
   boundary2.setY(position.getY());
  }

  /// Check if the boundaries are less than or equal to allowed distance
  if(dist(boundary1,position) < MIN_DISTANCE)
  {
   extendPoint(boundary1,position,-MIN_DISTANCE,boundary1);
  }

  if(dist(boundary2,position) < MIN_DISTANCE)
  {
   extendPoint(boundary2,position,-MIN_DISTANCE,boundary2);
  }

  originalBoundary = boundary2;
  intendedPosition = position;
  destPos=intendedPosition;
  if(ABS(angleDifference(angleBetween(intendedPosition,aggressorPos),
    angleBetween(intendedPosition,Pair(sp->field.THEIR_GOAL_LINE,sp->field.SPLIT_LINE)))) <= VALID_KICK_ANGLE){
    kicker_on = KICKER_ON;
  }else{
    kicker_on = false;
  }
  charging = false;
}
//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void ReceivePassSkill::execute()
{      

  ///If not active, dont do anything!
  if(!initialized)
  {
    return;  
  }

  /// Find the opponent which is closest to the goal point
  Pair goalPoint(sp->field.THEIR_GOAL_LINE,(sp->field.LEFT_GOAL_POST+sp->field.RIGHT_GOAL_POST+2)/2);
  float closest = 32000.0f;
  RobotIndex closeRobot = NO_ROBOT;

  Pair robot=getLocation(robotID,*currentVisionData,*sp);
  for(RobotIndex i=ROBOT0;i<NUM_PLAYERS_ON_TEAM;i++){
    Pair C(getLocation(sp->general.OTHER_TEAM,i,*currentVisionData));
    if(dist(C,goalPoint) < closest)
    {
	    closest = dist(C,goalPoint);
	    closeRobot = i;
    }
  }
	
  /// If the distance of our robot from the closest robot 
  /// is greater than certain Threshold
  if(closeRobot != NO_ROBOT)
  {
    Pair closeRobotLocation(getLocation(sp->general.OTHER_TEAM,closeRobot,*currentVisionData));
    if(dist(robot,closeRobotLocation) >= GAP)
    {
	    if(isExtension)
	    {  
		    boundary2 = extensionBoundary;
        command->setControl(OMNI_NORMAL_ENTERBOX);
	    }
    }
    else
    {
	    boundary2 = originalBoundary;
      command->setControl(OMNI_NORMAL);
    }
  }
  else
  {
    if(isExtension)
    {  
      boundary2 = extensionBoundary;
      command->setControl(OMNI_NORMAL_ENTERBOX);
    }
  }

  //GUI_Record.debuggingInfo.setDebugPoint(robotID,1,boundary1);
  //GUI_Record.debuggingInfo.setDebugPoint(robotID,2,boundary2);
     
  /// Find the direction in which the ball is going
  Line ballVelocityVector;
  ballVelocityVector.setA( getRobocupBall(*currentVisionData).getPos() );

  //Get the current robot position
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));

  //Get the ball location
  Pair ballPos(strategy->getCurrentRoboCupFrame()->getOffensiveBallLocation());

  // try to find where our shot is
  bool isShot;
  Pair goalPos;
  isShot = calcShot(frontOfRobot(robotID,*currentVisionData,*sp),sp->field.THEIR_GOAL_LINE,sp->strategy2002.SHOOT_LANE_THRESH,sp->field.RIGHT_GOAL_POST,sp->field.LEFT_GOAL_POST,
  NO_ROBOT,*currentVisionData,*sp,&goalPos);

  if(!isShot)
  {
    goalPos.setX(sp->field.THEIR_GOAL_LINE);
    goalPos.setY((sp->field.LEFT_GOAL_POST + sp->field.RIGHT_GOAL_POST)/2);
  }

  /// Rotation will be split angle and the kicker will be ON
  float angle;

  Pair ballVelocity = ballVel(*currentVisionData);
    // do rebounding or catching.
  if(kicker_on)
  {
      if(ABS(command->getRotation()) < (PI/2)*KICK_RATIO)
      {command->setKick(KICK_SHOT);}
      angle = reboundAngle(ballVelocity,frontOfRobot(robotID,*currentVisionData,*sp),goalPos,*sp);///findshootAngle(currentPos,goalPos,ballPos,BIAS);   
    
  }
  else //we want to face the ball and dribble
  {
    angle=angleBetween(currentPos,ballPos);
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);
  }
  command->setRotation(angle);

  if(getBallSpeed(*currentVisionData) >= THRESHOLD_SPEED)
  {
    
    float radius=dist(ballPos,frontOfRobot(robotID,*currentVisionData,*sp)) * 
        SIN(fabs(angleDifference(angleBetween(frontOfRobot(robotID,*currentVisionData,*sp),ballPos) , 
            angleBetween(ballVelocity.getX(),ballVelocity.getY(),0.0f,0.0f))));
    if(radius < CHARGE_RADIUS*sp->general.PLAYER_RADIUS &&
       ballPos.distanceTo(currentPos) > MIN_CHARGE_DIST){
      //we want to charge the ball.
      charging=true;
      strategy->getCurrentFrame()->setMessage(robotID, "ReceivePass: Charging the ball");
      command->setPos(robotPositionGivenFront(ballPos,angle,*sp));
    }else{ // we don't want to charge, do regular intercepting.
      if(charging){
        //if we were charging before, we'd better re-initialize ourselves.
        unInitialize();
        initialize(minDist,frontOfRobot(robotID,*currentVisionData,*sp),kicker_on,isExtension);
      }
      charging=false;
      ballVelocityVector.setBposX(
                           getRobocupBall(*currentVisionData).getXPos() + 
                           (5.0f*getRobocupBall(*currentVisionData).getXVel()) 
                         );
      ballVelocityVector.setBposY(
                           getRobocupBall(*currentVisionData).getYPos() +
                           (5.0f*getRobocupBall(*currentVisionData).getYVel())
                         );
      /// Before finding check if they are not parallel
      float slopeFirst,slopeSecond;
      if(((ballVelocityVector.getB()).getX() - (ballVelocityVector.getA()).getX()) != 0)
      {
        slopeFirst = ((ballVelocityVector.getB()).getY() - (ballVelocityVector.getA()).getY()) / ((ballVelocityVector.getB()).getX() - (ballVelocityVector.getA()).getX()) ;
      }
      else
      {
        slopeFirst = 32000.0f;
      }

      if(((LineOfInterest.getB()).getX() - (LineOfInterest.getA()).getX()) != 0)
      {
        slopeSecond = ((LineOfInterest.getB()).getY() - (LineOfInterest.getA()).getY()) / ((LineOfInterest.getB()).getX() - (LineOfInterest.getA()).getX()) ;
      }
      else
      {
        slopeSecond = 32000.0f;
      }
     
      if(slopeFirst != slopeSecond)
      {
        findLineIntersection(ballVelocityVector,LineOfInterest,&destPos);
      }

      /// Intersection of Line of Interest and ball's veleocityVector will be where
      /// we want the creator to be

      ///Corrected position of creator
      //Pair correctedPos;

      //GUI_Record.debuggingInfo.setDebugPoint(robotID,0,destPos);

      /** The process of finding the rotation */



      /// if the creator is not between the two boundaries then restrict it within
      /// Player radius of the boundaries
      if(!destPos.isBetween(boundary1,boundary2))
      {
        strategy->getCurrentFrame()->setMessage(robotID, "Ball headed outside of receive zone");
        if(dist(destPos,boundary1) < dist(destPos,boundary2))
        {
          destPos.set(boundary1);
          //extendPoint(boundary2,boundary1,-(sp->general.PLAYER_RADIUS+OFFSET_BOUNDARY),correctedPos);
        }
        else
        {
          destPos.set(boundary2);
          //extendPoint(boundary1,boundary2,-(sp->general.PLAYER_RADIUS+OFFSET_BOUNDARY),correctedPos);
        }
      }
      else
      {
        strategy->getCurrentFrame()->setMessage(robotID, "Receiving Ball");
      }
      command->setPos(robotPositionGivenFront(destPos,angle,*sp));

    }// charging if
  }
  else // ball moving slowly
  {
    if(!friendlyHasPossession(aggressorID,*sp)){
      strategy->getCurrentFrame()->setMessage(robotID, "ReceivePass: Ball too slow, charging");
      command->setPos(ballPos);
    }else{
      strategy->getCurrentFrame()->setMessage(robotID, "ReceivePass: I don't think it's been passed yet");
      command->setPos(robotPositionGivenFront(destPos,angle,*sp));
    }
  }
  

  //Debugging info
  GUI_Record.debuggingInfo.setDebugPoint(robotID,0,command->getPos());
  GUI_Record.debuggingInfo.setDebugPoint(robotID,1,boundary1);
  GUI_Record.debuggingInfo.setDebugPoint(robotID,2,boundary2);

}
//=====================================================
///If the timer goes past 3 secs, reverse rotation
bool ReceivePassSkill::evaluate() 
{
  return true;
}

//=====================================================
///For tweaking the skill.  You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void ReceivePassSkill::update() 
{
  ///at this time simple screening does not "learn"
}

//=====================================================
///Screen is never finished, it can always continue
bool ReceivePassSkill::isFinished(float tolerance){
  return false;
}

///Reads in the values of the parameters from the skill's parameter text file.
void ReceivePassSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/ReceivePassSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  READ_FLOAT(DEFAULT_MINIMUM_DISTANCE);
  READ_FLOAT(BIAS);
  READ_FLOAT(THRESHOLD_SPEED);
  READ_FLOAT(GAP);
  READ_FLOAT(OFFSET_BOUNDARY);
  READ_FLOAT(VALID_KICK_ANGLE);
  READ_FLOAT(KICK_RATIO);
  READ_FLOAT(CHARGE_RADIUS);
  READ_FLOAT(MIN_CHARGE_DIST);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();
  
}