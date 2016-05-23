#include "Server.h"

const int DATASIZE = 664;
// Important -- all the things we use are in the namespace RoboNetwork
using namespace RoboNetwork;

class Application;

class SampleServer : public Server {
public:
	SampleServer(char *hostname, int port);
	void setApp(Application *app) { app_  = app; };
private:
	
	Application *app_;
	bool handshake(int index);
	void registerClient(int index);
	void unregisterClient(int index);
	
};

class Application {
public:
	Application(Server *s);
	~Application();
	void addClient(int handle, char *name);
	void dropClient(int handle);
	void run();	

private:
	char *name_;
	char *clientName_[MAX_CONNECTIONS];
	int clientID_[MAX_CONNECTIONS];
	int lastClient_;
	Server *server_;
	int counter_;
	HANDLE mutex_;
};

