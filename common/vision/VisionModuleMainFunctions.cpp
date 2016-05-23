////////////////////////////////////////////////////////////////////
//  VisionModule.CPP
//
//  This file contains all the functions necessary for processing
//  raw vision data 
//
////////////////////////////////////////////////////////////////////

#include "VisionModule.h"
#include "common/util/math/dist.h"

manualObjectPlacement GUI_Placement;

//*****************************************************************************
//  CONSTRUCTOR
//*****************************************************************************
VisionModule::VisionModule(SystemParameters* sp) 
{
  this->sp        = sp;
  currentFrame  = VISION_FIRST_FRAME;
  int i;

  // Clear visionData and rawVisionData
  memset(    visionData, 0, MAX_LOG*sizeof(   VisionData) );
  memset( rawVisionData, 0, MAX_LOG*sizeof(RawVisionData) );

  // Initialize the values in the ball filter array
  for (i=0;i<NUM_TEAMS;i++) {
    for (int k=0;k<NUM_BALLS;k++) {
      filteredBalls[i][k].initialize("Params\\Vision_Params\\ballFilterSlow.txt",
                                     "Params\\Vision_Params\\ballFilterFast.txt");
    }
  }

  // Initialize the values in the opponent filter array
  for (i=0;i<NUM_PLAYERS_ON_TEAM;i++) {
    filteredOpponents[i].initialize("Params\\Vision_Params\\opponentFilter.txt",
                                    "Params\\Vision_Params\\opponentFilter.txt");
  }
  
  //Initializes the arrays used by the prediction mechanisms
  InitializePrediction();
  
  //by default vision is NOT running
  visionRunning = true;   
  //by default sides of the field are NOT flipped 
  sidesFlipped = false;
  // create an instance of vision packet
  incomingPacket = new VisionPacket(VISION_PACKET_NUM_ROBOTS,VISION_PACKET_NUM_BALLS,VISION_PACKET_NUM_OBSTACLES);


  // Initialize all pathLengths to zero
  for (i=0;i<NUM_PLAYERS_ON_TEAM;i++) {
    translationalPathLength[i] = 0.0f;
       rotationalPathLength[i] = 0.0f;
    
    translationalErrorCount[i] = 0;
       rotationalErrorCount[i] = 0;
  }


  // these variables are used to manually place objects from the AI.
  for(i=0; i<NUM_ROBOTS; i++)
  {
    GUI_Placement.friendlyRobotX[i] = GUI_Placement.friendlyRobotY[i] = BAD_PRED_ERR;
    GUI_Placement.friendlyRobotRotation[i] = 0.0f;
    GUI_Placement.opponentRobotX[i] = GUI_Placement.opponentRobotY[i] = BAD_PRED_ERR;
    GUI_Placement.opponentRobotRotation[i] = 0.0f;
  }
  
  for(i=0; i<NUM_TEAMS; i++) {
    for(int j=0; j<NUM_BALLS; j++) {
      GUI_Placement.ballX[i][j] = BAD_PRED_ERR;
      GUI_Placement.ballY[i][j] = BAD_PRED_ERR;
    }
  }
}


//*****************************************************************************
//  DESTRUCTOR
//*****************************************************************************
VisionModule::~VisionModule() 
{ 
  if (incomingPacket!=NULL) 
    delete incomingPacket;
}


//*****************************************************************************
//  RELOAD PARAMETERS:
//  Runs when new parameters are loaded into the SystemParameters struct
//*****************************************************************************

void VisionModule::reloadParameters()
{
  if(sidesFlipped)
  {
    sidesFlipped = false;
    flipSides();  //reverse the field params and set sidesFlipped back to true;
  }
  
  //reload LATENCY_FRAMES, etc.
  InitializePrediction();
}


//*****************************************************************************
//  GET NEW DATA:
//  Receives new vision data from the Vision system.
//*****************************************************************************

void VisionModule::getNewData(Client* client)
{
  
  if (!receiveVisionData(client))
  {
    //something bad happened
    visionRunning = false;
  }
  
  // Add angle offsets to each robot
  incomingVision.team[sp->general.TEAM][ROBOT0].Rotation -= sp->general.ANGLE_OFFSET_ROBOT_ZERO;
  incomingVision.team[sp->general.TEAM][ROBOT1].Rotation -= sp->general.ANGLE_OFFSET_ROBOT_ONE;
  incomingVision.team[sp->general.TEAM][ROBOT2].Rotation -= sp->general.ANGLE_OFFSET_ROBOT_TWO;
  incomingVision.team[sp->general.TEAM][ROBOT3].Rotation -= sp->general.ANGLE_OFFSET_ROBOT_THREE;
  incomingVision.team[sp->general.TEAM][ROBOT4].Rotation -= sp->general.ANGLE_OFFSET_ROBOT_FOUR;
}

//*****************************************************************************
//  RECEIVE VISION DATA:
//  Receives new vision data from the Vision system.
//*****************************************************************************
bool VisionModule::receiveVisionData(Client* client)
{
  if (!client->receive(incomingPacket->getPacket(),incomingPacket->getTotalSize()) )
  {
    return false;
  }
  
  int i;
  bool found;
  float xpos, ypos, orientation;
  
  // convert everything to incomingVision
  // robot information
  for (i=0;i<VISION_PACKET_NUM_ROBOTS;i++)
  {
    //--    
    incomingPacket->getFriendlyRobot(i,found,xpos,ypos,orientation);
    incomingVision.team[0][i].XPos = xpos;
    incomingVision.team[0][i].YPos = ypos;
    incomingVision.robotsFound[0][i] = found;
    
    //keep from normalizing -32000
    if(!found || orientation < -1000.0f)
      orientation = 0.0f;
    else
      orientation = normalizeAngle(orientation);
    incomingVision.team[0][i].Rotation = orientation;
    
    //-- 
    
    incomingPacket->getOpponentRobot(i,found,xpos,ypos,orientation);
    incomingVision.team[1][i].XPos = xpos;
    incomingVision.team[1][i].YPos = ypos;
    incomingVision.robotsFound[1][i] = found;
    
    //keep from normalizing -32000
    if(!found || orientation < -1000.0f)
      orientation = 0.0f;
    else
      orientation = normalizeAngle(orientation);
    incomingVision.team[1][i].Rotation = orientation;
    //-- 
    
  }
  
  // ball information
  for (i=0;i<VISION_PACKET_NUM_BALLS;i++)
  {
    char source;
    
    incomingPacket->getBall(i,found,xpos,ypos,source);
    incomingVision.ball[0][i].XPos = xpos;
    incomingVision.ball[0][i].YPos = ypos;
    incomingVision.ball[0][i].Rotation = 0.00f;
    incomingVision.ballsFound[0][i] = found;
    incomingVision.ballSource[0][i] = source;
    
    incomingVision.ball[1][i].XPos  = xpos;
    incomingVision.ball[1][i].YPos  = ypos;
    incomingVision.ball[1][i].Rotation = 0.00f;
    incomingVision.ballsFound[1][i] = found;
    incomingVision.ballSource[1][i] = source;
  }
  
  //log ball if found
  if(incomingVision.ballsFound[0][0])
  {
    if(areSidesFlipped())
    {
      lastUnpredictedBall.set(Pair(-incomingVision.ball[0][0].XPos,
                                   -incomingVision.ball[0][0].YPos));
    }
    else
    {
      lastUnpredictedBall.set(Pair(incomingVision.ball[0][0].XPos,
                                   incomingVision.ball[0][0].YPos));
    }
  }
  
  // other information pertaining to this packet
  incomingVision.timeStamp    = incomingPacket->getTimeStamp();
  incomingVision.startTime    = incomingPacket->getStartTime();
  incomingVision.finishTime   = incomingPacket->getFinishTime();
  incomingVision.instruction  = incomingPacket->getInstruction();
  return true;
}

//*****************************************************************************
//  GET NO VISION DATA:
//  Runs when the vision is turned off
//*****************************************************************************
/**  Sets all objects to not found and sets their positions and velocities to BAD_PRED_ERR **/
void VisionModule::getNoVisionData()
{
  memset(&incomingVision, 0, sizeof(FieldData));

  for (int i = 0; i < NUM_PLAYERS_ON_TEAM; i++) 
  {
    //Our team
    if(GUI_Placement.friendlyRobotX[i] == BAD_PRED_ERR)
    {
      incomingVision.robotsFound[sp->general.TEAM][i] = false;
    }
    else
    {
      incomingVision.robotsFound[sp->general.TEAM][i] = true;
      incomingVision.team[sp->general.TEAM][i].XPos = GUI_Placement.friendlyRobotX[i];
      incomingVision.team[sp->general.TEAM][i].YPos = GUI_Placement.friendlyRobotY[i];
      incomingVision.team[sp->general.TEAM][i].Rotation = GUI_Placement.friendlyRobotRotation[i];
    }
    
    //Opposite team
    if(GUI_Placement.opponentRobotX[i] == BAD_PRED_ERR)
    {
      incomingVision.robotsFound[sp->general.OTHER_TEAM][i] = false;
    }
    else
    {
      incomingVision.robotsFound[sp->general.OTHER_TEAM][i] = true;        
      incomingVision.team[sp->general.TEAM][i].XPos = GUI_Placement.opponentRobotX[i];
      incomingVision.team[sp->general.TEAM][i].YPos = GUI_Placement.opponentRobotY[i];
      incomingVision.team[sp->general.TEAM][i].Rotation = GUI_Placement.opponentRobotRotation[i];
    }
  }
  
  for (i = 0; i < NUM_BALLS; i++) 
  {
    //Our team balls
    if(GUI_Placement.ballX[0][i] == BAD_PRED_ERR)
    {
      incomingVision.ballsFound[sp->general.TEAM][0]=false;
    }
    else
    {
      incomingVision.ballsFound[sp->general.TEAM][0]=true;
      incomingVision.ball[sp->general.TEAM][0].XPos = GUI_Placement.ballX[0][i];
      incomingVision.ball[sp->general.TEAM][0].YPos = GUI_Placement.ballY[0][i];
    }
  }
}

//*****************************************************************************
//  SAMPLE_VISION_SPEED
//
//  In order to accurately predict positions and build trajectories, we must
//  know how much time elapses between each vision "frame".  This function runs 
//  when the system loads and calculates how fast vision is running.
//*****************************************************************************

//Number of samples used to calculate vision speed
const int NUM_VISION_SAMPLES = 30;

//Samples the vision to see what speed it is running at, sets FRAME_RATE accordingly
void VisionModule::SampleVisionSpeed(Client* client)
{
  AIData outgoingAI;
  memset(&outgoingAI, 0, sizeof(outgoingAI)); 
  double time = 0.0f, totalTime=0.0f;
  double hold_vision_time[NUM_VISION_SAMPLES]; 
  int usedSamples = 0;
  LARGE_INTEGER start, finish, frequency;

  double ticks, result;
  bool visionResult;
  int t;
  
  double visionCurrent, visionFinish, visionDif, visionDifTotal=0.0f;
  
  //determines frequency ot the Ticks
  QueryPerformanceFrequency(&frequency);
  if(frequency.LowPart == 0) {
    printf("QueryPerformanceCounter not supported.  Aborting vision test.");
    return;
  }
  
  ticks= float(frequency.LowPart)/1000.0f;
  receiveVisionData(client);
  client->send(&outgoingAI, sizeof(outgoingAI));
  
  visionCurrent = incomingVision.finishTime;
  QueryPerformanceCounter(&start);
  
  for( t=0;  t < NUM_VISION_SAMPLES; t++){
    //takes the value of the counter before the execution
    //calls the vision function to time...
    visionResult = receiveVisionData(client);
    client->send(&outgoingAI, sizeof(outgoingAI));

    //takes the value of the counter after the execution
    QueryPerformanceCounter(&finish);
    
    if (!visionResult)
    {
      visionRunning = false;
      sp->general.FRAME_RATE = 0;
      return;
    }
    
    visionFinish = incomingVision.finishTime;
    
    visionDif = visionFinish - visionCurrent;
    visionDifTotal += visionDif;
    visionCurrent = visionFinish;
    
    result = (double)(finish.QuadPart - start.QuadPart) / ticks;
    start = finish;

    //    printf("time: %lf\n", result);
    if (result > 0.0f) {
      // it's a non-zero turn-around time
      //      hold_vision_time[t] = visionDif;
      hold_vision_time[t] = result;
    } else {
      t--; // re-run this trial since we got a zero time
    }
    
  }
  
  //Finds the average time of the NUM_VISION_SAMPLES vision samples taken
  for( t=0;  t < NUM_VISION_SAMPLES; t++) {
    totalTime+=hold_vision_time[t];
  }
  time=totalTime/(float)NUM_VISION_SAMPLES;
  sp->general.FRAME_RATE = (float)(1000.0f/time); 
}



//*****************************************************************************
//  FLIP VISION:
//
//  The AI assumes that we are moving offensively in the positive x-direction.
//  If we have to switch sides and play the other way, we simply rotate the
//  vision data 180 degrees.
//*****************************************************************************

void VisionModule::FlipVision()
{
  int i;
  for (i = 0; i < NUM_BALLS; i++)
  {
    incomingVision.ball[sp->general.TEAM][i].XPos = -incomingVision.ball[sp->general.TEAM][i].XPos;
    incomingVision.ball[sp->general.TEAM][i].YPos = -incomingVision.ball[sp->general.TEAM][i].YPos; 
    incomingVision.ball[sp->general.OTHER_TEAM][i].XPos = -incomingVision.ball[sp->general.OTHER_TEAM][i].XPos;   
    incomingVision.ball[sp->general.OTHER_TEAM][i].YPos = -incomingVision.ball[sp->general.OTHER_TEAM][i].YPos;
  }
  
  for (i = 0; i < NUM_PLAYERS_ON_TEAM; i++) 
  {
    incomingVision.team[sp->general.TEAM][i].XPos = -incomingVision.team[sp->general.TEAM][i].XPos;
    incomingVision.team[sp->general.TEAM][i].YPos = -incomingVision.team[sp->general.TEAM][i].YPos;
    incomingVision.team[sp->general.TEAM][i].Rotation = normalizeAngle(incomingVision.team[sp->general.TEAM][i].Rotation + PI);
    
    if (incomingVision.team[sp->general.TEAM][i].Rotation >= PI)      
    {
      incomingVision.team[sp->general.TEAM][i].Rotation -= 2*PI;
    }
    
    incomingVision.team[sp->general.OTHER_TEAM][i].XPos = -incomingVision.team[sp->general.OTHER_TEAM][i].XPos;
    incomingVision.team[sp->general.OTHER_TEAM][i].YPos = -incomingVision.team[sp->general.OTHER_TEAM][i].YPos;
    incomingVision.team[sp->general.OTHER_TEAM][i].Rotation = normalizeAngle(incomingVision.team[sp->general.OTHER_TEAM][i].Rotation + PI);
    
    if (incomingVision.team[sp->general.OTHER_TEAM][i].Rotation >= PI)
    {
      incomingVision.team[sp->general.OTHER_TEAM][i].Rotation -= 2*PI;      
    }    
  }
}

//*****************************************************************************
//  FLIP SIDES:
//  Swap the field params.
//*****************************************************************************

//e.g. swapAndNegate(-1.6, 1.7) returns (-1.7, 1.6)
__inline void swapAndNegate(float* x1, float* x2)
{
  static float temp;
  temp = *x1;
  *x1 = -(*x2);
  *x2 = -temp;
}

bool VisionModule::flipSides()
{
  //flip all sidelines and startegy lines
  sp->field.switchSides();
  
  sidesFlipped = !sidesFlipped;
  return sidesFlipped;
}

