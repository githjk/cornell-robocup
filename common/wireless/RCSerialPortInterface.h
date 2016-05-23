
#ifndef RCSPI_HEADER
#define RCSPI_HEADER 
#include <windows.h>

#define PACKET_SIZE 25


//The wireless packet frame bytes for new Tx board (multi-module board)
#define DATA_PACKET_5   25
#define START_BYTE_5_1  0x0F
#define START_BYTE_5_2  0xCC
#define STOP_BYTE_5     0xEE

#define DATA_PACKET_11  50
#define START_BYTE_11_1 0x0F
#define START_BYTE_11_2 0x5A
#define STOP_BYTE_11    0x11

// The wireless header and footer bytes.
#define RPC_HEADER_CHAR 0xFF
#define RPC_FOOTER_CHAR 0xFE
#define RPC_MIDDLE_CHAR 0xFD

#define  RCS_INTERFACE_CANNOT_GET_COM_STATE       42
#define  RCS_INTERFACE_CANNOT_SET_COM_STATE       43
#define  RCS_INTERFACE_CANNOT_SET_COM_TIMEOUT     44
#define  RCS_INTERFACE_CANNOT_OPEN_PORT           45
#define  RCS_INTERFACE_PORT_CLOSED                46
#define  RCS_INTERFACE_TIMEOUT                    666
#define  RCS_INTERFACE_DATA_SENT                  -1

class RCSerialPortInterface {
private:
  HANDLE hComm;//Communications handle
  COMMTIMEOUTS timeouts;//Timeout settings
  int timeoutTries;//Number of times to try before throwing error on timeout
  
  
public:
  
  RCSerialPortInterface();
  
  void setBoard(int type);

  int openConnection(char* portName, int baudRate = 115200, int byteSize = 8, int parity = NOPARITY, int stopBits = ONESTOPBIT);
  
  int closeConnection();
  
  int sendPacket(char *data);
  int sendFirstPacket(char *data);
  
  char header[1];
  char middle[1];
  char footer[1];

  //frame bytes for the new 2002 Tx interface (2 start, 1 stop byte)
  char startFrame[2];
  char stopFrame[1];
  int dataPacketSize;

  //Which wireless board are we using
  //1 = JinWoo Board
  //2 = Michael Jordan Board
  int wirelessBoard;
  bool elevenOnElevenMode;

};


#endif