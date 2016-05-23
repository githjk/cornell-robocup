/** 
 * RefBoxInterface Header.
 * @author Joran Siu and Justin Wick
 */

#ifndef RBI_HEADER
#define RBI_HEADER


#include "SerialPortInterface.h"
#include "RefBoxInfo.h"
#include <queue>

using namespace std;

typedef enum teamColor {
  YELLOW = 0,
  BLUE
} teamColor;


/** 
  RefBoxInterface Class
 */
class RefBoxInterface 
{

private:
  bool running;//If the listener thread is running
  SerialPortInterface SPort;
  teamColor team;
  unsigned long len;
  queue<RefBoxInfo> cmdQueue;
  RefBoxInfo currentCmd;
  char *data;  

public:
  // Constructor
  RefBoxInterface();    // Defaults to Yellow team and COM1

  // Destructor.
  ~RefBoxInterface();

  // Initialize method.  This sets up the serial connection and team information.  Serial Connection parameters can be specified here.
  int initialize(teamColor color, 
                 char* portName, 
//                 int baudRate = 115200, 
                 int baudRate = 9600, 
                 int byteSize = 8, 
                 int parity = NOPARITY, 
                 int stopBits = ONESTOPBIT);

  // This starts the thread that listens to the serial port for roboref signals.
  int start();

  // This is the main processing loop that filters through the received input from 
  // refbox and stores each input into a queue.
  void loop();
  
  // This stops the loop from running.  => stops reading input from serial port.
  void end();
  
  // This returns the next command received.  A queue is implemented, so FIFO ordering 
  // of the received commands is kept.
  refBoxCommand getEvent();

};

#endif