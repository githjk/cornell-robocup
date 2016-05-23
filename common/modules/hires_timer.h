/*****************************************
*   hires_timer.h                        *
*                                        *
* A high-precision timer class based on  *
* QueryPerformanceCounter().             *
*                                        *
*****************************************/

#ifndef HIRES_TIMER_H
#define HIRES_TIMER_H

class Timer {
public:
	Timer();
	
	//call this at least once before using the timer.  I'm pretty sure
	// all x86 machines support QueryPerformanceCounter(), but no guarantees
	inline bool isTimerSupported() const { return freq != 0; }
	
	// after markStartTime and markEndTime have been called,
	// you can call getInterval to get the elapsed time;
	void markStartTime();
	void markEndTime();

  //getInterval() retreives the time bewteen calls to markStartTime and markEndTime,
	double getInterval() const;	  //seconds
	double getInterval_ms() const;//milliseconds

	//getElapsedTime() returns the amount of time that has passed since the last call to markStartTime.
	double getElapsedTime() const;// seconds
	
	//like above, but also resets the start time
	double getElapsedAndReset() const;// milliseconds
	
	//
	// Delay routines:
	//

	//delays the current thread by "interval" milliseconds
	// from the current time.
	void delay_ms(double interval) const; 
	

	//call this during a loop (e.g. the gamepad)
	// to ensure that the loop will be executed exactly once every 
	// "interval" milliseconds.
	//The first time this is called, it will delay until at least
	// "interval" ms have elapsed since the Timer was constructed.
  //
  //Returns the amount of time (in ms) between the start of this call and the
  // end of the previous  (i.e. the amount of time spent doing useful work
  // between calls to ensureLoopInterval_ms).
	double ensureLoopInterval_ms(double interval); 
	
private:
	//__int64 is a 64-bit signed integer in VC++
	__int64 freq;
	__int64 startTime, endTime;
	__int64 lastLoopBeginTime;
};


#endif //HIRES_TIMER_H