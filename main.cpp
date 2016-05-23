#include "main.h"
#include "main_includes.h"
#include "GUI/GUI.h"
#include "GUI/GUI_Internal_Includes.h"
#include "visionTestGUI/visionTestGUI.h"
#include <time.h>

#define REF_BOX_PORT "COM2"

RoboCupModule* sm;
int main(int argc, char **argv) 
{
  if (argc < 2) 
  {
    sm = new RoboCupModule(NULL);
  }
  else 
  {
    // argv[1] will be checked for errors in the constructor:
    sm = new RoboCupModule(argv[1]);
  }

  //===============================  
  // get a frame of vision (or at least some dummy values if vision isn't running)
  // before launching the GUI
  cout << "-Starting AI, getting first frame from vision...\n";
  sm->executeFrame();
  //===============================  
  //manage and start GUI thread
  cout << "-Starting GUI, splitting off as separate thread...\n";
  DWORD dwThreadId; 
  HANDLE guiThread; 
  guiThread = CreateThread(NULL,           // no security attributes 
                           0,              // use default stack size  
                           launchGUI,      // thread function 
                           NULL,           // argument to thread function 
                           0,              // use default creation flags 
                           &dwThreadId);   // returns the thread identifier 



  // Check the return value for success. 

  if (guiThread == NULL) { cout << "Failed to spawn GUI thread!!!\n"; }

  //SetThreadPriority(guiThread, THREAD_PRIORITY_IDLE);
  //===============================  
  //initialize vision quality test
  initializeVisionQualityTestData();
  //==================================
  //create, intialize, and start the referee
  //box interface thread

  refBoxInterface = new RefBoxInterface();
  refBoxInterface->initialize(YELLOW, REF_BOX_PORT);
  refBoxInterface->start();
  
  //==================================================
  // Set Thread Priorities and CPU affinities
  DWORD ProcessAffinityMask;
  DWORD SystemAffinityMask;
  const DWORD DesiredProcessAffinityMask          = 3;
  const DWORD DesiredMainThreadAffinityMask       = 2;
  //==================================================
  // Set Process priority to REAL-TIME
  HANDLE MainSystemProcess = GetCurrentProcess();
  DWORD priority=sm->getParameters()->general.THREAD_PRIORITY;
  if (GetPriorityClass(MainSystemProcess) != priority)
  {
    if (SetPriorityClass(MainSystemProcess,priority) == 0)
    {
      printf("Cannot boost main the process priority to real-time system\n");
    }
  }
  
  // Get Process AFFINITY value and set it to dual mode system
  if (GetProcessAffinityMask(MainSystemProcess,&ProcessAffinityMask,&SystemAffinityMask) != 0)
  {
    if (SystemAffinityMask != 3)
    {
      printf("Cannot enable single or dual-processor system support\n");
    }
    else
    {
      if (ProcessAffinityMask != 3)
      {
        if (SetProcessAffinityMask(MainSystemProcess,DesiredProcessAffinityMask) == 0)
        {
          printf("Cannot set the main process affinity value to dual-processor mode\n");
        }
      }
    }
  }

  //==================================================
  // Set the main Vision Thread to TIME-CRITICAL THREAD
  HANDLE MainSystemThread = GetCurrentThread();
  if (GetThreadPriority(MainSystemThread) != THREAD_PRIORITY_TIME_CRITICAL)
  {
    if (SetThreadPriority(MainSystemThread,THREAD_PRIORITY_TIME_CRITICAL) == 0)
    {
      printf("Cannot boost the main thread priority to time-critical system\n");
    }
  }
  
  // Set the AFFINITY value for the main vision thread to CPU 1
  if (SetThreadAffinityMask(MainSystemThread,DesiredMainThreadAffinityMask) == 0)
  {
    printf("Cannot set the main thread affinity value to CPU 1\n");
  }
  //==================================================
  //Before we start process, if running a timing test, create scratch array and 
  //every time around main loop write down system time. After set number of frames, 
  //calculate max, min, and average loop time and print on screen
  #define TIMING_TEST_FRAMES 10
  bool TIMING = false;
  int t = -1;
  LARGE_INTEGER frequency;  
  float ticks = 0.0f;
  //determine the frequency of the Ticks
  QueryPerformanceFrequency(&frequency);
  #ifdef TIMING_TEST
  cout << "-Creating scratch array for main loop timing test...\n";
  TIMING = true;
  ticks= float(frequency.LowPart)/1000.0f;
  #endif
  //===============================  

  //immediately init all robots to have 2002 positions
  sm->Initialize2002Positions();

  while(Cornell_is_Superior) //aka forever...
  {
   //Reset debugging points
    resetDebuggingPoints();

    //Execute one frame
    sm->executeFrame();

    //copy over working debugging points to real debugging points
    copyDebuggingPoints();

    //Check for GUI commands and update values
    reactToAICommands();

    //Check for Referee commands and update values
    reactToRefereeCommands();

    //Update vision quality information
    updateVisionQualityData();    
  }

  //----------------------------
  //Note:  Tcl kills the entire process, so this code won't be executed.
  //----------------------------
  //Stop GUI thread
  #ifdef USING_GUI
  CloseHandle( guiThread );
  #endif

  //Delete the referee ox interace
  delete refBoxInterface;
  //---------------------------- 
  delete sm;
  return 0;
 }
//===================================================================================
//===================================================================================
//resets debugging points so they are not displayed unless plays during
//this frame actually set them again
void resetDebuggingPoints()
{
  GUI_Record.debuggingInfo.reset();
}
//===================================================================================
//===================================================================================
//copies working debugging points to real debugging points. prevents flickering.
void copyDebuggingPoints()
{
  GUI_Record.debuggingInfo.syncPoints();
} 
//===================================================================================
//===================================================================================
//This method checks for any changes the GUI is instructing (like toggling AI status)
//and changed them. it is called from the main method rigth AFTER execute frame
void reactToAICommands()
{
  if(GUI_Record.newPlay != INVALID_PLAY) sm->setPlay(GUI_Record.newPlay);
  if(GUI_Record.toggleAI) sm->ToggleAI();
  if(GUI_Record.toggleVision) sm->ToggleVision();
  if(GUI_Record.toggleWireless) sm->ToggleWireless();
  if(GUI_Record.toggleSides) sm->TogglePlayingSide();
  if(GUI_Record.toggleTeam) sm->ToggleTeamNumber();
  if (GUI_Record.sampleFrameRate) sm->sampleFrameRate();

  //All done looking at GUI commands, reset variables 
  GUI_Record.reset();  
}
//===================================================================================
//===================================================================================
//Check for referee commands
void reactToRefereeCommands()
{
  //get first command off the queue
  refBoxCommand newCommand = refBoxInterface->getEvent();

  //handle it
  switch(newCommand)
  {
  //------------------
  case EMPTY:
    break;
  //------------------
  case START:
    sm->SetAI(true);
    break;
  //------------------
  case STOP:
    sm->SetAI(false);
    break;
  //------------------
  case BEGIN_FIRST_HALF:
    break;
  case BEGIN_HALF_TIME:
    break;
  case BEGIN_SECOND_HALF:
    break;
  case OVERTIME:
    break;
  case PENALTY_SHOOTOUT:
    break;
  case NEUTRAL_RESTART:
    break;
  case KICK_OFF_US:
    break;
  case KICK_OFF_THEM:
    break;
  case PENALTY_US:
    break;
  case PENALTY_THEM:
    break;
  case FREE_KICK_US:
    break;
  case FREE_KICK_THEM:
    break;
  case GOAL_US:
    break;
  case GOAL_THEM:
    break;
  case CONNECTED:
    break;
  default:
    cout << "got bogus command from, what gives? command: " << newCommand << endl;
    break;
  }
  //------------------
}
//===================================================================================
//===================================================================================


