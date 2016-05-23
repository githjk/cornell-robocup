#include "VisionTimer.h"

//===============================================
///Basic constructor. Calls reset.
VisionTimer::VisionTimer(VisionModule* visionHandle)
{
  vision = visionHandle;
  resetTimer();
}
//===============================================
///Basic destructor. does nothing for now.
VisionTimer::~VisionTimer()
{}
//===============================================
///Call this function to reset the timer to 0.
void VisionTimer::resetTimer()
{
  //As the 2 times are the same, we only use one of them
  //I decided to use the finish time.  If you don't like it, tough.
  time =  vision->getCurrentFrame()->getFinishTime();

}
//===============================================
///Returns current time
double VisionTimer::currentTime()
{
  return vision->getCurrentFrame()->getFinishTime()-time;
}
//===============================================

