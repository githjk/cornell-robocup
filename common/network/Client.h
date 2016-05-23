#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.h"
#include "CommType.h"

class Client {
public:

  Client(CommType clientType);
  Client(CommType clientType, const char* hostname, int port);

  bool openConnection( const char* hostname, int port );
  bool openConnection( );

  bool isOpen();  
  void close();

  bool send( void* message, int size );
  bool receive( void* message, int size );

  void setSendBuffer(int bufSize);
  void setReceiveBuffer(int bufSize);


private:

  char* hostname;
  int port;

  CommType clientType;
  CommType serverType;
  Socket* theSocket;

  bool sendCheck, receiveCheck;

};


#endif // Client_H






