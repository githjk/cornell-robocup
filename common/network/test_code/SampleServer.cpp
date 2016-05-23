#define MAGIC
#include <iostream.h>
#include <conio.h>
#include <string.h>
#include "SampleServer.h"

// Important -- all the things we use are in the namespace RoboNetwork
using namespace RoboNetwork;

SampleServer::SampleServer(char *hostname, int port) : Server(hostname, port) {
	app_ = NULL;
	setSendDataSize(DATASIZE); // Allow sending 33 bytes of data
	setReceiveDataSize(DATASIZE);
	cout << ">>>>> Started connection!" << endl;
	
}

bool SampleServer::handshake(int index) {
	void* data = new char[10];
	if (0 != readSocket(connectionData_->getSocket(index), data, 10))
		return false;
	connectionData_->setHandshakeData(index,data);
	return (0 == writeSocket(connectionData_->getSocket(index), "OK", 3));
}

void SampleServer::registerClient(int index) {
	if (app_) {
		app_->addClient(index, (char*)connectionData_->getHandshakeData(index));
	}
	return;
}

void SampleServer::unregisterClient(int index) {
	if (app_) {
		app_->dropClient(index);
	}
	return;
}

Application::Application(Server *s) {
	counter_=0; 
	server_=s; 
	lastClient_ = 0;
	name_ = _strdup("[App]");
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		clientName_[i] = NULL;
		clientID_[i] = -1;
	}
	mutex_ = createMutex();
};

Application::~Application() {
	if (name_)
		delete name_;
	name_ = NULL;
	for (int i = 0; i < lastClient_; i++) {
		if (clientName_[i]) 
			delete clientName_[i];
		clientName_[i] = NULL;
	}
}

void Application::addClient(int handle, char *name) {
	lock(mutex_);
	{	
		clientName_[lastClient_] = _strdup(name);
		clientID_[lastClient_] = handle;
		lastClient_++;	
	}
	unlock(mutex_);
}

void Application::dropClient(int handle) {
	int i, j;
	
	lock(mutex_);
	{	
		for (i = 0; i < lastClient_; i++) {
			if (handle == clientID_[i])
				break;
		}
		delete clientName_[i];
		for (j = i+1; j < lastClient_; j++) {
			clientID_[j-1] = clientID_[j];
			clientName_[j-1] = clientName_[j];
		}
		clientName_[lastClient_] = NULL;
		lastClient_--;	// pretty code, ain't it?
	}
	unlock(mutex_);
}

void Application::run() {
	int counter = 0;
	int i, result;
	char data[DATASIZE];
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	while (!_kbhit()) {
		//cout << "COUNTER = " << counter << endl;
		
		lock(mutex_);
		{		
			for (i = 0; i < lastClient_; i++) {
				result = server_->receive(clientID_[i], data);
				if (result && strlen(data) > 0) {
					//cout << "RECEIVED [" << i << "] Data: \"" << data << "\"" << endl;
					// sprintf(data,"%s: %ds", clientName_[i], counter);	
					server_->send(clientID_[i], data);
					//data[0] = 0;
				}
			}					
		}
		unlock(mutex_);
		
		counter ++;
	}
}

int main (int argc, char ** argv) {
	int port = 4444;
  cout << "Datasize: " << DATASIZE << endl;

	if (argc < 2) {
		cerr << "Local IP address must be passed as a parameter. " << endl;
		cerr << "(not 127.0.0.1, which is for localhost only)" << endl;
		return -1;
	}
	
	if (argc > 2)
		port = atoi(argv[2]);

	SampleServer s = SampleServer(argv[1], port);
	Application app = Application(&s);
	s.setApp(&app);
	s.start();
	app.run();
	cout << ">>>>>>>>>>>Quitting!" << endl;
	s.stop();
	return 0;
}

