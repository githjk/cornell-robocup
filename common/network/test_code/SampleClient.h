#include "Client.h"
// Important -- all the things we use are in the namespace RoboNetwork

const int DATASIZE = 664;

using namespace RoboNetwork;

class SampleClient : public Client {
public:
	SampleClient(char* hostname, int port, char *id);
	~SampleClient();
	inline int send(char * string) { return Client::send((void *)string); };
	inline int receive(char *string) { return Client::receive((void *)string); };
private:
	char * id_;
	bool handshake();	
};
