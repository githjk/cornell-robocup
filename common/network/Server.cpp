#include "Server.h"
#include <stdio.h>
#include <iostream.h>

// Constructor
Server::  Server(
    CommType serverType,
    int MaxSend,
    int MaxReceive
  )
{

  this->serverType = serverType;

  if (MaxSend < 0) this->MaxSend = 0;
  else this->MaxSend = MaxSend;

  if (MaxReceive < 0) this->MaxReceive = 0;
  else this->MaxReceive = MaxReceive;

  MaxConnections = this->MaxSend + this->MaxReceive;

  alreadyListening = false;

  // Initialize array of Sockets
  arrayOfSockets = (Socket**)malloc( MaxConnections*sizeof(Socket*) );
  newConnection = (bool*)malloc( MaxConnections*sizeof(bool) );
  for (int i=0; i < MaxConnections; i++) {
    arrayOfSockets[i] = new Socket();
    newConnection[i] = false;
  }


  // Initialize send index
  sendIndex = (int*)malloc( MaxSend*sizeof(int) );
  for (i=0; i < MaxSend; i++) {
    sendIndex[i] = -1;
  }

  // Initialize receive index
  receiveIndex = (int*)malloc( MaxReceive*sizeof(int) );
  for (i=0; i < MaxReceive; i++) {
    receiveIndex[i] = -1;
  }

  sendLoop = 0;
  receiveLoop = 0;

  sendBuffer = 0;
  receiveBuffer = 0;

}


// Sends data to all receiving clients
void Server::sendAll( void* message, int size) {
  //cout << "MaxSend " << MaxSend << endl;
  for(int i=0; i < MaxSend; i++) {
    send(i, message, size);
  }
}


// Send data to client
bool Server::send( int clientIndex, void* message, int size) {

  if ((clientIndex < 0) || (clientIndex > MaxSend) ) return false;

  // Is this a valid client index?
  int temp = sendIndex[clientIndex];
//cout << "Sending Data " << temp <<  endl;
  if ( temp < 0 ) return false;

  // Is this a newConnection?
  if ( newConnection[temp] ) {
    newConnection[temp] = false;
    
    // Reset newConnection variable for the receiver Socket*
    for (int j=MaxSend; j < MaxConnections; j++){
      if (arrayOfSockets[temp] == arrayOfSockets[j])
        newConnection[j] = false;
    }
  }

  // Send data
  if (! arrayOfSockets[temp]->sendData((char *) message, size) ){
//cout << "Sending Data to client" << temp <<  endl;
    arrayOfSockets[ temp ]->close();
    sendIndex[temp] = -1;
    return false;
  }
  
  return true;
}


// Receive data from client
bool Server::receive( int clientIndex, void* message, int size) {

  if ((clientIndex < 0) || (clientIndex > MaxReceive) ) return false;

  // Is this a valid client index?
  int temp = receiveIndex[clientIndex];

  //cout << "Receiving from " << temp << endl;
  if ( temp < 0 ) return false;

  // Is this a new connection?  If so, then wait for first send
  
   if (newConnection[temp]) return false;

  // Send data
  if (! arrayOfSockets[temp]->receiveData( (char *) message, size) ){
    arrayOfSockets[ temp ]->close();
    receiveIndex[ temp ] = -1;
    return false;
  }
  //cout<<"DONE RECEIVING"<<endl;
  return true;
}

DWORD WINAPI listenerProc(LPVOID server){

  Server* theServer = (Server*)server;
  theServer->listen();
  return true;
}

// Start listening for connections
bool Server::startListener( char* hostname, int port) {

  // Have we already called this function?
  if (alreadyListening) return false;

  // Open listener
  listener = new Socket();
  if (! listener->openListener(hostname, port) )
    return false;

  alreadyListening = true;

  // Start the listener thread
  DWORD dwThreadId; 
  HANDLE listenerThread; 
  listenerThread = CreateThread( 
                         NULL,            // no security attributes 
                         0,               // use default stack size  
                         listenerProc,    // thread function 
                         this,            // argument to thread function 
                         0,               // use default creation flags 
                         &dwThreadId);    // returns the thread identifier 

  return true;
}



// Accepts connections from the listener socket and
// assigns them to the proper client index
void Server::listen() {

  Socket* temp;
  CommType CT[1];

  bool running = true;

  while (running) {

    temp = listener->acceptConnection();
    
    cout << "SERVER: Accepting Connection" << endl;
    

    // Receive client type
    if (temp->receiveData( (char *) CT, sizeof(CT)) ) {

       

      // Can we accept this type of client?
      if ( (JUST_SEND == CT[0]) && (serverType != JUST_SEND) ) {
        CT[0] = serverType;
        addReceiver(temp);
        temp->sendData( (char *) CT, sizeof(CT) );
        cout<< "SERVER JUST SEND"<<endl;
      }
      else if ( (JUST_RECEIVE == CT[0]) && (serverType != JUST_RECEIVE) ) {
        CT[0] = serverType;
        addSender(temp);
        temp->sendData( (char *) CT, sizeof(CT) ); 
        cout<< "SERVER JUST RECEIVE"<<endl;
      }
      else if (SEND_RECEIVE == CT[0]) {// the server sends and receives
        CT[0] = serverType;
        addSenderReceiver(temp);
        temp->sendData( (char *) CT, sizeof(CT) );
        cout<< "SERVER - SEND & RECEIVE" << CT[0]<<endl;
      }
      else { // the server is confused
        temp->close();
        //cout << "SERVER is confused. :( " << endl;
      }
    }
    else {
      temp->close();
    }

  }//end_while
}

void Server::addSender(Socket* theSocket) {

  if (serverType == JUST_RECEIVE) return;

//  if (sendBuffer > 0)
//    theSocket->setSendBuffer(sendBuffer);

  //cout<<"ADDING SENDER"<<endl;
  // Find slot for new sender
    // Set receive buffer
  for (int send=0; send < MaxSend; send++) {
   
    // Is this slot open?
    if (
      ( -1 == sendIndex[send] ) ||
      ( ! arrayOfSockets[send]->isOpen() )
    )
    { 
      
      arrayOfSockets[send] = theSocket;
      sendIndex[send] = send;
      newConnection[send] = true;
      return;
    }
  }

  // If you reach this line, there is no open slot
  // Overwrite an old connection
 
  arrayOfSockets[sendLoop]->close();
  arrayOfSockets[sendLoop] = theSocket;
  sendIndex[sendLoop] = sendLoop;
  newConnection[sendLoop] = true;

  sendLoop = (sendLoop + 1) % MaxSend;
}

void Server::addReceiver(Socket* theSocket) {

  // Set receive buffer
//  if (receiveBuffer > 0)
//    theSocket->setReceiveBuffer(receiveBuffer);


  if (serverType == JUST_SEND) return;

   //cout<<"ADDING RECEIVER"<<endl;
  // Find slot for new receiver
  for (int receive=0; receive < MaxReceive; receive++) {

    // Is this slot open?
    if (
      ( -1 == receiveIndex[receive] ) ||
      ( ! arrayOfSockets[MaxSend + receive]->isOpen() )
    )
    {
      
      
      arrayOfSockets[MaxSend + receive] = theSocket;
      receiveIndex[receive] = MaxSend + receive;
      newConnection[MaxSend + receive] = true;

     // cout<<"RECEIVEINDEX "<<receiveIndex[receive];
      return;
    }
  }

  // If you reach this line, there is no open slot
  // Overwrite an old connection
  
  arrayOfSockets[MaxSend + receiveLoop]->close();
  arrayOfSockets[MaxSend + receiveLoop] = theSocket;
  receiveIndex[receiveLoop] = MaxSend + receiveLoop;
  newConnection[MaxSend + receiveLoop] = true;

  receiveLoop = (receiveLoop + 1) % MaxReceive;

}
void Server::addSenderReceiver(Socket* theSocket){
  addSender(theSocket);
  addReceiver(theSocket);
}


void Server::setSendBuffer(int bufSize) {
  sendBuffer = bufSize;

//  for (int i=0; i < MaxConnections; i++) {
//    arrayOfSockets[i]->setSendBuffer(sendBuffer);
//  }
}

void Server::setReceiveBuffer(int bufSize){
  receiveBuffer = bufSize;
//  for (int i=0; i < MaxConnections; i++) {
//    arrayOfSockets[i]->setReceiveBuffer(receiveBuffer);
//  }
}
