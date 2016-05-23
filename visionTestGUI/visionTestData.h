//---------------------

//Basic visionTestData_DEF declarations
//---------------------
#ifndef visionTestData_DEF
#define visionTestData_DEF

//==================================
//vision quality information

#define visionQualityNumberFramesAnalized 30
  
//max difference between two frames
#define visionQualityRotationThreshhold 10
  
//max difference between two x or y positions
#define visionQualityPositionThreshhold 0.50 //~30 degrees


struct dataQuality
{    
  //are we currently running such a test? helps
  //keep track if we should create/kill a thread if toggle clicked
  bool runningVisionQualityTest;
  
  //just keep looping around by incrementing the frame counter
  int visionQualityFrame;
  
  //was robot found in teh last frame?
  bool friendlyRobotFound[NUM_ROBOTS][visionQualityNumberFramesAnalized];
  
  //did a friendly robot have a stable position between frames n and n-1?
  bool friendlyRobotStablePosition[NUM_ROBOTS][visionQualityNumberFramesAnalized];
  
  //did a friendly robot have a stable rotation between frames n and n-1?
  bool friendlyRobotStableRotation[NUM_ROBOTS][visionQualityNumberFramesAnalized];
  
  //did a opponent robot have a stable position between frames n and n-1?
  bool opponentRobotFound[NUM_ROBOTS][visionQualityNumberFramesAnalized];
  
  //did a opponent robot have a stable position between frames n and n-1?
  bool opponentRobotStablePosition[NUM_ROBOTS][visionQualityNumberFramesAnalized];
  
  //was the ball found?
  bool ballFound[visionQualityNumberFramesAnalized];
  
  //did the ball have a stable position between frames n and n-1?
  bool ballStablePosition[visionQualityNumberFramesAnalized];
  //==================================
};


#endif //visionTestGUI_DEF
