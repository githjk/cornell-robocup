//This is the Client.cpp

#include "Client.h"


Client::Client(CommType clientType) {

  this->hostname = "127.0.0.1";
  this->port = 4545;

  this->clientType = clientType;
  theSocket = new Socket();

  sendCheck = false;
  receiveCheck = false;
};

Client::Client(CommType clientType, const char* hostname, int port) {

  this->hostname = (char*)hostname;
  this->port = port;
  
  this->clientType = clientType;
  theSocket = new Socket();

  sendCheck = false;
  receiveCheck = false;
};

bool Client::openConnection(const char* hostname, int port ) {

  this->hostname = (char*)hostname;
  this->port = port;

  return openConnection();
}

bool Client::openConnection( ) {
	CommType CT[1];


  // Open connection
  if(theSocket->openConnection(hostname, port)) { 

    // Send client type
    CT[0] = clientType;
    if (! theSocket->sendData((char*)CT, sizeof(CT)) ) {
      theSocket->close();
      return false;
    }

    // Receive server type (this call will return false if the
    // clientType was refused
    if ( theSocket->receiveData((char*)CT, sizeof(CT)) ){
      serverType = CT[0];

      // Check server type
      if (JUST_SEND == serverType){
        receiveCheck = true;  // the server will only send data
      }
      else if (JUST_RECEIVE == serverType) { // the server will only receive data
        sendCheck = true;
      }
      else if (SEND_RECEIVE == serverType) { // the server sends and receives
        receiveCheck = true;
        sendCheck = true;
      }
      else { // the server is confused
        theSocket->close();
        return false;
      }

      return true;
    }
  }

	return false;
}

// Return true for openConnection, false otherwise
bool Client::isOpen() {
	return( theSocket->isOpen() );
}

// Close the connection
void Client::close() {
	theSocket->close();
}

// Send data to server
bool Client::send( void* message, int size ) {

  // Does the server accept sends?
  if(sendCheck) {
    if (theSocket->sendData((char*)message, size) )
  		return true;
  	else {
	  	theSocket->close();
		  return false;
  	}
  }

  // The server will not accept sends, but the socket remains open
  return false;
}

// Receive data from server
bool Client::receive( void* message, int size ) {

  // Does the server send data?
  if(receiveCheck) {
    if (theSocket->receiveData((char*)message, size) )
  		return true;
  	else {
	  	theSocket->close();
		  return false;
  	}
  }

  // The server will not send data, but the socket remains open
  return false;
}

//void Client::setSendBuffer(int bufSize){
//  theSocket->setSendBuffer(bufSize);
//}

//void Client::setReceiveBuffer(int bufSize){
//  theSocket->setReceiveBuffer(bufSize);
//}


// end of Client.cpp