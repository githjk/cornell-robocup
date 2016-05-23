#include "stdafx.h"
#include "SerialPortInterface.h"
#include <memory.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  SerialPortInterface RCSObject;
  char *portName = "COM1";
  char *sendData = "a1234567890123456789012345b";
  char *data = (char *) malloc(10000);
  int errorCode;
  char done = 0;
  unsigned long len;

	printf("Initing Serial Port %s\n",portName);
  errorCode = RCSObject.OpenConnection(portName);
  printf("Error code: %d\n",errorCode);
	printf("Sending data %s on Serial Port %s (%d bytes)\n",sendData,portName,27);
     errorCode = RCSObject.SendData(sendData,27 /*Don't send null terminator*/);
  
  
  printf("Error code: %d\n",errorCode);
	
  while(!done) {
      errorCode = RCSObject.ReceiveData(data,1,&len);\
      if (errorCode != RCS_INTERFACE_DATA_RECEIVED)
        printf("ERROR: %d\n", errorCode);
      if (len > 0) {
        printf("Recieved: \"%c\"\n",*data);
        if (*data == 7)
          done = 1;
      } //else printf("NONE\n");
  }
  
  free(data);
  
  printf("Closing Serial Port %s\n",portName);
  errorCode = RCSObject.CloseConnection();
  printf("Error code: %d\n",errorCode);

  return 0;
}
