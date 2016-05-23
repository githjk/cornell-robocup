////////////////////////////////////////////////////////////////////
//  VisionModule.CPP
//
//  This file contains all the functions necessary for processing
//  raw vision data 
//
////////////////////////////////////////////////////////////////////

#include "VisionModule.h"

//=========================================================================
// CONSTANTS
//=========================================================================
const float TRANSLATIONAL_ERROR_NOISE_CONSTANT  = 0.03f;  // meters
const float ROTATIONAL_ERROR_NOISE_CONSTANT     = 0.10f;   // radians

const float MAX_TRANSLATIONAL_ERROR             = 1.0f; //1.0f;
const float MAX_ROTATIONAL_ERROR                = 0.10f;

const int MAX_TRANSLATIONAL_ERROR_COUNT         = 3; //10;
const int MAX_ROTATIONAL_ERROR_COUNT            = 3; //10;

const int NUM_FRAMES_TO_AVERAGE                 = 3;

//=========================================================================
// This function saves the raw vision data and uses it to determine how
//    well the robots are following our commands.
//=========================================================================
void VisionModule::updateRawVision()
{
  loadRawVision();
  calculatePredictionError();
  evaluatePredictionError();
}


//=========================================================================
// This function loads the incoming vision into rawVisionData[currentFrame]
//=========================================================================
void VisionModule::loadRawVision()
{
  int i,j;
  int tempFrame;

  for (i=0;i<NUM_PLAYERS_ON_TEAM;i++)
  {
    for (j=0;j<NUM_TEAMS;j++) 
    {
      tempFrame = (currentFrame + MAX_LOG - NUM_FRAMES_TO_AVERAGE) % MAX_LOG;

      rawVisionData[currentFrame].teams[j][i].setXPos(incomingVision.team[j][i].XPos);
      rawVisionData[currentFrame].teams[j][i].setYPos(incomingVision.team[j][i].YPos);
      rawVisionData[currentFrame].teams[j][i].setRotation(incomingVision.team[j][i].Rotation);
      rawVisionData[currentFrame].robotsFound[j][i] = incomingVision.robotsFound[j][i];
    
      //-----------------------------------------------------------------------------
      // Calculate average velocity over the past NUM_FRAMES_TO_AVERAGE frames
      if (rawVisionData[currentFrame].robotsFound[j][i] &&
          rawVisionData[tempFrame].robotsFound[j][i]
      )
      {
        rawVisionData[currentFrame].teams[j][i].setXVel(
          (rawVisionData[currentFrame].teams[j][i].getXPos() - rawVisionData[tempFrame].teams[j][i].getXPos() )
          * sp->general.FRAME_RATE / NUM_FRAMES_TO_AVERAGE
        );
        rawVisionData[currentFrame].teams[j][i].setYVel(
          (rawVisionData[currentFrame].teams[j][i].getYPos() - rawVisionData[tempFrame].teams[j][i].getYPos() )
          * sp->general.FRAME_RATE / NUM_FRAMES_TO_AVERAGE
        );

        rawVisionData[currentFrame].teams[j][i].setRotVel(
          normalizeAngle(rawVisionData[currentFrame].teams[j][i].getRotation() - rawVisionData[tempFrame].teams[j][i].getRotation() )
          * sp->general.FRAME_RATE / NUM_FRAMES_TO_AVERAGE
        );
      }
      
      //-----------------------------------------------------------------------------
      // If one of the two frames is lost, then just use the previous frame's 
      //    calculated average velocities.
      else {

        tempFrame = (currentFrame + MAX_LOG - 1) % MAX_LOG;

        rawVisionData[currentFrame].teams[j][i].setXVel  (rawVisionData[tempFrame].teams[j][i].getXVel());
        rawVisionData[currentFrame].teams[j][i].setYVel  (rawVisionData[tempFrame].teams[j][i].getYVel());
        rawVisionData[currentFrame].teams[j][i].setRotVel(rawVisionData[tempFrame].teams[j][i].getRotVel());
      }
    }
  }

  for (i=0;i<NUM_PLAYERS_ON_TEAM;i++)
  {
    // Initialize prediction error values
    rawVisionData[currentFrame].resetTranslationalVelocity[i] = false;
    rawVisionData[currentFrame].resetRotationalVelocity[i]    = false;

    rawVisionData[currentFrame].distanceTravelled[i] = 0.0f;
    rawVisionData[currentFrame].rotationTravelled[i] = 0.0f;
    
    rawVisionData[currentFrame].predictedTranslationalError[i]  = 0.0f;
    rawVisionData[currentFrame].predictedRotationalError[i]     = 0.0f;

    rawVisionData[currentFrame].normalizedTranslationalError[i] = 0.0f;
    rawVisionData[currentFrame].normalizedRotationalError[i]    = 0.0f;
  }
  
  // ball information
  for (i=0;i<NUM_BALLS;i++)
  {
    rawVisionData[currentFrame].ball[0][i].setXPos(incomingVision.ball[0][i].XPos);
    rawVisionData[currentFrame].ball[0][i].setYPos(incomingVision.ball[0][i].YPos);
    rawVisionData[currentFrame].ballsFound[0][i] = incomingVision.ballsFound[0][i];
    rawVisionData[currentFrame].ballSource[0][i] = incomingVision.ballSource[0][i];
    
    rawVisionData[currentFrame].ball[1][i].setXPos(incomingVision.ball[1][i].XPos);
    rawVisionData[currentFrame].ball[1][i].setYPos(incomingVision.ball[1][i].YPos);
    rawVisionData[currentFrame].ballsFound[1][i] = incomingVision.ballsFound[1][i];
    rawVisionData[currentFrame].ballSource[1][i] = incomingVision.ballSource[1][i];
  }

  rawVisionData[currentFrame].validData = true;
}

//=========================================================================
// This function calculates the values necessary for determining the
//    normalized prediction error for translation and rotation
//=========================================================================
void VisionModule::calculatePredictionError()
{
  int predictedFrame, tempFrame;

  int count;
  float xRaw, yRaw, thetaRaw;
  float xPos, yPos, theta;

  // Calculate values for each robot
  for (int i=0;i<NUM_PLAYERS_ON_TEAM;i++)
  {

    // Is the robot found?
    if (rawVisionData[currentFrame].robotsFound[sp->general.TEAM][i] ) {

      xRaw     = rawVisionData[currentFrame].teams[sp->general.TEAM][i].getXPos();
      yRaw     = rawVisionData[currentFrame].teams[sp->general.TEAM][i].getYPos();
      thetaRaw = rawVisionData[currentFrame].teams[sp->general.TEAM][i].getRotation();

      //------------------------------------------------------------------------
      // Calculate distance travelled between this frame and the previous frame.  
      //    - If the robot was lost during the previous frame, keep stepping
      //        back up to LATENCY_FRAMES
      count = 1;
      while (count < (int)sp->general.LATENCY_FRAMES) {

        tempFrame = (currentFrame + MAX_LOG - count) % MAX_LOG;
        if ( rawVisionData[tempFrame].validData && 
             rawVisionData[tempFrame].robotsFound[0][i] 
           )
        {
          xPos   = rawVisionData[tempFrame].teams[sp->general.TEAM][i].getXPos();
          yPos   = rawVisionData[tempFrame].teams[sp->general.TEAM][i].getYPos();
          theta  = rawVisionData[tempFrame].teams[sp->general.TEAM][i].getRotation();

          rawVisionData[currentFrame].distanceTravelled[i] = (float)sqrt( (xPos-xRaw)*(xPos-xRaw) + (yPos-yRaw)*(yPos-yRaw) );
          rawVisionData[currentFrame].rotationTravelled[i] = ABS( theta - thetaRaw );
          count = (int)sp->general.LATENCY_FRAMES + 1;
        }
        else {
          count++;
        }
      } // while (count < sp->general.LATENCY_FRAMES)

      //------------------------------------------------------------------------
      // Calculate error between raw data and the previous predicted frame.  
      predictedFrame = (currentFrame + MAX_LOG - (int)sp->general.LATENCY_FRAMES) % MAX_LOG;
      
      if ( visionData[predictedFrame].robotsFound[sp->general.TEAM][i] ) 
      {
        xPos   = visionData[predictedFrame].teams[sp->general.TEAM][i].getXPos();
        yPos   = visionData[predictedFrame].teams[sp->general.TEAM][i].getYPos();
        theta  = visionData[predictedFrame].teams[sp->general.TEAM][i].getRotation();

        rawVisionData[currentFrame].predictedTranslationalError[i]  = (float)sqrt( (xPos-xRaw)*(xPos-xRaw) + (yPos-yRaw)*(yPos-yRaw) );
        rawVisionData[currentFrame].predictedRotationalError[i]     = ABS( theta - thetaRaw );
      }

      //------------------------------------------------------------------------
      // Update path length
      translationalPathLength[i]  += rawVisionData[currentFrame].distanceTravelled[i];
         rotationalPathLength[i]  += rawVisionData[currentFrame].rotationTravelled[i];

      if ( rawVisionData[predictedFrame].validData ) 
      {
        translationalPathLength[i]  -= rawVisionData[predictedFrame].distanceTravelled[i];
           rotationalPathLength[i]  -= rawVisionData[predictedFrame].rotationTravelled[i];
      }

      //------------------------------------------------------------------------
      // Calculate normalized error
      //    - NormalizeError = ( Error / ERROR_NOISE_CONSTANT + totalPathLength );

      rawVisionData[currentFrame].normalizedTranslationalError[i] = 
          ( rawVisionData[currentFrame].predictedTranslationalError[i] ) / 
          ( TRANSLATIONAL_ERROR_NOISE_CONSTANT + translationalPathLength[i] );
      
      rawVisionData[currentFrame].normalizedRotationalError[i] = 
          ( rawVisionData[currentFrame].predictedRotationalError[i] ) / 
          ( ROTATIONAL_ERROR_NOISE_CONSTANT + rotationalPathLength[i] );

    } // if (rawVisionData[currentFrame].robotsFound[sp->general.TEAM][i] ) 
  }
}

//=========================================================================
// This function looks at the normalized error of each robot and determines
//    if the commanded velocities are being followed or not.  If not, the
//    algorithm resets the velocities used during prediction.
//=========================================================================

#define RECALCULATE_TRANSLATIONAL_VELOCITIES

void VisionModule::evaluatePredictionError()
{

  int i,j;
  int tempFrame;
  float xVel, yVel, rotVel;
 
#ifdef RECALCULATE_TRANSLATIONAL_VELOCITIES
  float xVelPrime, yVelPrime;
  float rotation, rotationPrime;

  float cosTheta, cosThetaPrime;
  float sinTheta, sinThetaPrime;
  float cCsS, cSCs;
#endif

  // Calculate values for each robot
  for (i=0; i<NUM_PLAYERS_ON_TEAM; i++)
  {

    xVel    = rawVisionData[currentFrame].teams[sp->general.TEAM][i].getXVel();
    yVel    = rawVisionData[currentFrame].teams[sp->general.TEAM][i].getYVel();
    rotVel  = rawVisionData[currentFrame].teams[sp->general.TEAM][i].getRotVel();


    //------------------------------------------------------------------------
    // Update error counts
    if ( rawVisionData[currentFrame].normalizedTranslationalError[i] > MAX_TRANSLATIONAL_ERROR ) {
      translationalErrorCount[i]++;
    }
    else if (translationalErrorCount[i] > 0) {
      translationalErrorCount[i]--;
    }

    if ( rawVisionData[currentFrame].normalizedRotationalError[i] > MAX_ROTATIONAL_ERROR ) {
      rotationalErrorCount[i]++;
    }
    else if (translationalErrorCount[i] > 0) {
      rotationalErrorCount[i]--;
    }

    //------------------------------------------------------------------------
    // If the error count exceeds LATENCY_FRAMES, reset the velocities for
    //    the past LATENCY_FRAMES of commands
    // Then reset the error count

    if ( rotationalErrorCount[i] > MAX_ROTATIONAL_ERROR_COUNT ) 
    {

      tempFrame = (currentFrame + MAX_LOG - (int)sp->general.LATENCY_FRAMES) % MAX_LOG;

#ifdef RECALCULATE_TRANSLATIONAL_VELOCITIES
      rotationPrime = rawVisionData[tempFrame].teams[sp->general.TEAM][i].getRotation();
#endif

      // Reset velocities for the past LATENCY_FRAMES
      for (j=(int)sp->general.LATENCY_FRAMES; j >= 1; j--) {

        tempFrame = (currentFrame + MAX_LOG - j) % MAX_LOG;
        rawVisionData[tempFrame].resetRotationalVelocity[i] = true;
        rawVisionData[tempFrame].teams[sp->general.TEAM][i].setRotVel(rotVel);
//        rawVisionData[tempFrame].teams[sp->general.TEAM][i].setRotVel(0.0f);

#ifdef RECALCULATE_TRANSLATIONAL_VELOCITIES
        // If the rotational velocities were being predicted improperly, then the translational
        // velocities will be predicted wrong as well.  This code should readjust those velocities
        // to account for the reset predicted rotations.
        rawVisionData[tempFrame].resetTranslationalVelocity[i] = true;

        // get predicted rotation
        rotation  = visionData[tempFrame].teams[sp->general.TEAM][i].getRotation();
        // get commanded velocities
        xVel      = visionData[tempFrame].teams[sp->general.TEAM][i].getXVel();
        yVel      = visionData[tempFrame].teams[sp->general.TEAM][i].getYVel();

        // [ Vx' ]  =  [ cos' -sin' ] [  cos  sin ]  [ Vx ]
        // [ Vy' ]  =  [ sin'  cos' ] [ -sin  cos ]  [ Vy ]

        // calculate sines and cosines for rotations
        cosTheta      = (float)cos(rotation);
        sinTheta      = (float)sin(rotation);
        cosThetaPrime = (float)cos(rotationPrime);
        sinThetaPrime = (float)sin(rotationPrime);

        // These variables could probably be named better, but they represent
        // the expanded version of the matrix calculations
        cCsS      = (cosTheta*cosThetaPrime) + (sinThetaPrime*sinTheta);
        cSCs      = (cosTheta*sinThetaPrime) - (cosThetaPrime*sinTheta);

        xVelPrime = cCsS*xVel - cSCs*yVel;
        yVelPrime = cSCs*xVel + cCsS*yVel;

        rawVisionData[tempFrame].teams[sp->general.TEAM][i].setXVel(xVelPrime);
        rawVisionData[tempFrame].teams[sp->general.TEAM][i].setYVel(yVelPrime);

        // Predict the next rotation
        rotationPrime = rotationPrime + (rotVel / sp->general.FRAME_RATE);

#endif

      }
      rotationalErrorCount[i] = 0;
    }
    else if ( translationalErrorCount[i] > MAX_TRANSLATIONAL_ERROR_COUNT) 
    {
  
      // Reset velocities for the past LATENCY_FRAMES
      for (j=(int)sp->general.LATENCY_FRAMES; j >= 1; j--) {
        
        tempFrame = (currentFrame + MAX_LOG - j) % MAX_LOG;
        rawVisionData[tempFrame].resetTranslationalVelocity[i] = true;
        rawVisionData[tempFrame].teams[sp->general.TEAM][i].setXVel(xVel);
        rawVisionData[tempFrame].teams[sp->general.TEAM][i].setYVel(yVel);
      }
      translationalErrorCount[i] = 0;
    }


  }
}


//****************************************************************************


