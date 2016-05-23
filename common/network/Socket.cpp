#include "Socket.h"
#include "iostream.h"

#ifdef _DEBUG
  #define SOCKET_DEBUG
#endif

#define DISABLE_NAGLE_ALGORITHM

#define USE_RECEIVE_TIMEOUT


static bool WinsockInitialized = false;

Socket::Socket() {

  // Initialize WinSock
  if (!WinsockInitialized) {
    WSAData wsaData;
	  WSAStartup(MAKEWORD(1, 1), &wsaData);

    WinsockInitialized = true;
  }
  
  isConnected = false;
  theSocket = NULL;
}

// Opens a theSocket connection
bool Socket::openConnection( char* hostname, int port ) 
{

  // Find the server's address
  u_long nRemoteAddr = inet_addr(hostname);
  if (nRemoteAddr == INADDR_NONE) {
    // pcHost isn't a dotted IP, so resolve it through DNS
    hostent* pHE = gethostbyname(hostname);
    if (pHE == 0) {
        return false;
    }
    nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
  }
  if (nRemoteAddr == INADDR_NONE) return false;


  // Create address
  in_addr Address;
  memcpy(&Address, &nRemoteAddr, sizeof(u_long)); 

  // Create a stream theSocket
  theSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (theSocket != INVALID_SOCKET) {
    sockaddr_in sinRemote;
    sinRemote.sin_family = AF_INET;
    sinRemote.sin_addr.s_addr = nRemoteAddr;
    sinRemote.sin_port = htons(port);
    if (connect(theSocket, (sockaddr*)&sinRemote, sizeof(sockaddr_in)) == SOCKET_ERROR) {
      theSocket = NULL;
      return false;
    }
  }

#ifdef DISABLE_NAGLE_ALGORITHM
  bool noDelay = true;
  setsockopt(theSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&noDelay, sizeof(bool));
#endif

#ifdef USE_RECEIVE_TIMEOUT
  int timeout = 5000;
  setsockopt(theSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int));
#endif

  isConnected = true;
  return true;
}


// Opens a theSocket listener
bool Socket::openListener( char* hostname, int port ) 
{

  // Find the server's address
  u_long nInterfaceAddr = inet_addr(hostname);
  if (nInterfaceAddr != INADDR_NONE) {

    theSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (theSocket != INVALID_SOCKET) {
    
      sockaddr_in sinInterface;
      sinInterface.sin_family = AF_INET;
      sinInterface.sin_addr.s_addr = nInterfaceAddr;
      sinInterface.sin_port = htons(port);
      
      // Bind the theSocket and start listening
      if (bind(theSocket, (sockaddr*)&sinInterface, 
        sizeof(sockaddr_in)) != SOCKET_ERROR) {
        listen(theSocket, SOMAXCONN);
        return true;
      }
    }
  }

  theSocket = NULL;
  return false;

}

// Accepts incoming connections
Socket* Socket::acceptConnection()
{
  Socket* newSocket = new Socket();

  sockaddr_in sinRemote;
  int nAddrSize = sizeof(sinRemote);

  newSocket->theSocket = accept(theSocket, (sockaddr*)&sinRemote,
                &nAddrSize);

  if (newSocket->theSocket != INVALID_SOCKET) {
    newSocket->isConnected = true;

#ifdef DISABLE_NAGLE_ALGORITHM
    bool noDelay = true;
    setsockopt(newSocket->theSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&noDelay, sizeof(bool));
#endif

  }
  else {
    newSocket->theSocket = NULL;
    newSocket->isConnected = false;
  }

  return newSocket;
}



// Returns true if the theSocket is open
bool Socket::isOpen() {
  return isConnected;
}

// Closes a theSocket connection
void Socket::close() { 
  closesocket(theSocket);
  isConnected = false; 
}

// Sends a packet of data
bool Socket::sendData( char* message, int size ) 
{
  int flags = 0;//MSG_OOB;
  if (! isOpen() ) return false;

  int result = send(theSocket, (char*)message, size, flags);

  // Did we successfully send?
  if (result == size) {
    return true;
  }
  // Only part of message sent, send the remainder
  else if ((SOCKET_ERROR != result) && (result < size) && (result > 0)){
    return sendData( (char*)(message + result), size - result);
  }
  // We encountered an error
  else {
    #ifdef SOCKET_DEBUG
      cout << "Socket Error: send(message," << size << ") = " << WSAGetLastError() << endl;
    #endif

    close();
    return false;
  }
  
}

// Receives a packet of data
bool Socket::receiveData( char* message, int size )
{
  int flags = 0;//MSG_OOB;
  if (! isOpen() ) return false;

  int result = recv(theSocket, (char*)message, size, flags);

  // Did we successfully receive?
  if (result == size) {
    return true;
  }
  // Only part of message received, receive the remainder
  else if ((SOCKET_ERROR != result) && (result < size) && (result > 0)){
    return receiveData( (char*)(message + result), size - result);
  }
  // We encountered an error
  else  {

    #ifdef SOCKET_DEBUG
      cout << "Socket Error: recv(message," << size << ") = " << WSAGetLastError() << endl;
    #endif

    memset(message, 0, size);
    close();
    return false;
  }

}

// Sets proper flags for the socket
void Socket::setBuffer(int bufSize){

//  int intSize = sizeof(int);
//  int buffer = 0;
//  buffer = bufSize;

//  setsockopt(theSocket, SOL_SOCKET, SO_SNDBUF, (char*)&bufSize, intSize);
//  setsockopt(theSocket, SOL_SOCKET, SO_RCVBUF, (char*)&bufSize, intSize);
}
