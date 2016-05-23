// RefBoxInterface.

#include "RefBoxInterface.h"
#include <iostream>
#include <stdio.h>

// Constructor
RefBoxInterface::RefBoxInterface() {
  data = (char *)malloc (4242);
  running = false;
}

// Destructor.
RefBoxInterface::~RefBoxInterface() {
  free(data);
  
}

// Initialize method.  This sets up the serial connection and team information.  Serial Connection parameters can be specified here.
int RefBoxInterface::initialize(teamColor color, //must supply this
                                char* portName,  //must supply this
                                int baudRate, 
                                int byteSize, 
                                int parity, 
                                int stopBits) 
{
  
  // Set color and open the serial connection.
  team = color;
  int errorCode = 0;
  
  errorCode = SPort.OpenConnection(portName, baudRate, byteSize, parity, stopBits);

  if (errorCode)
    cout << "ERROR OPENING COMPORT" << endl;
  else
    cout << "Listening for Ref Commands on " << portName << endl;    
  return errorCode;
}


DWORD WINAPI refListenerProc(LPVOID refBoxInt) {
  RefBoxInterface* theRB = (RefBoxInterface*) refBoxInt;
  theRB->loop();
  return true;
}

// This starts the thread that listens to the serial port for roboref signals.
int RefBoxInterface::start() {
  if (!running) {
    running = true;
    HANDLE listenerThread;
    DWORD dwThreadID;
    listenerThread = CreateThread(
                                  NULL,             //No security stuff
                                  0,                //Default stack size
                                  refListenerProc,     //Listening procedure, calls loop()
                                  this,             //So that listening procedure knows what object to use
                                  0,                //Use default creation flags
                                  &dwThreadID);      //Returns thread identifier, not used
    return (listenerThread != 0);
  }
  return -1;
}

// This is the main processing loop that filters through the received input from 
// refbox and stores each input into a queue.
void RefBoxInterface::loop() {

  long time = 0;
  
  int errorCode;
  
  while(running) {

    Sleep(1);
    errorCode = SPort.ReceiveData(data,1,&len);
//      if (errorCode != S_INTERFACE_DATA_RECEIVED)
//        printf("ERROR: %d\n", errorCode);
      if (len > 0) {
          
        // Filter through data.
        if (1 != len) {
          cout << "Received extra data (length = " << len << ")" << endl;
        }
        else{
          RefBoxInfo newInfo;
          switch (*data) {
          case 's':
            newInfo.command = START;
            //cout << "START: " << *data << endl;
            break;
            
          case 'S':
            newInfo.command = STOP;
            //cout << "STOP: " << *data << endl;
            break;
            
          case '1':
            newInfo.command = BEGIN_FIRST_HALF;
            //cout << "BEGIN_FIRST_HALF: " << *data << endl;
            break;
            
          case 'h':
            newInfo.command = BEGIN_HALF_TIME;
            //cout << "BEGIN_HALF_TIME: " << *data << endl;
            break;
            
          case '2':
            newInfo.command = BEGIN_SECOND_HALF;
            //cout << "BEGIN_SECOND_HALF: " << *data << endl;
            break;
            
          case 'o':
            newInfo.command = OVERTIME;
            //cout << "OVERTIME: " << *data << endl;
            break;
            
          case 'a':
            newInfo.command = PENALTY_SHOOTOUT;
            //cout << "PENALTY_SHOOTOUT: " << *data << endl;
            break;
            
          case 'r':
            newInfo.command = NEUTRAL_RESTART;
            //cout << "NEUTRAL_RESTART: " << *data << endl;
            break;
            
          case 'k':
            if (team == YELLOW)
              newInfo.command = KICK_OFF_US;
            else 
              newInfo.command = KICK_OFF_THEM;
            //cout << "KICK_OFF: " << *data << endl;
            break;
            
          case 'K':
            if (team == YELLOW)
              newInfo.command = KICK_OFF_THEM;
            else 
              newInfo.command = KICK_OFF_US;
            //cout << "KICK_OFF: " << *data << endl;
            break;  
            
          case 'p':
            if (team == YELLOW)
              newInfo.command = PENALTY_US;
            else 
              newInfo.command = PENALTY_THEM;
            //cout << "PENALTY: " << *data << endl;
            break;
            
          case 'P':
            if (team == YELLOW)
              newInfo.command = PENALTY_THEM;
            else 
              newInfo.command = PENALTY_US;
            //cout << "PENALTY: " << *data << endl;
            break;  
            
          case 'f':
            if (team == YELLOW)
              newInfo.command = FREE_KICK_US;
            else 
              newInfo.command = FREE_KICK_THEM;
            //cout << "FREE_KICK: " << *data << endl;
            break;
            
          case 'F':
            if (team == YELLOW)
              newInfo.command = FREE_KICK_THEM;
            else 
              newInfo.command = FREE_KICK_US;
            //cout << "FREE_KICK: " << *data << endl;
            break;  
            
          case 't':
            if (team == YELLOW)
              newInfo.command = TIMEOUT_US;
            else 
              newInfo.command = TIMEOUT_THEM;
            //cout << "TIMEOUT: " << *data << endl;
            break;
            
          case 'T':
            if (team == YELLOW)
              newInfo.command = TIMEOUT_THEM;
            else 
              newInfo.command = TIMEOUT_US;
            //cout << "TIMEOUT: " << *data << endl;
            break;  
            
          case 'z':
            newInfo.command = TIMEOUT_END;
            //cout << "TIMOUT END" << endl;
            break;
            
          case 'g':
            if (team == YELLOW)
              newInfo.command = GOAL_US;
            else 
              newInfo.command = GOAL_THEM;
            //cout << "GOAL: " << *data << endl;
            break;
            
          case 'G':
            if (team == YELLOW)
              newInfo.command = GOAL_THEM;
            else 
              newInfo.command = GOAL_US;
            //cout << "GOAL: " << *data << endl;
            break;  
            
          case ' ':
            // cout << "empty" << endl;
            newInfo.command = CONNECTED;
            
            break;
            
          default:
            break;
          }
          
          newInfo.time = time;
          cmdQueue.push(newInfo);
        }
        // This compares the char received and sets the correct event.
        
        time++;
        if (time == LONG_MAX){
          time = 0;
        }
      } 
  }
  
  cout << "Ending Referee Box Thread" << endl;
  
}


// This stops the loop from running.  => stops reading input from serial port.
void RefBoxInterface::end() {
  cout << "Ending Referee Box Interface" << endl;
  running = false;
}

// This returns the next command received.  A queue is implemented, so FIFO ordering 
// of the received commands is kept.
refBoxCommand RefBoxInterface::getEvent() {
  // Check if queue is empty.
  if (cmdQueue.empty())
    return EMPTY;

  // Get first command off queue and return the command.
  RefBoxInfo theCmd = cmdQueue.front();
  cmdQueue.pop();

  return theCmd.command;  
}

  