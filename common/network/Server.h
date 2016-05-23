#ifndef SERVER_H
#define SERVER_H

#include "Socket.h"
#include "CommType.h"

class Server {
public:

  Server(CommType serverType, int MaxSend, int MaxReceive );

  // The startLister() function will call the listen() function
  bool startListener( char* hostname, int port );
  void listen();
  
  void sendAll( void* message, int size );

  bool send(int clientIndex, void* message, int size); 
  bool receive(int clientIndex, void* message, int size );

  void setSendBuffer(int bufSize);
  void setReceiveBuffer(int bufSize);

private:

  int sendBuffer;
  int receiveBuffer;

  void addSender(Socket* theSocket);
  void addReceiver(Socket* theSocket);
  void addSenderReceiver(Socket* theSocket);

  CommType serverType;

  bool alreadyListening;
  Socket* listener;

  Socket** arrayOfSockets;
  CommType* clientTypes;
  bool* newConnection;

  int *sendIndex, sendLoop;
  int *receiveIndex, receiveLoop;

  int MaxSend, MaxReceive, MaxConnections;

};


#endif // SERVER_H






