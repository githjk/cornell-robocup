////////////////////////////////////////////////////////////////////
//  VisionModule.CPP
//
//  This file contains all the functions necessary for processing
//  raw vision data 
//
////////////////////////////////////////////////////////////////////

#include "VisionModule.h"

// Don't add prediction to Ball or Opponents if both velocities are below this threshold
const float IGNORE_PREDICTION = 0.015f; 
const float IGNORE_TRANSLATION_COMMAND = 0.025f;
const float IGNORE_ROTATION_COMMAND    = 0.05f;

// If the filtering process yields velocities above these values, reset the filter state
// All these are in metres/sec
const float BALL_ERROR_VELOCITY_SQUARED   = 100.0f;
const float OPPONENT_ERROR_VELOCITY_SQUARED = 16.0f;

//*****************************************************************************
//  INITIALIZE_PREDICTION
//
//  Initialize Static Variables
//*****************************************************************************

void VisionModule::InitializePrediction()
{
  num_latency_frames = (int)sp->general.LATENCY_FRAMES;
  other_num_latency_frames = (int)sp->general.OTHER_LATENCY_FRAMES;
  
  latency_frame = (float)(1 / sp->general.FRAME_RATE);
  squared_latency_frame = latency_frame*latency_frame;
  
  latency_fraction = (sp->general.LATENCY_FRAMES - num_latency_frames) * latency_frame;
  squared_latency_fraction = latency_fraction*latency_fraction;
  
  other_latency_fraction = (sp->general.OTHER_LATENCY_FRAMES - other_num_latency_frames) * latency_frame;
  other_squared_latency_fraction = other_latency_fraction * other_latency_fraction;
  
  
  // Initially, everything is lost
  for (int i = 0; i < NUM_PLAYERS_ON_TEAM; i++) 
  {
    robotLost[i]  = sp->general.OUR_LOST_FRAMES;
    opponentLost[i] = sp->general.OPPONENT_LOST_FRAMES;
  }
  
  for (i = 0; i < NUM_BALLS; i++) 
  {
    ballLost[TEAM_ONE][i] = sp->general.BALL_LOST_FRAMES;
    ballLost[TEAM_TWO][i] = sp->general.BALL_LOST_FRAMES;
  }
  
  //intialize last frame data for the ball to lost so we can safely
  //copy that data over ever is ball is lost in first frame
  
  for (int k=0; k<MAX_LOG; k++)
  {
    for (int team=0; team<2; team++)
    {
      for (int ballIndex=0; ballIndex < NUM_BALLS; ballIndex++)
      {
        visionData[k].setBallPos(team, ballIndex, Pair(BAD_PRED_ERR, BAD_PRED_ERR));
        visionData[k].setBallFound(team, ballIndex, false);
        visionData[k].setBallXVel(team, ballIndex, 0.0f);
        visionData[k].setBallYVel(team, ballIndex, 0.0f);
      }
      
      for (int robotIndex=0; robotIndex < NUM_ROBOTS; robotIndex++)
      {
        visionData[k].setRobotFound(team, robotIndex, false);
        visionData[k].setRobotRotation(team, robotIndex, 0.0f);
        visionData[k].setRobotRotVel(team, robotIndex, 0.0f);
        visionData[k].setRobotXPos(team, robotIndex, BAD_PRED_ERR);
        visionData[k].setRobotYPos(team, robotIndex, BAD_PRED_ERR);
        visionData[k].setRobotXVel(team, robotIndex, 0.0f);
        visionData[k].setRobotYVel(team, robotIndex, 0.0f);
      }
    }
  }
}

//*****************************************************************************
//  PREDICT_VISION
//
//  Our vision data has a certain delay, or latency, associated with it - our
//  most recent vision data is actually several frames old.  Therefore, we must
//  predict where the robots and ball will be when our next command actually
//  gets executed.
//*****************************************************************************

void VisionModule::predictVision(ControlData command[MAX_LOG])
{
  int i;
  
//  if (visionRunning)
//  {
    
    visionData[currentFrame].startTime    = incomingVision.startTime;
    visionData[currentFrame].finishTime   = incomingVision.finishTime;
    visionData[currentFrame].instruction  = incomingVision.instruction;
    
    // Which robots were found?
    for (i=0;i<NUM_PLAYERS_ON_TEAM; i++)
    {
      visionData[currentFrame].robotsFound[sp->general.TEAM][i]= incomingVision.robotsFound[sp->general.TEAM][i];
      visionData[currentFrame].robotsFound[sp->general.OTHER_TEAM][i]= incomingVision.robotsFound[sp->general.OTHER_TEAM][i];
    }
    // Which balls were found?
    for (i=0;i<NUM_BALLS; i++)
    {
      visionData[currentFrame].ballsFound[sp->general.TEAM][i] = incomingVision.ballsFound[sp->general.TEAM][i];
      visionData[currentFrame].ballsFound[sp->general.OTHER_TEAM][i]= incomingVision.ballsFound[sp->general.OTHER_TEAM][i];
    }
    
    // Predict The Balls of both teams
    for (i = 0; i < NUM_BALLS; i++)
    {
      predictBalls(sp->general.TEAM, i);
      predictBalls(sp->general.OTHER_TEAM, i);
    }
    
    // Predict each individual enemy robot
    for (i = 0; i < NUM_PLAYERS_ON_TEAM; i++)
    {
      predictOpponentRobots(i);
    }
    
    // Predict each individual friendly robot
    for (i = 0; i < NUM_PLAYERS_ON_TEAM; i++)
    {
      predictFriendlyRobots(i,command);
    }

//  } // end 'if (visionRunning)'
  
}

//*****************************************************************************//

void VisionModule::filterBallLocation(int team, int ball, float posx, float posy)
{
  float z[2];
  z[0] = posx; z[1] = posy;
  float out[2][2];
  filteredBalls[team][ball].updatePosition(z,out);
  visionData[currentFrame].setBallXPos(team, ball,out[0][0]);
  visionData[currentFrame].setBallXVel(team, ball,out[0][1]);
  visionData[currentFrame].setBallYPos(team, ball,out[1][0]);
  visionData[currentFrame].setBallYVel(team, ball,out[1][1]);
}


void VisionModule::filterOpponentLocation(int robotID,float posx, float posy)
{
  float z[2];
  z[0] = posx; z[1] = posy;
  float out[2][2];
  filteredOpponents[robotID].updatePosition(z,out);

  if(sp->general.ENABLE_PREDICTION_AND_FILTERING_OPPONENTS)
  { 
    visionData[currentFrame].setRobotXPos(sp->general.OTHER_TEAM,robotID,out[0][0]);
    visionData[currentFrame].setRobotXVel(sp->general.OTHER_TEAM,robotID,out[0][1]);
    visionData[currentFrame].setRobotYPos(sp->general.OTHER_TEAM,robotID,out[1][0]);
    visionData[currentFrame].setRobotYVel(sp->general.OTHER_TEAM,robotID,out[1][1]);
  }
  else
  {
    visionData[currentFrame].setRobotXPos(sp->general.OTHER_TEAM,robotID,posx);
    visionData[currentFrame].setRobotXVel(sp->general.OTHER_TEAM,robotID,0.0f);
    visionData[currentFrame].setRobotYPos(sp->general.OTHER_TEAM,robotID,posy);
    visionData[currentFrame].setRobotYVel(sp->general.OTHER_TEAM,robotID,0.0f);
  }
}




//*****************************************************************************
//  PREDICT_OPPONENT_ROBOT
//
//  This function uses a Kalman Filter to filter where the opponent robots were
//  when the vision snapshot was taken, then uses linear prediction to guess 
//  where they will be when the next command gets executed
//*****************************************************************************

inline void VisionModule::predictOpponentRobots( int robot )
{
  //if opponent found and is inside the field??
  if ( visionData[currentFrame].isRobotFound(sp->general.OTHER_TEAM,robot) && 
    incomingVision.team[sp->general.OTHER_TEAM][robot].XPos < sp->field.THEIR_GOAL_LINE + sp->field.GOAL_DEPTH &&
    incomingVision.team[sp->general.OTHER_TEAM][robot].XPos > sp->field.OUR_GOAL_LINE - sp->field.GOAL_DEPTH &&
    incomingVision.team[sp->general.OTHER_TEAM][robot].YPos < sp->field.LEFT_SIDE_LINE + sp->general.WALL_BUFFER &&
    incomingVision.team[sp->general.OTHER_TEAM][robot].YPos > sp->field.RIGHT_SIDE_LINE - sp->general.WALL_BUFFER
    ) 
  {  
    XPos = incomingVision.team[sp->general.OTHER_TEAM][robot].XPos;
    YPos = incomingVision.team[sp->general.OTHER_TEAM][robot].YPos;
    
    // If the bot was lost last frame, reset filter
    if ( opponentLost[robot] > 0 ) {
      
      float post[2];
      post[0] = XPos; post[1] = YPos;
      filteredOpponents[robot].initializePos(post);
      XVel = 0.0f;
      YVel = 0.0f;
    }
    
    opponentLost[robot] = 0;
    visionData[currentFrame].setRobotFound(sp->general.OTHER_TEAM,robot,true);       
    
    // Filter Opponent Data
    filterOpponentLocation(robot,XPos,YPos);
    
    // Make sure our filtered velocities are reasonable
    if ((visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getXVel() * visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getXVel()) + 
      (visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getYVel() * visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getYVel()) > 
      OPPONENT_ERROR_VELOCITY_SQUARED) 
    {
      XPos = incomingVision.team[sp->general.OTHER_TEAM][robot].XPos;
      YPos = incomingVision.team[sp->general.OTHER_TEAM][robot].YPos;
      
      XVel = 0.0f;
      YVel = 0.0f;
      
      //  TO DO : Verify whether this is necessary
      // filterOpponentLocation(robot,XPos,YPos);
    }
    
    // Predict Opponent Data
    if ( ( fabs(visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getXVel()) < IGNORE_PREDICTION ) && 
      ( fabs(visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getYVel()) < IGNORE_PREDICTION ) )
    {
      visionData[currentFrame].setRobotXVel(sp->general.OTHER_TEAM,robot,0.0f);
      visionData[currentFrame].setRobotYVel(sp->general.OTHER_TEAM,robot,0.0f);
    }
    
//====================================
//====================================
//====================================
    if(sp->general.ENABLE_PREDICTION_AND_FILTERING_OPPONENTS)
    {    
    // Predict the robot to go forward
    visionData[currentFrame].setRobotXPos(sp->general.OTHER_TEAM,robot,
      visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getXPos() + visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getXVel() * sp->general.OTHER_LATENCY_FRAMES*latency_frame);

    // Predict the robot to go forward
    visionData[currentFrame].setRobotYPos(sp->general.OTHER_TEAM,robot, 
      visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getYPos() + visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getYVel() * sp->general.OTHER_LATENCY_FRAMES*latency_frame);
    }
//====================================
//====================================
//====================================
    
  }
  else 
  {
    opponentLost[robot]++;
    
    visionData[currentFrame].setRobotXVel(sp->general.OTHER_TEAM,robot, visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getRobot(sp->general.OTHER_TEAM,robot).getXVel());
    visionData[currentFrame].setRobotYVel(sp->general.OTHER_TEAM,robot, visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getRobot(sp->general.OTHER_TEAM,robot).getYVel());
    visionData[currentFrame].setRobotXPos(sp->general.OTHER_TEAM,robot, visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getRobot(sp->general.OTHER_TEAM,robot).getXPos());
    visionData[currentFrame].setRobotYPos(sp->general.OTHER_TEAM,robot, visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getRobot(sp->general.OTHER_TEAM,robot).getYPos());
    
    // Predict opponent positions either way, but if the opponent remains lost for more than
    // p->OPPONENT_LOST_FRAMES, then:
    //    - set the visionData[currentFrame].RobotsFound[OTHER_TEAM][robot] to false
    //    - set the opponent velocity to zero
    if ( opponentLost[robot] > sp->general.OPPONENT_LOST_FRAMES ) 
    {  

      //--- bookmark
      visionData[currentFrame].setRobotFound(sp->general.OTHER_TEAM,robot,false);       
      visionData[currentFrame].setRobotXVel(sp->general.OTHER_TEAM,robot,0.0f);
      visionData[currentFrame].setRobotYVel(sp->general.OTHER_TEAM,robot,0.0f);
      visionData[currentFrame].setRobotXPos(sp->general.OTHER_TEAM,robot,-32000.0f);
      visionData[currentFrame].setRobotYPos(sp->general.OTHER_TEAM,robot,-32000.0f);
/*
      // If the opponent is lost, set its velocity to zero, and leave it at
      //    its last known location
      visionData[currentFrame].setRobotFound(sp->general.OTHER_TEAM,robot,true);       
      visionData[currentFrame].setRobotXVel(sp->general.OTHER_TEAM,robot,0.0f);
      visionData[currentFrame].setRobotYVel(sp->general.OTHER_TEAM,robot,0.0f);
*/
    }
    else
    {
      // make predictions assuming it is going in the same direction 
      visionData[currentFrame].setRobotFound(sp->general.OTHER_TEAM,robot,true);       
      
      // Predict Opponent Data
      if ( ( fabs(visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getXVel()) < IGNORE_PREDICTION ) && 
        ( fabs(visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getYVel()) < IGNORE_PREDICTION ) )
      {
        visionData[currentFrame].setRobotXVel(sp->general.OTHER_TEAM,robot, 0.0f);
        visionData[currentFrame].setRobotYVel(sp->general.OTHER_TEAM,robot, 0.0f);
      }
      
      // linear interpolation assuming robot going in the same direction of motion
      visionData[currentFrame].setRobotXPos(sp->general.OTHER_TEAM,robot, 
        visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getXPos() + visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getXVel() * sp->general.OTHER_LATENCY_FRAMES*latency_frame);
      
      visionData[currentFrame].setRobotYPos(sp->general.OTHER_TEAM,robot, 
        visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getYPos() + visionData[currentFrame].getRobot(sp->general.OTHER_TEAM,robot).getYVel() * sp->general.OTHER_LATENCY_FRAMES*latency_frame);
    }
  }
}

//*****************************************************************************
//  PREDICT_BALL
//
//  This function uses a Kalman Filter to filter where the ball was when the 
//  vision snapshot was taken, then uses linear prediction to guess where it
//  will be when the next command gets executed
//*****************************************************************************

inline void VisionModule::predictBalls( int team, int ball )
{
  
  if (incomingVision.ball[team][ball].XPos < sp->field.THEIR_GOAL_LINE + sp->field.GOAL_DEPTH &&
    incomingVision.ball[team][ball].XPos > sp->field.OUR_GOAL_LINE - sp->field.GOAL_DEPTH &&
    incomingVision.ball[team][ball].YPos < sp->field.LEFT_SIDE_LINE  + sp->general.WALL_BUFFER &&
    incomingVision.ball[team][ball].YPos > sp->field.RIGHT_SIDE_LINE - sp->general.WALL_BUFFER &&
    visionData[currentFrame].isBallFound(team,ball)
    )
  {
    
    XPos = incomingVision.ball[team][ball].XPos;
    YPos = incomingVision.ball[team][ball].YPos;
    // If the ball was lost last frame, reset filter
    if ( ballLost[team][ball] > 0 ) {
      float post[2];
      post[0] = XPos; post[1] = YPos;
      filteredBalls[team][ball].initializePos(post);
      XVel = 0.0f;
      YVel = 0.0f;
    }
    
    ballLost[team][ball] = 0;     
    visionData[currentFrame].setBallFound(team,ball,true); 

    // Filter Ball Data
    filterBallLocation(team,ball, XPos, YPos);
    
    // Make sure our filtered velocities are reasonable
    if ((visionData[currentFrame].getBall(team,ball).getXVel() * visionData[currentFrame].getBall(team,ball).getXVel()) + 
      (visionData[currentFrame].getBall(team,ball).getYVel() * visionData[currentFrame].getBall(team,ball).getYVel()) > 
      BALL_ERROR_VELOCITY_SQUARED) 
    {
      XPos = incomingVision.ball[team][ball].XPos;
      YPos = incomingVision.ball[team][ball].YPos;
      
      XVel = 0.0f;
      YVel = 0.0f;
      // filterBallLocation(team,ball, XPos, YPos);
    }
    
    // If our velocities are small enough, set them to zero
    if (( fabs(visionData[currentFrame].getBall(team,ball).getXVel()) < IGNORE_PREDICTION ) &&
      ( fabs(visionData[currentFrame].getBall(team,ball).getYVel()) < IGNORE_PREDICTION ) )
    {
      visionData[currentFrame].setBallXVel(team,ball,0.0f);
      visionData[currentFrame].setBallYVel(team,ball,0.0f);
    }
    
    predictBallForward(team,ball);
    
  }
  else // ball not found 
  {
    ballLost[team][ball]++;
    
    // We may be able to improve our prediction algorithm when the ball
    // gets lost to better account for:
    //    Robots occluding ball
    //    Goalie Ball (ours and theirs)
    
    
    // If the ball remains lost for more than p->BALL_LOST_FRAMES, then set the 
    // ball velocity to zero
    if ( ballLost[team][ball] > sp->general.BALL_LOST_FRAMES ) 
    { 
      //use last ball, 0 vel  
      visionData[currentFrame].setBallXPos(team,ball,visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getBall(team,ball).getXPos());
      visionData[currentFrame].setBallYPos(team,ball,visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getBall(team,ball).getYPos());  
      visionData[currentFrame].setBallXVel(team,ball,0.0f);
      visionData[currentFrame].setBallYVel(team,ball,0.0f);    
      visionData[currentFrame].setBallFound(team,ball,false);
    }
    else
    {
      // make predictions assuming it is going in the same direction since it is not lost for too long
      visionData[currentFrame].setBallXPos(team,ball,visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getBall(team,ball).getXPos());
      visionData[currentFrame].setBallYPos(team,ball,visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getBall(team,ball).getYPos());
      visionData[currentFrame].setBallXVel(team,ball,visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getBall(team,ball).getXVel());
      visionData[currentFrame].setBallYVel(team,ball,visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getBall(team,ball).getYVel());
      visionData[currentFrame].setBallFound(team,ball,true);
      predictBallForward(team,ball);
    } 
  } 
}

//*****************************************************************************//
//Simple code for Ball prediction
//get the position of the ball from vision data(which is lagging by the 
//latency number of frames from the robot frame of reference)%%
void VisionModule::predictBallForward( int team, int ball )
{
  
  float k = .25f; //velocity derate every sec(units (m/s)/s)
  float frame_rate = sp->general.FRAME_RATE;
  float tsample = (float)1.0f/(float)frame_rate;
  float frames_ahead = sp->general.OTHER_LATENCY_FRAMES; //can be any real number theoretically
  
  float vx_vision = visionData[currentFrame].getBall(team,ball).getXVel(); 
  float vy_vision = visionData[currentFrame].getBall(team,ball).getYVel();
  
  float xpos_vision = visionData[currentFrame].getBall(team,ball).getXPos();
  float ypos_vision = visionData[currentFrame].getBall(team,ball).getYPos();
  
  float vball_vision = float(sqrt(vx_vision*vx_vision + vy_vision*vy_vision));
  
  float t = frames_ahead*tsample;
  
  float v = vball_vision - k*t;
  float dist0 = vball_vision*t - k*(t*t)/2.0f;
  
  float dist;
  float vball_pred;
  
  // if speed turns out to be negative..it means that ball has stopped, so calculate that amount of 
  // distance traveled
  if(v < 0){
    vball_pred = 0.0f;
    dist = (vball_vision*vball_vision)*k/2.0f;
    // i.e the ball has stopped, so take a newer vision data for the prediction
  }
  else{
    vball_pred = v;
    dist = dist0;
  }
  
  if(vball_vision != 0){
    visionData[currentFrame].setBallXVel(team,ball,vball_pred*(vx_vision)/vball_vision);
    visionData[currentFrame].setBallYVel(team,ball,vball_pred*(vy_vision)/vball_vision);
    visionData[currentFrame].setBallXPos(team,ball,xpos_vision + dist*(vx_vision)/vball_vision);
    visionData[currentFrame].setBallYPos(team,ball,ypos_vision + dist*(vy_vision)/vball_vision);
  }
  
  return;
}


//*****************************************************************************
//  STEP FORWARD MACROS
//
//  These macros are used to predict the X, Y and Rotation variables ahead in time
//
//
//  'Velocity_Step' is used if the robots attempt to reach the commanded velocity
//    immediately (approximately instantaneous)
//*****************************************************************************

#define USE_ESTIMATED_VELOCITIES

void VisionModule::Velocity_StepXPos(ControlData command[MAX_LOG],int team,int robot, float timeStep, int frame){

  int tempFrame = (currentFrame-frame+MAX_LOG)%MAX_LOG;

  // use the estimated velocities if this frame was reset due to prediction error
  if ( rawVisionData[tempFrame].validData &&
       rawVisionData[tempFrame].resetTranslationalVelocity[robot]
     )
  {

#ifdef USE_ESTIMATED_VELOCITIES
    visionData[currentFrame].setRobotXPos(team,robot, 
      ( visionData[currentFrame].getRobot(team,robot).getXPos() +
        rawVisionData[tempFrame].teams[team][robot].getXVel()*timeStep) 
      );
#else
    return;
#endif

  }
  // Only predict forward if the commanded velocity is big enough
  else if ( ABS( command[tempFrame].getXVel(robot) ) > IGNORE_TRANSLATION_COMMAND)
  {
    visionData[currentFrame].setRobotXPos(team,robot, 
      ( visionData[currentFrame].getRobot(team,robot).getXPos() +
        command[tempFrame].getXVel(robot)*timeStep) 
      );
  }
}

void VisionModule::Velocity_StepYPos(ControlData command[MAX_LOG],int team,int robot, float timeStep, int frame){

  int tempFrame = (currentFrame-frame+MAX_LOG)%MAX_LOG;

  // use the estimated velocities if this frame was reset due to prediction error
  if ( rawVisionData[tempFrame].validData &&
       rawVisionData[tempFrame].resetTranslationalVelocity[robot]
     )
  {

#ifdef USE_ESTIMATED_VELOCITIES
    visionData[currentFrame].setRobotYPos(team,robot, 
      ( visionData[currentFrame].getRobot(team,robot).getYPos() +
        rawVisionData[tempFrame].teams[team][robot].getYVel()*timeStep) 
      );
#else
    return;
#endif

  }
  // Only predict forward if the commanded velocity is big enough
  else if ( ABS( command[tempFrame].getYVel(robot) ) > IGNORE_TRANSLATION_COMMAND)
  {
    visionData[currentFrame].setRobotYPos(team,robot,
      ( visionData[currentFrame].getRobot(team,robot).getYPos() +
        command[tempFrame].getYVel(robot)*timeStep) );
  }
}

void VisionModule::Velocity_StepRotation(ControlData command[MAX_LOG],int team,int robot, float timeStep, int frame){

  int tempFrame = (currentFrame-frame+MAX_LOG)%MAX_LOG;

  // use the estimated velocities if this frame was reset due to prediction error
  if ( rawVisionData[tempFrame].validData &&
       rawVisionData[tempFrame].resetRotationalVelocity[robot]
     )
  {

#ifdef USE_ESTIMATED_VELOCITIES
    visionData[currentFrame].setRobotRotation(team,robot,
      normalizeAngle(visionData[currentFrame].getRobot(team,robot).getRotation()  +
      rawVisionData[tempFrame].teams[team][robot].getRotVel()*timeStep)
    );
#else
    return;
#endif

  }
  // Only predict forward if the commanded velocity is big enough
  else if ( ABS( command[tempFrame].getRotVel(robot) ) > IGNORE_ROTATION_COMMAND)
  {
    visionData[currentFrame].setRobotRotation(team,robot,
      normalizeAngle(visionData[currentFrame].getRobot(team,robot).getRotation()  +
      command[tempFrame].getRotVel(robot)*timeStep)
    );
  }
}

//*****************************************************************************
//
//  PREDICT_FRIENDLY_ROBOT
//
//  This function uses the last few commands to predict where the robot will be
//  when the next command gets executed
//
//*****************************************************************************

inline void VisionModule::predictFriendlyRobots( int robot, ControlData command[MAX_LOG] )
{
  
  //******************
  //  POSITION FOUND
  //******************
  
  // If the robot was found, predict forward several frames to account for latency
  if ( visionData[currentFrame].isRobotFound(sp->general.TEAM,robot) &&
    incomingVision.team[sp->general.TEAM][robot].XPos < sp->field.THEIR_GOAL_LINE + sp->field.GOAL_DEPTH &&
    incomingVision.team[sp->general.TEAM][robot].XPos > sp->field.OUR_GOAL_LINE - sp->field.GOAL_DEPTH &&
    incomingVision.team[sp->general.TEAM][robot].YPos < sp->field.LEFT_SIDE_LINE + sp->general.WALL_BUFFER &&
    incomingVision.team[sp->general.TEAM][robot].YPos > sp->field.RIGHT_SIDE_LINE -  sp->general.WALL_BUFFER
    )
  {
    robotLost[robot] = 0;
    visionData[currentFrame].setRobotFound(sp->general.TEAM,robot,true);
    
    
    //Get raw position
    visionData[currentFrame].setRobotXPos(sp->general.TEAM,robot,incomingVision.team[sp->general.TEAM][robot].XPos);
    visionData[currentFrame].setRobotYPos(sp->general.TEAM,robot,incomingVision.team[sp->general.TEAM][robot].YPos);
    
    //Offset position because position marker not centered on robot
    visionData[currentFrame].setRobotXPos(sp->general.TEAM,robot,visionData[currentFrame].getRobot(sp->general.TEAM,robot).getXPos()+sp->general.VISION_OFFSET*COS(visionData[currentFrame].getRobot(sp->general.TEAM,robot).getRotation()));
    visionData[currentFrame].setRobotYPos(sp->general.TEAM,robot,visionData[currentFrame].getRobot(sp->general.TEAM,robot).getYPos()+sp->general.VISION_OFFSET*SIN(visionData[currentFrame].getRobot(sp->general.TEAM,robot).getRotation()));
    
    //Set velocity to last command
    visionData[currentFrame].setRobotXVel(sp->general.TEAM,robot, (float)command[(currentFrame-1+MAX_LOG)%MAX_LOG].getXVel(robot));
    visionData[currentFrame].setRobotYVel(sp->general.TEAM,robot, (float)command[(currentFrame-1+MAX_LOG)%MAX_LOG].getYVel(robot));
    visionData[currentFrame].setRobotRotVel(sp->general.TEAM,robot, (float)command[(currentFrame-1+MAX_LOG)%MAX_LOG].getRotVel(robot));

    //***********************************
    // POSITION FOUND - PREDICT POSITION
    //***********************************
    if (latency_fraction > 0.0f) 
    {
      Velocity_StepXPos( command,sp->general.TEAM,robot, latency_fraction, (num_latency_frames + 1));
      Velocity_StepYPos( command,sp->general.TEAM,robot, latency_fraction, (num_latency_frames + 1));
    }
    
    int i;
    for (i = num_latency_frames; i > 0; i --) 
    {
      Velocity_StepXPos( command,sp->general.TEAM,robot, latency_frame, i );
      Velocity_StepYPos( command,sp->general.TEAM,robot, latency_frame, i );
    }
    
    //***********************************
    // POSITION FOUND - PREDICT ROTATION
    //***********************************
    // If the rotation was found, predict forward several frames to account for latency
    // if ( UNINITIALIZED != incomingVision.team[sp->general.TEAM][robot].Rotation ) {
    if ( ABS(incomingVision.team[sp->general.TEAM][robot].Rotation) < 100.0f ) 
    {
      visionData[currentFrame].setRobotRotation(sp->general.TEAM,robot,incomingVision.team[sp->general.TEAM][robot].Rotation);
      
      if (latency_fraction>0)
      {
        Velocity_StepRotation( command,sp->general.TEAM,robot, latency_fraction, (num_latency_frames + 1) );
      }
      
      for (i = num_latency_frames; i > 0; i --) 
      {
        Velocity_StepRotation( command,sp->general.TEAM,robot, latency_frame, i );
      }
    }
    else 
    {
      visionData[currentFrame].setRobotRotation(sp->general.TEAM,robot, visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getRobot(sp->general.TEAM,robot).getRotation());    
      Velocity_StepRotation( command,sp->general.TEAM,robot, latency_fraction, 1 );
    }
  }
  
  //******************
  //  POSITION LOST
  //******************
  // If the robot was lost, predict forward one more frame from last time
  else 
  {
    robotLost[robot]++;
    
    // If the robot remains lost for more than p->OUR_LOST_FRAMES, then set 
    // visionData[currentFrame].RobotsFound to false
    if ( robotLost[robot] > sp->general.OUR_LOST_FRAMES ) 
    {
      visionData[currentFrame].setRobotRotation(sp->general.TEAM,robot,0.0f);
      visionData[currentFrame].setRobotXPos(sp->general.TEAM,robot,-32000.0f);
      visionData[currentFrame].setRobotYPos(sp->general.TEAM,robot,-32000.0f);
      visionData[currentFrame].setRobotXVel(sp->general.TEAM,robot, 0.0f);
      visionData[currentFrame].setRobotYVel(sp->general.TEAM,robot, 0.0f);
      visionData[currentFrame].setRobotFound(sp->general.TEAM,robot,false);
    }
    else
    {
      visionData[currentFrame].setRobotFound(sp->general.TEAM,robot,true);
      
      //use last position
      visionData[currentFrame].setRobotXPos(sp->general.TEAM,robot, visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getRobot(sp->general.TEAM,robot).getXPos());
      visionData[currentFrame].setRobotYPos(sp->general.TEAM,robot, visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getRobot(sp->general.TEAM,robot).getYPos());
      visionData[currentFrame].setRobotRotation(sp->general.TEAM,robot, visionData[(currentFrame-1+MAX_LOG)%MAX_LOG].getRobot(sp->general.TEAM,robot).getRotation());
      
      //use last commands as velocity
      visionData[currentFrame].setRobotXVel(sp->general.TEAM,robot, (float)command[(currentFrame-1+MAX_LOG)%MAX_LOG].getXVel(robot));
      visionData[currentFrame].setRobotYVel(sp->general.TEAM,robot, (float)command[(currentFrame-1+MAX_LOG)%MAX_LOG].getYVel(robot));
      visionData[currentFrame].setRobotRotVel(sp->general.TEAM,robot, (float)command[(currentFrame-1+MAX_LOG)%MAX_LOG].getRotVel(robot));
      
      //step based on velocity
      Velocity_StepXPos( command,sp->general.TEAM,robot,  latency_frame, 1 );
      Velocity_StepYPos( command,sp->general.TEAM,robot, latency_frame, 1 );
      Velocity_StepRotation( command,sp->general.TEAM,robot, latency_frame, 1 );
    }    
  } // if ( RobotsFound  )
  
  
  //********************
  // FINAL CALCULATIONS
  //********************
  // Normalize Angles
  if( visionData[currentFrame].isRobotFound(sp->general.TEAM,robot)) {
    while ( visionData[currentFrame].getRobot(sp->general.TEAM,robot).getRotation() > PI ) {
      visionData[currentFrame].setRobotRotation(sp->general.TEAM,robot, visionData[currentFrame].getRobot(sp->general.TEAM,robot).getRotation() - TWO_PI);
    }
    while ( visionData[currentFrame].getRobot(sp->general.TEAM,robot).getRotation() < -PI ) {
      visionData[currentFrame].setRobotRotation(sp->general.TEAM,robot,visionData[currentFrame].getRobot(sp->general.TEAM,robot).getRotation()+ TWO_PI);
    }
  }  
}
