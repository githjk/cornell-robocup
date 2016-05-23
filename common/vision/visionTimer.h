#ifndef VISION_TIMER_H
#define VISION_TIMER_H

#include "VisionModule.h"

class VisionTimer
{
public:
  /**@name    public functions  */
  //@{

  ///Basic constructor. Calls reset.
  VisionTimer(VisionModule* visionHandle);

  ///Basic destructor. does nothing for now.
  ~VisionTimer();

  ///Call this function to reset the timer to 0.
  void resetTimer();
  
  ///Returns current time
  double currentTime();

  //}@

protected:
  
  //actual timer count
  double time;
  
  //pointer to the visionmodule, so we can grab a hold on the time
  //that has passed by based on the vision system's clock
  VisionModule* vision;
};
//getStartTime

#endif 
