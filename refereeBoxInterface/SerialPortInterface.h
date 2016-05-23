
#ifndef SPI_HEADER
#define SPI_HEADER
#include <windows.h>


#define  S_INTERFACE_CANNOT_GET_COM_STATE       -42
#define  S_INTERFACE_CANNOT_SET_COM_STATE       -43
#define  S_INTERFACE_CANNOT_SET_COM_TIMEOUT     -44
#define  S_INTERFACE_CANNOT_OPEN_PORT           -45
#define  S_INTERFACE_PORT_CLOSED                -46
#define  S_INTERFACE_TIMEOUT                    -666
#define  S_INTERFACE_DATA_SENT                  -1
#define  S_INTERFACE_DATA_RECEIVED              -1
#define  S_INTERFACE_NOERR                      -1



class SerialPortInterface 
{
private:
  HANDLE hComm;//Communications handle
  COMMTIMEOUTS timeouts;//Timeout settings
  int writeTimeoutRetries;//Number of times to try before throwing error on timeout
  int readTimeoutRetries;//Number of times to try before throwing error on timeout
  
  
public:
  
  SerialPortInterface();
  
  //Open/Close functions

  int OpenConnection(char* portName, int baudRate = 115200, int byteSize = 8, int parity = NOPARITY, int stopBits = ONESTOPBIT);
  
  int CloseConnection();

  //Data functions
  
  int SendData(char *data, unsigned long dataLen);

  int ReceiveData(char *data, unsigned long bytesToRead, unsigned long *dataLen);

  //Settings functions

  //Retries settings

  int getSendRetries();

  void setSendRetries(int retries);

  int getReceiveRetries();

  void setReceiveRetries(int retries);

  //Timeout settings (in millieconds)

  //The maximum time in ms between charactors
  int getReadIntervalTimeout();

  int setReadIntervalTimeout(int newVal);

  //Total timeout = timeout_multiplier * num_bytes + timeout_constant
  int getReadTotalTimeoutMultiplier();

  int setReadTotalTimeoutMultiplier(int newVal);

  //Total timeout = timeout_multiplier * num_bytes + timeout_constant
  int getReadTotalTimeoutConstant();

  int setReadTotalTimeoutConstant(int newVal);

  //Total timeout = timeout_multiplier * num_bytes + timeout_constant
  int getWriteTotalTimoutMultiplier();

  int setWriteTotalTimoutMultiplier(int newVal);

  //Total timeout = timeout_multiplier * num_bytes + timeout_constant
  int getWriteTotalTimeoutConstant();

  int setWriteTotalTimeoutConstant(int newVal);

};



#endif