/*
 *	latency_test.cpp
 */

#include "LED_latency_test.h"
#include "math/angle.h"
#include "../../main.h"
#include "info/ball/ball.h"
#include "../noplay.h"
#include "GUI/GUI.h"

Play LED_LatencyTest::playNum = -1;

LED_LatencyTest::LED_LatencyTest(VisionModule *vm) : BasePlay(vm)
{

}


void LED_LatencyTest::initializePlay(const VisionData& vision, RobocupStrategyData* rsd) 
{
	 //backup old # of lost frames
  ballLostFrames = rsd->getSystemParams().general.BALL_LOST_FRAMES;
  
	 //initialize the counter
	 counter = numberOfFramesforTest;
   
   //make ball appear lost right away during test
   rsd->getMutableSystemParams().general.BALL_LOST_FRAMES = 0;
}

void LED_LatencyTest::executePlay(VisionData* vision, RobocupStrategyData* rsd) 
{
  for(RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++)
  {
    //set all robots to stop
    stopRobot(i, this, *vision, rsd);
    
    //set dribblers
    if(isRobocupBallFound(*vision))
    {
      rsd->getDestination(i)->setDribble(NO_DRIBBLE);
    }
    else
    {
 	  rsd->getDestination(i)->setDribble(FAST_DRIBBLE);
    }
  }
  
  //decrement counter 
  counter--;
  
  //log data
  ballStatus[counter] = isRobocupBallFound(*vision);
  
  //break out of latency test when all done
  if( counter == 0)
  {
    //restore lost frames #
    rsd->getMutableSystemParams().general.BALL_LOST_FRAMES = ballLostFrames;
    
    //set play to no play
    rsd->setNewPlay(NoPlay::playNum);
    
    //finally, calculate the latency for the system and
    //set those values to print on the screen
    float sum=0;
    float num=0;
    float minLatency=100;
    float maxLatency=0;		
    float meanLatency;
    
    for(int i=0; i<numberOfFramesforTest; i++)
    {
      //record new ball status for first frame of a chunk
      bool status = ballStatus[i];
      int statusIndex = i;
      
      //keep moving forward until ball status changes
      while(
        (ballStatus[i] == status) &&
        (i < numberOfFramesforTest)
        )
      {
        i++;
      }
      
      //calculate number of frames for this set
      int numFrames = (i - statusIndex);
      
      //add to statistics
      //ignore first and last result...
  	  if(
	    	 (num != 0) && //not first result
         (i != numberOfFramesforTest)
		    )
      {
        sum+=((float)numFrames);
        num++;
        if(numFrames < minLatency)
          minLatency = (float)numFrames;
        if(numFrames > maxLatency)
          maxLatency = (float)numFrames;
      }
      else
      {
		    num++;
      }
    }

    //when all done, calculate mean and then post results
    meanLatency = sum/num;
    sm->setMinLatency(minLatency);
    sm->setMaxLatency(maxLatency);
    sm->setMeanLatency(meanLatency);
    sm->setLatencyTestDone(true);

    for(i=0; i<numberOfFramesforTest; i++)
    {
      if(ballStatus[i])
        GUI_Record.debuggingInfo.addDebugMessage("1");
      else
        GUI_Record.debuggingInfo.addDebugMessage("0");
    }   
  }
}
