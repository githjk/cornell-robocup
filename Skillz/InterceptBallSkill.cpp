#include "InterceptBallSkill.h"
#include "MoveToBallSkill.h"
#include "ReceivePassSkill.h"
#include "info/robot/robot.h"
#include "pair/pair.h"
#include "info/ball/possession.h"
#include "common/util/shoot/shoot.h"
#include "math/dist.h"
#include "math/angle.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/control/Trajectory.h"

int InterceptBallSkill::skillNum = -1;

//===============================================================================
//Constructor.  Calls the base class Skill constructor
InterceptBallSkill::InterceptBallSkill(SystemParameters *sp,
                                       VisionModule *vm, 
                                       RoboCupStrategyModule *strat,
                                       RobotIndex id, 
                                       SkillSet* set) : Skill(sp,vm,strat,id, set)
{
  loadValues();
}
//===============================================================================
/// Check to see if we have the ball or not.
bool InterceptBallSkill::isValid() 
{ 
  return true;
}
//===============================================================================
///resets timer
void InterceptBallSkill::initialize() 
{
  timer->resetTimer();
  destPoint.setPos(getBallLocation(*currentVisionData));
  destPoint.setRotation(0.0f);
  hold=false;
  ballFast = true;
  initialized=true;
}
//===============================================================================
//Execute the skill.  This is the main part of the skill, where you tell the
//robot how to perform the skill.
void InterceptBallSkill::execute()
{    
  ///If not active, dont do anything!
  if(!initialized) return; 

  Pair ballPoint = getBallLocation(*currentVisionData);
  float ballSpeed = getBallSpeed(*currentVisionData);
  Pair ballVelocity = ballVel(*currentVisionData);
  MovingObject robot = getRobot(robotID,*currentVisionData,*sp);
  float faceAngle;
  
  ReceivePassSkill* rpass=(ReceivePassSkill *)skillSet->getSkill(ReceivePassSkill::skillNum);  

  //check if ball is moving fast
  Pair currentPos(getLocation(robotID,*currentVisionData,*sp));

  if(ballFast && ballSpeed < (BALL_SPEED_THRESHOLD - 
                              BALL_SPEED_THRESHOLD_HISTORISIS_FACTOR))
  {
    ballFast = false;
  }
  else if(!ballFast && ballSpeed > (BALL_SPEED_THRESHOLD + 
                                    BALL_SPEED_THRESHOLD_HISTORISIS_FACTOR))
  {
    ballFast = true;
  }

  //ball slow, run it down
  if(!ballFast)
  {
    strategy->getCurrentFrame()->setMessage(robotID,"moving to");
    skillSet->getSkill(MoveToBallSkill::skillNum)->initialize();
    skillSet->getSkill(MoveToBallSkill::skillNum)->run();
  }
  //see if the ball is moving away from us and not towards us
  else if(ABS(angleDifference(ATAN2(ballVelocity.getY(),ballVelocity.getX()),
                           angleToBall(robotID,*currentVisionData,*sp))) <=  FLEEING_ANGLE){
      //chase the ball
      rpass->unInitialize();
      strategy->getCurrentFrame()->setMessage(robotID,"running after the ball");
      ((MoveToBallSkill*)skillSet->getSkill(MoveToBallSkill::skillNum))->initialize(true);
      skillSet->getSkill(MoveToBallSkill::skillNum)->run();
  }
  else{
  //ball fast, intercept

    //test if we're in front of the ball:
    float radius=dist(ballPoint,currentPos) * 
        SIN(fabs(angleDifference(angleToBall(robotID,*currentVisionData,*sp) , 
            angleBetween(ballVelocity.getX(),ballVelocity.getY(),0.0f,0.0f))));
    if(radius < CATCH_RADIUS*sp->general.PLAYER_RADIUS){
      if(radius < INNER_CATCH*sp->general.PLAYER_RADIUS){
        command->setPos(ballPoint);
        command->setRotation(angleToBall(robotID,*currentVisionData,*sp));
        strategy->getCurrentFrame()->setMessage(robotID,"Charging the ball");
        rpass->unInitialize();
      }else{
        //initialize every time.
        if(!rpass->isInitialized()){
          rpass->initialize(0.3f,frontOfRobot(robot.getPos(),destPoint.getRotation(),*sp),false,false);
        }
        rpass->run();
      }
    //check to see if we can see the ball
    }else {
      rpass->unInitialize();
      if(isRobocupBallFound(*currentVisionData) && !hold){
        //find a good intersection point
        destPoint.setPos(ballPoint);
        Pair nearPoint,farPoint;
        nearPoint=ballPoint;
        farPoint.setX(ballPoint.getX()+ballVelocity.getX()*3.0f);
        farPoint.setY(ballPoint.getY()+ballVelocity.getY()*3.0f);

        float leadTime=LEAD_TIME;

        bool interceptfound=false;
        ObjectPosVel start, final;
        for(int count=0; count<PRECISION;count++){
          destPoint.setXPos((nearPoint.getX()+farPoint.getX())/2);
          destPoint.setYPos((nearPoint.getY()+farPoint.getY())/2);
          GUI_Record.debuggingInfo.setDebugPoint(robotID,count,destPoint.getXPos(),destPoint.getYPos());

        
          start.xPos = robot.getXPos();
          start.yPos = robot.getYPos();
          start.xVel = robot.getXVel();
          start.yVel = robot.getYVel();
          start.rotation= robot.getRotation();
          start.rotVel= robot.getRotVel();
        
          final.xPos = destPoint.getXPos();
          final.yPos = destPoint.getYPos();
          faceAngle=angleBetween(robot,ballPoint);
          /*if(faceAngle <= BIAS_ANGLE){
            faceAngle=0;
          }else{
            if (faceAngle<0) faceAngle+=BIAS_ANGLE;
            else faceAngle-=BIAS_ANGLE;
          }*/
        
          final.rotation=faceAngle;
          //final.rotation=start.rotation;
          final.xVel = 0.0f;
          final.yVel = 0.0f;
          final.rotVel= 0.0f;

          float robotTime;
          expectedPathTime(start,
                           final,
                           sp->general.FRAME_RATE,
                           strategy->getRobotTrajParams(robotID)->DEFAULT.MAX_ACCELERATION,
                           strategy->getRobotTrajParams(robotID)->DEFAULT.MAX_VELOCITY,
                           robotTime);
            
          float ballTime=dist(destPoint,ballPoint)/ballSpeed;
          if(robotTime+leadTime<=ballTime){
            farPoint=destPoint.getPos(); 
          }else{
            nearPoint=destPoint.getPos();
          }
        }
        strategy->getCurrentFrame()->setMessage(robotID,"Intercept found");
        //set rotation to be more towards their end
        destPoint.setRotation(faceAngle);

        //Keep the destination on the field and out of the goalie box
        constrainToField(*sp,&(destPoint.getPos()));
        if(nearOurGoalieBox(destPoint.getPos(),*sp,sp->general.PLAYER_RADIUS)){
          extrapolateForY(ballPoint,farPoint,
                        sp->field.OUR_GOALIE_BOX+sp->general.PLAYER_RADIUS,
                        destPoint.getPos());
        }
      }
      command->setPos(destPoint.getPos());
      command->setRotation(destPoint.getRotation());
    }
    command->setDribble(FAST_DRIBBLE);
    command->setVerticalDribble(FAST_V_DRIBBLE);

    //keep in field, and our of our goalie box!!!
    Pair dLoc(command->getPos());
    constrainToField(*sp, &dLoc);

    if(strategy->getCurrentRoboCupFrame()->getRobotByPosition(BLOCKER) != NO_ROBOT)
      constrainOutOfOurBox(*sp, &dLoc);

    //if ball past their goal line, keep out of their box
    if(ballPoint.getX() >= sp->field.THEIR_GOAL_LINE)
      constrainOutOfTheirBox(*sp, &dLoc);
    
    command->setPos(dLoc);
  }

    //arm kicker if we have a shot, or it's in their box
  if((goodCurrentShot(robot.getPos(),robot.getRotation(),*currentVisionData,*sp,sp->strategy2002.SHOOT_LANE_THRESH) &&
      robot.getXPos() > sp->field.HALF_LINE) ){
    command->setKick(KICK_SHOT);
    command->setDribble(NO_DRIBBLE);
    command->setVerticalDribble(NO_V_DRIBBLE);
  }

}
//===============================================================================
///always true for now.
bool InterceptBallSkill::evaluate() 
{
  return true;
}
//===============================================================================
//For tweaking the skill.  You may want to change local parameters or behave 
//differently to adapt to any situation that is frustrating the skill
void InterceptBallSkill::update() 
{
  
}
//===============================================================================
///check if we've reached our target destination and rotation
bool InterceptBallSkill::isFinished(float tolerance)
{
  return friendlyHasPossession(robotID,*sp);
}
//===============================================================================
///Reads in the values of the parameters from the skill's parameter text file.
void InterceptBallSkill::loadValues()
{
  ///Open the paramater file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/InterceptBallSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");

  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  ///Read in a test param
  READ_FLOAT(BALL_SPEED_THRESHOLD);
  READ_FLOAT(BALL_SPEED_THRESHOLD_HISTORISIS_FACTOR);
  READ_INT(PRECISION);
  READ_FLOAT(LEAD_TIME);
  READ_FLOAT(DRIBBLE_DISTANCE);
  READ_FLOAT(CATCH_RADIUS);
  READ_FLOAT(INNER_CATCH);
  READ_FLOAT(FLEEING_ANGLE);
  READ_FLOAT(BIAS_ANGLE);

  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
}
//===============================================================================
