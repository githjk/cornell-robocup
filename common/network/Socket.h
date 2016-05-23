#ifndef SOCKET_H
#define SOCKET_H

#include <winsock.h>

class Socket {
public:

  Socket();

  bool openConnection( char* hostname, int port );
  bool openListener( char* hostname, int port );
  
  void setBuffer(int bufSize);

  bool isOpen();  
  void close();

  bool sendData( char* message, int size );
  bool receiveData( char* message, int size );

  Socket* acceptConnection();

private:

  bool isConnected;
  SOCKET theSocket;

};


#endif // SOCKET_H

