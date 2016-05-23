
//Declare visionTestGUI information before defining it
#include "visionTestGUI.h"

//Actual variables in which we'll store vision quality data
#include "visionTestData.h"

//actual set of data
dataQuality visionQuality;

//---------------------------------
//Initialize VisionQualityTest Data
//---------------------------------
void initializeVisionQualityTestData()
{
  //start logging vision quality on frame 0
  visionQuality.visionQualityFrame = 0;

  //by default all values are false until stability if found
  for(int j=0; j<visionQualityNumberFramesAnalized; j++)
  {
    visionQuality.ballFound[j] = false;
    visionQuality.ballStablePosition[j] = false;

    for(int i=0; i<NUM_ROBOTS; i++)
    {
      visionQuality.friendlyRobotFound[i][j] = false;
      visionQuality.friendlyRobotStableRotation[i][j] = false;
      visionQuality.friendlyRobotStablePosition[i][j] = false;
      visionQuality.opponentRobotFound[i][j] = false;
      visionQuality.opponentRobotStablePosition[i][j] = false;
    }
  }
}

//----------------------------------------
//Update vision quality data
//----------------------------------------
void updateVisionQualityData()
{
  //update variables for vision quality test
  for(int i=0; i<NUM_ROBOTS; i++)
  {
    //-------------------------------
    //record if our robot was found
    visionQuality.friendlyRobotFound[i][visionQuality.visionQualityFrame] = sm->OurRobotFound(i);
    //-------------------------------
     //was our robots position stable?   
    if( 
       ABS(
           sm->GetOurRobotX(i, sm->ReturnFrame()) - 
           sm->GetOurRobotX(i, sm->ReturnFrame() - 1)
          ) > visionQualityPositionThreshhold
       ||
       ABS(
           sm->GetOurRobotY(i, sm->ReturnFrame()) - 
           sm->GetOurRobotY(i, sm->ReturnFrame() - 1)
          ) > visionQualityPositionThreshhold
      )
      visionQuality.friendlyRobotStablePosition[i][visionQuality.visionQualityFrame] = false;
    else
      visionQuality.friendlyRobotStablePosition[i][visionQuality.visionQualityFrame] = true;;
    //-------------------------------
    //was our robots rotation stable?   
    if( 
       ABS(
           sm->GetOurRobotRotation(i, sm->ReturnFrame()) - 
           sm->GetOurRobotRotation(i, sm->ReturnFrame() - 1)
          ) > visionQualityRotationThreshhold
      )
      visionQuality.friendlyRobotStableRotation[i][visionQuality.visionQualityFrame] = false;
    else
      visionQuality.friendlyRobotStableRotation[i][visionQuality.visionQualityFrame] = true;;
    //-------------------------------  
    //record if their robot was found
    visionQuality.opponentRobotFound[i][visionQuality.visionQualityFrame] = sm->TheirRobotFound(i);
    //-------------------------------
     //was opponent robots position stable?   
    if( 
       ABS(
           sm->GetTheirRobotX(i, sm->ReturnFrame()) - 
           sm->GetTheirRobotX(i, sm->ReturnFrame() - 1)
          ) > visionQualityPositionThreshhold
       ||
       ABS(
           sm->GetTheirRobotY(i, sm->ReturnFrame()) - 
           sm->GetTheirRobotY(i, sm->ReturnFrame() - 1)
          ) > visionQualityPositionThreshhold
      )
      visionQuality.opponentRobotStablePosition[i][visionQuality.visionQualityFrame] = false;
    else
      visionQuality.opponentRobotStablePosition[i][visionQuality.visionQualityFrame] = true;;
    //-------------------------------
    //was the ball found?
    visionQuality.ballFound[visionQuality.visionQualityFrame] = sm->RoboCupBallFound();
    //-------------------------------  
    //was the balls position stable?   
    if( 
       ABS(
           sm->GetBallX(sm->ReturnFrame()) - 
           sm->GetBallX(sm->ReturnFrame() - 1)
          ) > visionQualityPositionThreshhold
       ||
       ABS(
           sm->GetBallY(sm->ReturnFrame()) - 
           sm->GetBallY(sm->ReturnFrame() - 1)
          ) > visionQualityPositionThreshhold
      )
      visionQuality.ballStablePosition[visionQuality.visionQualityFrame] = false;
    else
      visionQuality.ballStablePosition[visionQuality.visionQualityFrame] = true;;
    //-------------------------------
  }

  //increment to next frame, loop if necessary
  visionQuality.visionQualityFrame++;

  if(visionQuality.visionQualityFrame == visionQualityNumberFramesAnalized)
    visionQuality.visionQualityFrame = 0;

}

