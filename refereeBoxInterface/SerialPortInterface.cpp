/* SerialPortInterface.cpp : Defines the entry point for the console application.
   
   This is a simple interface to the serial port.  It uses blocking IO, so if you call
   read or write, your thread WILL be blocked until the operation completes.

   Each read/write operation returns an error code.  Check these please!  Also, timeout options
   can be adjusted through appropriate methods.
   
*/

//#include "stdafx.h"
//#include <windows.h>
#include "SerialPortInterface.h"


/*  Private variables

  HANDLE hComm;//Communications handle
  COMMTIMEOUTS timeouts;//Timeout settings
  int writeTimeoutRetries;//Number of times to try before throwing error on timeout
  
*/



SerialPortInterface::SerialPortInterface() {
  writeTimeoutRetries = 10;
  readTimeoutRetries = 1000;
}

int SerialPortInterface::OpenConnection(char* portName, int baudRate, int byteSize, int parity, int stopBits) {
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
    return S_INTERFACE_CANNOT_OPEN_PORT;
  
  //Set port state
  if (!SetCommState(hComm, &dcb))
    return S_INTERFACE_CANNOT_SET_COM_STATE;
  
  //Set port timeouts
  //Microsoft Recommended Settings
  timeouts.ReadIntervalTimeout = 20;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.ReadTotalTimeoutConstant = 100;
  timeouts.WriteTotalTimeoutConstant = 10;
  timeouts.WriteTotalTimeoutMultiplier = 100;
  
  if (!SetCommTimeouts(hComm, &timeouts))
    return S_INTERFACE_CANNOT_SET_COM_TIMEOUT;
  
  
  //Return
  return 0;
}

int SerialPortInterface::CloseConnection() {
  //Close port
  CloseHandle(hComm);
  //Return
  return 0;
}

int SerialPortInterface::SendData(char *data, unsigned long dataLen) {
  int triesLeft = writeTimeoutRetries;
  unsigned long bytesWritten;
  int errorCode;
  //Timeout loop
  
  if (hComm == INVALID_HANDLE_VALUE)
    return S_INTERFACE_PORT_CLOSED;

  while(triesLeft > 0) {
    //Send data to port
    errorCode = WriteFile(hComm,data,dataLen,&bytesWritten,0);

    if (!errorCode)
      return GetLastError();//Return error if there is one

    //Check for timeout
    if (bytesWritten == dataLen)
      return S_INTERFACE_DATA_SENT;//No error

    data += bytesWritten;

    //Subtract one from tries
    triesLeft--;
  }

  return S_INTERFACE_TIMEOUT;
}


int SerialPortInterface::ReceiveData(char *data, unsigned long bytesToRead, unsigned long *dataLen) {
  int triesLeft = readTimeoutRetries;
  int errorCode;
  //Timeout loop
  
  //printf("Calling RD.  DATAP: %x  bytesToRead: %d  dataLenP: %x \n", data, bytesToRead, dataLen);
  
  if (hComm == INVALID_HANDLE_VALUE)
    return S_INTERFACE_PORT_CLOSED;

  while(triesLeft > 0) {
    //Send data to port
    errorCode = ReadFile(hComm,data,bytesToRead,dataLen,0);

    if (!errorCode)
      return GetLastError();//Return error if there is one

    //Check for timeout
    if (*dataLen >= 0)
      return S_INTERFACE_DATA_RECEIVED;//No error

    //Subtract one from tries
    triesLeft--;
  }

  return S_INTERFACE_TIMEOUT;
}


//Settings stuff


//Retries settings

int  SerialPortInterface::getSendRetries() { return writeTimeoutRetries; }

void SerialPortInterface::setSendRetries(int retries) { writeTimeoutRetries = retries; }

int  SerialPortInterface::getReceiveRetries() { return readTimeoutRetries; }

void SerialPortInterface::setReceiveRetries(int retries) { readTimeoutRetries = retries; }

//Timeout settings (in millieconds)

//The maximum time in ms between charactors
int  SerialPortInterface::getReadIntervalTimeout() { return timeouts.ReadIntervalTimeout; }

int  SerialPortInterface:: setReadIntervalTimeout(int newVal) {
  timeouts.ReadIntervalTimeout = newVal;
  if (!SetCommTimeouts(hComm, &timeouts))
    return S_INTERFACE_CANNOT_SET_COM_TIMEOUT;
  return S_INTERFACE_NOERR;
}

//Total timeout = timeout_multiplier * num_bytes + timeout_constant
int  SerialPortInterface::getReadTotalTimeoutMultiplier() { return timeouts.ReadTotalTimeoutMultiplier; }

int  SerialPortInterface::setReadTotalTimeoutMultiplier(int newVal) {
  timeouts.ReadTotalTimeoutMultiplier = newVal;
  if (!SetCommTimeouts(hComm, &timeouts))
    return S_INTERFACE_CANNOT_SET_COM_TIMEOUT;
  return S_INTERFACE_NOERR;
}

//Total timeout = timeout_multiplier * num_bytes + timeout_constant
int  SerialPortInterface::getReadTotalTimeoutConstant() { return timeouts.ReadTotalTimeoutConstant; }

int  SerialPortInterface::setReadTotalTimeoutConstant(int newVal) {
  timeouts.ReadTotalTimeoutConstant = newVal;
  if (!SetCommTimeouts(hComm, &timeouts))
    return S_INTERFACE_CANNOT_SET_COM_TIMEOUT;
  return S_INTERFACE_NOERR;
}

//Total timeout = timeout_multiplier * num_bytes + timeout_constant
int  SerialPortInterface::getWriteTotalTimoutMultiplier()  { return timeouts.WriteTotalTimeoutMultiplier; }

int  SerialPortInterface::setWriteTotalTimoutMultiplier(int newVal) {
  timeouts.WriteTotalTimeoutMultiplier = newVal;
  if (!SetCommTimeouts(hComm, &timeouts))
    return S_INTERFACE_CANNOT_SET_COM_TIMEOUT;
  return S_INTERFACE_NOERR;
}

//Total timeout = timeout_multiplier * num_bytes + timeout_constant
int  SerialPortInterface::getWriteTotalTimeoutConstant()  { return timeouts.WriteTotalTimeoutConstant; }

int  SerialPortInterface::setWriteTotalTimeoutConstant(int newVal) {
  timeouts.WriteTotalTimeoutConstant = newVal;
  if (!SetCommTimeouts(hComm, &timeouts))
    return S_INTERFACE_CANNOT_SET_COM_TIMEOUT;
  return S_INTERFACE_NOERR;
}
