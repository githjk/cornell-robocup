/*
 * hires_timer.cpp
 */

#include "hires_timer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

Timer::Timer(){
	//LARGE_INTEGERs and __int64s are interchangable
	QueryPerformanceFrequency( (LARGE_INTEGER *) &freq );
	//The function using Timer must check the frequency for 0;

	//initialize these
	QueryPerformanceCounter( (LARGE_INTEGER *) &startTime );
	QueryPerformanceCounter( (LARGE_INTEGER *) &endTime );
	QueryPerformanceCounter( (LARGE_INTEGER *) &lastLoopBeginTime );
}

void Timer::markStartTime()
{
	QueryPerformanceCounter( (LARGE_INTEGER *) &startTime );
}

void Timer::markEndTime()
{
	QueryPerformanceCounter( (LARGE_INTEGER *) &endTime );
}

// after markStartTime and markEndTime have been called,
// you can call getInterval to get the elapsed time;
double Timer::getInterval() const
{
	return (double(endTime - startTime)) / freq;
}
double Timer::getInterval_ms() const
{
	return getInterval() * 1000.0;
}

double Timer::getElapsedTime() const
{
	__int64 currentTime;
	QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
	return (double(currentTime - startTime)) / freq;
}

double Timer::getElapsedAndReset() const
{
	__int64 lastStartTime = startTime;
	QueryPerformanceCounter( (LARGE_INTEGER *) &startTime );
	return (double(startTime - lastStartTime)) / freq;
}

	
//delays the current thread by "interval" milliseconds
// from the current time
void Timer::delay_ms(double interval) const
{
	__int64 beginTime, currentTime;
	QueryPerformanceCounter( (LARGE_INTEGER *) &beginTime );
	QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );

  double timeSoFar_ms = (double(currentTime - beginTime)) * 1000 / freq;
  if (timeSoFar_ms < (interval*.8 - 10)) {
    //Sleep conservatively:  if we'll be waiting for a while, let other processes do stuff;
    // but when we get close to the deadline wake up and start spinning so we get accurate performance.
    Sleep( (int)(interval*.8 - 10 - timeSoFar_ms) );
  }

	__int64 intervalInTicks = __int64(freq * interval / 1000);
	do {
		QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
	} while ( (currentTime - beginTime) <= intervalInTicks );

}
	
//delays the processor until at least "interval" ms have elapsed since the
// last call to ensureLoopInterval_ms or since the Timer was initialized
//
//Returns the amount of time (in ms) between the start of this call and the
// end of the previous  (i.e. the amount of time spent doing useful work
// between calls to ensureLoopInterval_ms).
double Timer::ensureLoopInterval_ms(double interval)
{
	__int64 currentTime;
	QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
  double timeSinceLastCall = (double(currentTime - lastLoopBeginTime)) * 1000 / freq;

  //====== Let other processes do stuff ======
  double timeSoFar_ms = (double(currentTime - lastLoopBeginTime)) * 1000 / freq;
  if (timeSoFar_ms < (interval*.8 - 10)) {
    //Sleep conservatively:  if we'll be waiting for a while, let other processes do stuff;
    // but when we get close to the deadline wake up and start spinning so we get accurate performance.
    Sleep( (int)(interval*.8 - 10 - timeSoFar_ms) );
  }

  //====== Now spin until time is up. ========
	__int64 intervalInTicks = __int64(freq * interval / 1000);
  do {
  	QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
	}	while ( (currentTime - lastLoopBeginTime) <= intervalInTicks );

	lastLoopBeginTime = currentTime;
  return timeSinceLastCall;
}
	
