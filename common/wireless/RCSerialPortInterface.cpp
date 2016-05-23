// RCSerialPortInterface.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include "RCSerialPortInterface.h"
#include "stdio.h"

#include "iostream.h"

/*  Private variables

  HANDLE hComm;//Communications handle
  COMMTIMEOUTS timeouts;//Timeout settings
  int timeoutTries;//Number of times to try before throwing error on timeout
  
*/



RCSerialPortInterface::RCSerialPortInterface() 
{
  wirelessBoard = 2;
  timeoutTries = 10;
  elevenOnElevenMode = false;

  // Build header, middle, and footer character arrays (Jin Woo Board)
  sprintf(header, "%c", RPC_HEADER_CHAR);
  sprintf(middle, "%c", RPC_MIDDLE_CHAR);
  sprintf(footer, "%c", RPC_FOOTER_CHAR);

  // Build header, middle, and footer character arrays (Michael Jordan Board)
  if(elevenOnElevenMode)
  {
    sprintf(startFrame, "%c%c", START_BYTE_11_1, START_BYTE_11_2);
    sprintf(stopFrame, "%c", STOP_BYTE_11); 
    dataPacketSize = DATA_PACKET_11;
  }
  //otherwise use 5 robot packet
  else 
  {
    sprintf(startFrame, "%c%c", START_BYTE_5_1, START_BYTE_5_2);
    sprintf(stopFrame, "%c", STOP_BYTE_5); 
    dataPacketSize = DATA_PACKET_5;
  }
}

//-----------------------------------------------------
void RCSerialPortInterface::setBoard(int type)
{
  wirelessBoard = type;
}
//-----------------------------------------------------


int RCSerialPortInterface::openConnection(char* portName, int baudRate, int byteSize, int parity, int stopBits) {

  DCB dcb = {0};//Create DCB
  dcb.DCBlength = sizeof(dcb);
  
  //Get current DCB settings
  //if (!GetCommState(hComm, &dcb))
  //  return RCS_INTERFACE_CANNOT_GET_COM_STATE;
  
  //Update DCB rate, byte size, parity, and stop bits size
  dcb.BaudRate = baudRate;
  dcb.ByteSize = byteSize;
  dcb.Parity   = parity;
  dcb.StopBits = stopBits;
  //Set event mask.  We dont' want to receive events, set to null
  dcb.EvtChar = '\0';
  
  //Update flow control settings (we don't use flow control)
  dcb.fDtrControl     = false;
  dcb.fRtsControl     = false;
  
  dcb.fOutxCtsFlow    = false;
  dcb.fOutxDsrFlow    = false;
  dcb.fDsrSensitivity = false;
  dcb.fOutX           = false;
  dcb.fInX            = false;
  dcb.fTXContinueOnXoff = false;
  dcb.XonChar         = '\0';
  dcb.XoffChar        = '\0';
  dcb.XonLim          = '\0';
  dcb.XoffLim         = '\0';
  dcb.fParity = TRUE;
  
  
  //Open port
  
  hComm = CreateFile( portName,  
    GENERIC_READ | GENERIC_WRITE, 
    0, 
    0, 
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    0);
  
  if (hComm == INVALID_HANDLE_VALUE)
    return RCS_INTERFACE_CANNOT_OPEN_PORT;
  
  //Set port state
  if (!SetCommState(hComm, &dcb))
    return RCS_INTERFACE_CANNOT_SET_COM_STATE;
  
  //Set port timeouts
  //Microsoft Recommended Settings
  timeouts.ReadIntervalTimeout = 20;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.ReadTotalTimeoutConstant = 100;
  timeouts.WriteTotalTimeoutConstant = 10; 
  timeouts.WriteTotalTimeoutMultiplier = 100;
  
  if (!SetCommTimeouts(hComm, &timeouts))
    return RCS_INTERFACE_CANNOT_SET_COM_TIMEOUT;
  
  
  //Return
  return 0;
}

int RCSerialPortInterface::closeConnection() {
  //Close port
  CloseHandle(hComm);
  //Return
  return 0;
}

int RCSerialPortInterface::sendPacket(char *data) {
  int triesLeft = timeoutTries;
  unsigned long bytesWritten;
  int errorCode;
  //Timeout loop
  
  if (hComm == INVALID_HANDLE_VALUE)
    return RCS_INTERFACE_PORT_CLOSED;

  //Michael Jordan Board
  if (wirelessBoard == 2) 
  {
    while(triesLeft > 0) {
      //------------------------------------------------------
      //Send header
      errorCode = WriteFile(hComm,startFrame,2,&bytesWritten,0);
      if (!errorCode)
        return GetLastError();
      //------------------------------------------------------
      //Send data
      errorCode = WriteFile(hComm,data,dataPacketSize,&bytesWritten,0);
      if (!errorCode)
        return GetLastError();
      //------------------------------------------------------
      //Check for timeout
      if (bytesWritten == PACKET_SIZE)
      {
        errorCode = WriteFile(hComm,stopFrame,1,&bytesWritten,0);
        if(triesLeft != timeoutTries)
        {
          cout << "Failed to get acknowledgemnt " << (timeoutTries-triesLeft) << " times." << "\n";
          cout.flush();
        }   
        return RCS_INTERFACE_DATA_SENT;//No error
      }
      //------------------------------------------------------
      //Send failed, decrement counter and send again
      triesLeft--;
      cout << "Failed to get acknowledgemnt " << (timeoutTries-triesLeft) << " times." << "\n";
      cout.flush();
      //------------------------------------------------------
    }
  }
  //Jin Woo Board
  else
  {
    while(triesLeft > 0) 
    {
      //------------------------------------------------------
      //Send header
      errorCode = WriteFile(hComm,header,1,&bytesWritten,0);
      if (!errorCode)
        return GetLastError();
      //------------------------------------------------------
      //Send data
      errorCode = WriteFile(hComm,data,PACKET_SIZE,&bytesWritten,0);
      if (!errorCode)
        return GetLastError();
      //------------------------------------------------------
      //Check for timeout
      if (bytesWritten == PACKET_SIZE)
      {
        errorCode = WriteFile(hComm,footer,1,&bytesWritten,0);
        if(triesLeft != timeoutTries)
        {
          cout << "Failed to get acknowledgemnt " << (timeoutTries-triesLeft) << " times." << "\n";
          cout.flush();
        }   
        return RCS_INTERFACE_DATA_SENT;//No error
      }
      //------------------------------------------------------
      //Send failed, decrement counter and send again
      triesLeft--;
      cout << "Failed to get acknowledgemnt " << (timeoutTries-triesLeft) << " times." << "\n";
      cout.flush();
      //------------------------------------------------------
    }
  }

  return RCS_INTERFACE_TIMEOUT;
}


int RCSerialPortInterface::sendFirstPacket(char *data) {
  int triesLeft = timeoutTries;
  unsigned long bytesWritten;
  int errorCode;
  //Timeout loop
  
  if (hComm == INVALID_HANDLE_VALUE)
    return RCS_INTERFACE_PORT_CLOSED;

  errorCode = WriteFile(hComm,header,1,&bytesWritten,0);
  while(triesLeft > 0) {
    //Send data to port
    
    errorCode = WriteFile(hComm,data,PACKET_SIZE,&bytesWritten,0);
  
    if (!errorCode)
      return GetLastError();//Return error if there is one

    //Check for timeout
    if (bytesWritten == PACKET_SIZE){
      errorCode = WriteFile(hComm,middle,1,&bytesWritten,0);
      return RCS_INTERFACE_DATA_SENT;//No error
    }
    //Subtract one from tries
    triesLeft--;
  }

  return RCS_INTERFACE_TIMEOUT;
}
